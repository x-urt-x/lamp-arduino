#include "MemManager.h"

uint16_t MemManager::reservAddr(uint16_t size, DataObjectIDEnum id)
{
	byte obj_data_count = EEPROM.read(0) & 0b0111'1111;
	uint16_t addr = 0;
	bool done = false;
	for (byte i = 0; i <= obj_data_count && !done; i++)
	{
		EEPROM.get(i * 2 + 1, addr);
		if (addr >> 12 == 0)
		{
			addr &= 0x0FFF;
			if (addr + size >= OBJ_DATA_MAX_ADDR)
			{
				LOG_USB_MEM("max data\n");
				return 0;
			}
			uint16_t next_addr = 0;
			uint16_t next_addr_id = 0;

			for (byte j = i + 1; j < OBJ_DATA_CAP; j++)
			{
				EEPROM.get(j * 2 + 1, next_addr);
				next_addr_id = next_addr & 0x0FFF;
				next_addr &= 0x0FFF;
				if (next_addr >> 12 != 0 && next_addr < addr + size)
				{
					//end of section, still not enough
					LOG_USB_MEM("end of section size = %d i = %d j = %d addr = %d next = %d\n", size, i, j, addr, next_addr);
					i = j;
					break;
				}

				if (next_addr == 0)
				{
					//add new in the end
					LOG_USB_MEM("add new in the end size = %d i = %d j = %d addr = %d next = %d\n", size, i, j, addr, next_addr);
					obj_data_count += 1;
					EEPROM.put(i * 2 + 1, (uint16_t)(addr | (id << 12)));
					EEPROM.put((i + 1) * 2 + 1, addr + size);
					done = true;
					break;
				}
				if (next_addr < addr + size)
				{
					//trying to find more in this section
					LOG_USB_MEM("next el in cur section size = %d i = %d j = %d addr = %d next = %d\n", size, i, j, addr, next_addr);
					continue;
				}
				if (next_addr == addr + size)
				{
					//exact in middle 
					LOG_USB_MEM("perfect match size = %d i = %d j = %d addr = %d next = %d\n", size, i, j, addr, next_addr);
					EEPROM.put(i * 2 + 1, (uint16_t)(addr | (id << 12)));
					done = true;
					break;
				}
				if (i + 1 == j)
				{
					//write in one

					if (next_addr_id == 0)
					{
						//move border of 2 free section
						LOG_USB_MEM("move border size = %d i = %d j = %d addr = %d next = %d\n", size, i, j, addr, next_addr);
						EEPROM.put(i * 2 + 1, (uint16_t)(addr | (id << 12)));
						EEPROM.put((i + 1) * 2 + 1, addr + size);
						done = true;
						break;
					}
					else
					{
						//devide 1 in middle into 2
						LOG_USB_MEM("shiftRight size = %d i = %d j = %d addr = %d next = %d\n", size, i, j, addr, next_addr);
						if (!shiftRight(i, 1))
						{
							//no space to shift right
							continue;
						}
						obj_data_count += 1;
						EEPROM.put(i * 2 + 1, (uint16_t)(addr | (id << 12)));
						EEPROM.put((i + 1) * 2 + 1, addr + size);
						done = true;
						break;
					}
				}
				else
				{
					//2 or more into 2
					LOG_USB_MEM("shiftLeft size = %d i = %d j = %d addr = %d next = %d\n", size, i, j, addr, next_addr);
					shiftLeft(i + 2, j - i - 2);
					obj_data_count -= j - i - 2;
					EEPROM.put(i * 2 + 1, (uint16_t)(addr | (id << 12)));
					EEPROM.put((i + 1) * 2 + 1, addr + size);
					done = true;
					break;
				}
			}
		}
	}
	if (!done)
	{
		LOG_USB_MEM("no place\n");
		return 0;
	}
	EEPROM.write(0, obj_data_count | EEPROM.read(0) & 0b1000'0000);
	EEPROM.commit();
	return addr;
}

void MemManager::deleteAddr(byte pos)
{
	if (pos >= OBJ_DATA_CAP)
	{
		LOG_USB_MEM("out of range\n");
		return;
	}
	uint16_t addr = 0;
	EEPROM.get(pos * 2 + 1, addr); // first byte for counter
	addr &= 0x0FFF; //first 4 bits for obj type id
	LOG_USB_MEM("delete mem obj pos = %d with addr = %d\n", pos, addr);
	EEPROM.write(addr, 0x00); //delited
	EEPROM.put(pos * 2 + 1, addr);
	EEPROM.commit();
}

DataObjAddrArr MemManager::getAllById(DataObjectIDEnum id)
{
	byte obj_data_count = EEPROM.read(0) & 0b0111'1111;
	byte len = 0;
	uint16_t addr = 0;
	for (byte pos = 0; pos < obj_data_count; pos++)
	{
		EEPROM.get(pos * 2 + 1, addr); // first byte for counter
		if (addr >> 12 == id) len++;
	}
	DataObjAddrArr dataObjectAddrArr(len);
	LOG_USB_MEM("getAllById id = %d obj_data_count = %d count = %d\n", id, obj_data_count, dataObjectAddrArr.len);
	int i = 0;
	for (byte pos = 0; pos < obj_data_count; pos++)
	{
		EEPROM.get(pos * 2 + 1, addr); // first byte for counter
		if (addr >> 12 == id)
		{
			dataObjectAddrArr.addrArr[i] = addr & 0x0FFF;
			dataObjectAddrArr.posArr[i] = pos;
			i++;
		}
	}
	return dataObjectAddrArr;
}

void MemManager::deleteAllAddrOnDelitedDataObj()
{
	byte obj_data_count = EEPROM.read(0) & 0b0111'1111;
	uint16_t addr = 0;
	for (byte i = 0; i < obj_data_count; i++)
	{
		EEPROM.get(i * 2 + 1, addr);
		LOG_USB_MEM("clear deleted mem obj pos = %d with addr = %d obj id = %d\n", i, (addr & 0x0FFF), EEPROM.read(addr & 0x0FFF));
		if (EEPROM.read(addr & 0x0FFF) == 0x00) deleteAddr(i);
	}
}

void MemManager::shiftLeft(byte posTo, byte shift)
{
	byte obj_data_count = EEPROM.read(0) & 0b0111'1111;
	uint16_t addr = 0;
	for (byte i = posTo; i < obj_data_count; i++)
	{
		if (i + shift >= OBJ_DATA_CAP)
		{
			EEPROM.put(i * 2 + 1, (uint16_t)0x0000);
		}
		else
		{
			EEPROM.get((i + shift) * 2 + 1, addr);
			EEPROM.put(i * 2 + 1, addr);
		}
	}
}

bool MemManager::shiftRight(byte posFrom, byte shift)
{
	if (posFrom + shift >= OBJ_DATA_CAP)
	{
		LOG_USB_MEM("no place to shift\n");
		return false;
	}
	byte obj_data_count = EEPROM.read(0) & 0b0111'1111;
	uint16_t addr = 0;
	for (byte i = posFrom; i < obj_data_count - shift; i++)
	{
		if (i < shift)
		{
			EEPROM.put(i * 2 + 1, (uint16_t)0x0000);
		}
		else
		{
			EEPROM.get(i * 2 + 1, addr);
			EEPROM.put((i + shift) * 2 + 1, addr);
		}
	}
	return true;
}

DataObjAddrArr::DataObjAddrArr(byte len)
{
	this->len = len;
	addrArr = new uint16_t[len];
	posArr = new byte[len];
	count++;
}

DataObjAddrArr::DataObjAddrArr(const DataObjAddrArr& obj)
{
	len = obj.len;
	addrArr = obj.addrArr;
	posArr = obj.posArr;
	count++;
}

DataObjAddrArr::~DataObjAddrArr()
{
	count--;
	if (count == 0)
	{
		if (addrArr) delete[] addrArr;
		if (posArr) delete[] posArr;
	}
}

unsigned int DataObjAddrArr::count = 0;
