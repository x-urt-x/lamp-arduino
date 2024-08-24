#include "Strip_class.h"
#include "log.h"

#define EB_NO_FOR
#define EB_NO_CALLBACK
#define EB_DEB_TIME 50     // таймаут гашения дребезга кнопки (кнопка)
#define EB_CLICK_TIME 500  // таймаут ожидания кликов (кнопка)
#define EB_HOLD_TIME 1500  // таймаут удержания (кнопка)
#define EB_STEP_TIME 200   // таймаут импульсного удержания (кнопка)
#define EB_FAST_TIME 30    // таймаут быстрого поворота (энкодер)
#include <EncButton.h>

#define ENC_S1 D0
#define ENC_S2 D5
#define ENC_KEY D6
#define inbr_cur_PIN A0
#define GREEN_PIN D1
#define RED_PIN D2
#define ORNG_PIN D3
#define STRIP_PIN D4

#define STRIP_LED_COUNT 100



EncButtonT<ENC_S1, ENC_S2, ENC_KEY> encoder;


Strip strip(STRIP_LED_COUNT, STRIP_PIN);

unsigned long strip_update_prev_time = 0;
unsigned long strip_update_cur_time = 0;
uint strip_update_delay_time = 2000;

void setup() {

  Serial.begin(115200);
  Serial.println("");
  strip.begin();
  strip.fill(strip.Color(0, 0, 0));
  delay(0);
  strip.show();

  pinMode(inbr_cur_PIN, INPUT);
  pinMode(GREEN_PIN, OUTPUT);
  digitalWrite(GREEN_PIN, LOW);
  pinMode(ORNG_PIN, OUTPUT);
  digitalWrite(ORNG_PIN, LOW);
  pinMode(RED_PIN, OUTPUT);
  digitalWrite(RED_PIN, LOW);

  strip.set_main_color(Color_str(50, 50, 50));
  strip.eff_singleColor();
  randomSeed(analogRead(0));
}

int br = 5000;
int enc_select = 1;
volatile bool is_enc = false;
int d = 200;

void loop() {
    encoder.tick();
   if (encoder.turn()) {
        if(encoder.pressing())
        {
          br += (encoder.fast() ? 500 : 100) * encoder.dir();

        }
        else
        {
          br += (encoder.fast() ? 25 : 1) * encoder.dir();
        }
              //inbr_mid += (encoder.pressing() ? 5 : 1) * encoder.dir();
              
              if(br > 247335)
                br = 247335;
              if(br < 0)
                br = 0;
              LOG_USB_ENC("%d\n",br);
      }

  if (Serial.available() > 0) {
    String inputstr = Serial.readStringUntil('\n');
    char key = inputstr[0];
    int input = inputstr.substring(1).toInt();
    switch(key) {
      case 'b':
        br = input;
        break;
      case 'd':
        d = input;
        break;
      case 'a':
        strip._br_cutoff_bound = input;
        break;
      case 'c':
        Color_str col(0,0,0);
        col.set(strtoul(inputstr.substring(1).c_str(), nullptr, 16));
        strip.set_main_color(col);
        break;
      
    }
  }

  #if defined(LOG_USB_BR_ENABLE) || defined(LOG_USB_COLOR_MAP_ENABLE)
  strip_update_cur_time = millis();
  if (strip_update_cur_time - strip_update_prev_time > strip_update_delay_time) 
  {
    strip_update_prev_time = strip_update_cur_time;
    Serial.printf("\n");
    #else
    {
    #endif
    //strip.eff_singleColor();
    
    strip.eff_fire();
    delay(d);
    //strip.eff_halfSingleColor();
    strip.set_br(br);
    strip.apply_br();
    strip.show();
    
  }
}








/*
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <Adafruit_NeoPixel.h>
#include <WiFiUdp.h>
#include "Pass.h" //  define in this file SSID and PASS of ur wifi

#define EB_NO_FOR
#define EB_NO_CALLBACK
#define EB_DEB_TIME 50     // таймаут гашения дребезга кнопки (кнопка)
#define EB_CLICK_TIME 500  // таймаут ожидания кликов (кнопка)
#define EB_HOLD_TIME 1500   // таймаут удержания (кнопка)
#define EB_STEP_TIME 200   // таймаут импульсного удержания (кнопка)
#define EB_FAST_TIME 30    // таймаут быстрого поворота (энкодер)
#include <EncButton.h>

#define ENC_S1 D0
#define ENC_S2 D5
#define ENC_KEY D6
#define inbr_cur_PIN A0
#define GREEN_PIN D1
#define RED_PIN D2
#define ORNG_PIN D3
#define STRIP_PIN D4

#define STRIP_LED_COUNT 100
//#define USB_LOG

const char* ssid = SSID;
const char* password = PASS;
const unsigned int localPort = 8888;

unsigned int br_edge = 30;

class MyStrip
{
byte r, g, b, w, m_r, m_g, m_b, m_w;
  float br;
};

struct rgbwColor {
  byte r, g, b, w, m_r, m_g, m_b, m_w;
  float br;

  rgbwColor(byte red, byte green, byte blue, byte white, byte br = 100)
    : r(red), g(green), b(blue), w(white), br(br/255.0f) {
  }

  uint32_t get() {
    return (uint32_t)r << 16 | (uint32_t)g << 8 | b | (uint32_t)w << 24;
  }

  void set(uint32_t colorValue) {
    r = (colorValue >> 16) & 0xFF;
    g = (colorValue >> 8) & 0xFF;
    b = colorValue & 0xFF;
    w = (colorValue >> 24) & 0xFF;
  }

  uint32_t m_get() {
    return (uint32_t)m_r << 16 | (uint32_t)m_g << 8 | m_b | (uint32_t)m_w << 24;
  }

  void map() {
    m_r = r * br;
    m_g = g * br;
    m_b = b * br;
    m_w = w * br;
  }

   void map(float _br) {
    m_r = constrain(r * _br, 0, 255);
    m_g = constrain(g * _br, 0, 255);
    m_b = constrain(b * _br, 0, 255);
    m_w = constrain(w * _br, 0, 255);
  }

  void over_map(float _br) {
    m_r = constrain(m_r * _br, 0, 255);
    m_g = constrain(m_g * _br, 0, 255);
    m_b = constrain(m_b * _br, 0, 255);
    m_w = constrain(m_w * _br, 0, 255);
  }

  void dbl_map(float _br) {
    m_r = r * _br * br;
    m_g = g * _br * br;
    m_b = b * _br * br;
    m_w = w * _br * br;
  }

  byte get_avg()
  {
    return (m_r + m_g + m_b + m_w)/4;
  }
};

////////////////////////////////////
char inbytes[4];
bool state = true;

unsigned long strip_update_prev_time = 0;
unsigned long strip_update_cur_time = 0;
uint strip_update_delay_time = 100;
int effect_speed = 5;
void(*effect_cur)();

bool is_inbr = false;
int inbr_cur = 100;
int inbr_mid = 500;
int inbr_dist = 200;

int inbr_cur_old = 100;
int inbr_cur_new = 100;
int br_delta = 0;
float br_cor = 1.04;

int enc_select = 1;
volatile bool is_enc = false;
EncButtonT<ENC_S1, ENC_S2, ENC_KEY> encoder;

rgbwColor stripColor(255, 255, 255, 255);

Adafruit_NeoPixel strip(STRIP_LED_COUNT, STRIP_PIN, NEO_GRB + NEO_KHZ800);
ESP8266WebServer server(80);
WiFiUDP Udp;
////////////////////////////////////

void changeState(bool newState) {
  if (state && !newState) {
    state = false;
    strip.fill(strip.Color(0, 0, 0, 0));
    strip.show();
  }
  if (!state && newState) {
    state = true;
    updateStrip();
  }
}

IRAM_ATTR void enc_click() {
  is_enc = true;
  digitalWrite(GREEN_PIN, HIGH);
}

////////////////////////////////////
void chooseEffect(int effect_id)
{ 
  switch(effect_id)
  {
    case 0:
      effect_cur = effect_Static;
      strip_update_delay_time = 600;
      break;
    case 1:
      effect_cur = effect_Blink;
      strip_update_delay_time = 10;
      break;
    case 2:
      effect_cur = effect_Rainbow_All;
      strip_update_delay_time = 10;
      break;
    default:
      effect_cur = effect_Static;
      strip_update_delay_time = 10000;
      break;
  }
}

uint32_t getRGBWFromAngle(int angle) {
  int hue = angle % 360;
  int sector = hue / 60;
  int remainder = hue % 60;
  int p = 0;
  int q = (255 * (60 - remainder)) / 60;
  int t = (255 * remainder) / 60;

  switch (sector) {
    case 0: return 0xFF000000 | (255 << 16) | t << 8 | p; // Red
    case 1: return 0xFF000000 | q << 16 | 255 << 8 | p; // Yellow
    case 2: return 0xFF000000 | p << 16 | 255 << 8 | t; // Green
    case 3: return 0xFF000000 | p << 16 | q << 8 | 255; // Cyan
    case 4: return 0xFF000000 | t << 16 | p << 8 | 255; // Blue
    default: return 0xFF000000 | 255 << 16 | p << 8 | q; // Magenta
  }
}


void effect_Static()
{
  if(is_inbr)
  {
    //Serial.printf("br_all = %d\tbr_lamp = %d\tbr_no_lamp = %d\n", br_all , br_lamp, br_no_lamp);
    //br_delta = inbr_cur_old - inbr_cur;
    //Serial.printf("inbr_cur = %d\tinbr_cur_old = %d\tinbr_cur_new = %d\tbr_delta = %d\n", inbr_cur, inbr_cur_old, inbr_cur_new, br_delta);
    
    int inbr_cur_cor = inbr_cur - br_delta * br_cor;
    float coef = (0.5 - float(inbr_cur_cor- inbr_mid) / (inbr_dist*2));
    //br_delta *= 0.9;
    #ifdef USB_LOG
    Serial.printf("inbr_cur_cor = %d\tcoef = %f\n", inbr_cur_cor , coef);
    #endif
    stripColor.map(coef);
  }
  else
  {
    stripColor.map();
  }
  strip.fill(stripColor.m_get());
  strip.show();
}

int cur_br = 0;
bool dir = true;
void effect_Blink()
{
  if(dir)
  {
    cur_br += effect_speed;
    if(cur_br>255)
    {
      dir=false;
      cur_br -= effect_speed;
    }
  }
  else
  {
    cur_br -= effect_speed;
    if(cur_br<0)
    {
      dir=true;
      cur_br += effect_speed;
    }
  }
  stripColor.dbl_map(cur_br/255.0f);
  strip.fill(stripColor.m_get());
  strip.show();
}

int angl=0;
void effect_Rainbow_All()
{
angl+=effect_speed;
strip.fill(getRGBWFromAngle(angl));
strip.show();
}

inline void updateStrip() {
  if (is_inbr) {
    inbr_cur = analogRead(inbr_cur_PIN);
  }
  //delay(10);
  effect_cur();
  //delay(40);
  // if (is_inbr)
  // {
  // inbr_cur_old = inbr_cur;
  // inbr_cur_new = analogRead(inbr_cur_PIN);
  // br_delta += inbr_cur_new - inbr_cur_old;
  // }
}

////////////////////////////////////

inline void handleUDP();

////////////////////////////////////
void setup() {

#ifdef USB_LOG
  Serial.begin(115200);
  Serial.println("");
#endif

  digitalWrite(RED_PIN, HIGH);
  strip.begin();
  strip.fill(strip.Color(0, 0, 0, 0));
  delay(0);
  strip.show();

  pinMode(inbr_cur_PIN, INPUT);
  pinMode(GREEN_PIN, OUTPUT);
  digitalWrite(GREEN_PIN, LOW);
  pinMode(ORNG_PIN, OUTPUT);
  digitalWrite(ORNG_PIN, LOW);
  pinMode(RED_PIN, OUTPUT);
  digitalWrite(RED_PIN, LOW);

  attachInterrupt(12, enc_click, RISING);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ORNG_PIN, HIGH);
    delay(250);
    digitalWrite(ORNG_PIN, LOW);
    delay(250);
#ifdef USB_LOG
    Serial.println("Connecting to WiFi...");
#endif
  }

#ifdef USB_LOG
  Serial.print("Connected! IP address: ");
  //Serial.println(WiFi.localIP());
#endif

  if (!SPIFFS.begin()) {
#ifdef USB_LOG
    Serial.println("Failed to mount file system");
#endif
    return;
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/", HTTP_POST, handleRoot);

  Udp.begin(localPort);
  server.begin();

#ifdef USB_LOG
  Serial.printf("UDP server on port %d\n", localPort);
  Serial.println("HTTP server started");
#endif

  digitalWrite(RED_PIN, LOW);

  stripColor.map();
  strip.fill(stripColor.m_get());
  strip.show();

  chooseEffect(0); //эффект по умолчанию
}

////////////////////////////////////
void loop() {
  server.handleClient();
  if(is_enc)
  {
    encoder.tick();
      if (encoder.hasClicks()) {
        enc_select = encoder.getClicks();
        #ifdef USB_LOG
        Serial.println(String("Select: ") + enc_select);
        #endif
      }
        if(encoder.hold())
          changeState(!state);

     if (encoder.turn()) {
        // меняем переменную
        switch (enc_select) {
            case 1:
                //inbr_mid += (encoder.pressing() ? 5 : 1) * encoder.dir();
                stripColor.br += (encoder.fast() ? 0.02 : 0.004) * encoder.dir();
                if(stripColor.br > 1)
                  stripColor.br = 1;
                if(stripColor.br < 0)
                  stripColor.br = 0;
                stripColor.map();
                updateStrip();
                break;
            case 2:
                //inbr_dist += (encoder.pressing() ? 5 : 1) * encoder.dir();
                angl += (encoder.fast() ? 10 : 1) * encoder.dir();
                if(angl > 360)
                  angl = 0;
                if(angl < 0)
                  angl = 360; 
                stripColor.set(getRGBWFromAngle(angl));
                updateStrip();
                break;
        }
        //Serial.println(String("vars ") + inbr_mid + ',' + inbr_dist + ',' + br_cor);
    }

    if(encoder.timeout(5000))
    {
      is_enc = false;
      digitalWrite(GREEN_PIN,LOW);
    }
  }
  if (state) {
    handleUDP();

    strip_update_cur_time = millis();
    if (strip_update_cur_time - strip_update_prev_time > strip_update_delay_time) {
      strip_update_prev_time = strip_update_cur_time;
      updateStrip();
    }

  }
}
////////////////////////////////////

inline void handleUDP() {
  if (Udp.parsePacket()) {
    Udp.read(inbytes, 4);

    stripColor.r = byte(inbytes[0]);
    stripColor.g = byte(inbytes[1]);
    stripColor.b = byte(inbytes[2]);
    stripColor.w = byte(inbytes[3]);

#ifdef USB_LOG
    Serial.println("UDP Received Data:");
    Serial.printf("color.r %d\n", stripColor.r);
    Serial.printf("color.g %d\n", stripColor.g);
    Serial.printf("color.b %d\n", stripColor.b);
    Serial.printf("color.w %d\n", stripColor.w);
#endif
  }
}

void handleRoot() {
  if (server.method() == HTTP_GET) {
    // Отправка HTML файла из файловой системы SPIFFS без загрузки в оперативную память
    File file = SPIFFS.open("/index.html", "r");
    if (file) {
      server.streamFile(file, "text/html");
      file.close();
    } else {
      server.send(500, "text/plain", "Internal Server Error");
    }
  } else if (server.method() == HTTP_POST) {
    // Обработка POST запроса
    String data = server.arg("plain");

    int state_ind = data.indexOf("state=") + 6;
    int brightness_ind = data.indexOf("brightness=") + 11;
    int color_ind = data.indexOf("color=") + 6;
    int effect_ind = data.indexOf("effect=") + 7;
    int strip_update_delay_time_ind = data.indexOf("strip_update_delay_time=") + 24;
    int effect_speed_ind = data.indexOf("effect_speed=") + 13;


    if (state_ind != 5)
      changeState(data.substring(state_ind, state_ind + 1) == "t");
    if (brightness_ind != 10)
      stripColor.br = data.substring(brightness_ind, brightness_ind + 3).toInt()/255.0;
    if (color_ind != 5)
      stripColor.set(0xFF000000 | strtoul(data.substring(color_ind + 1, color_ind + 7).c_str(), nullptr, 16));
    if (effect_ind != 6)
      chooseEffect(data.substring(effect_ind, effect_ind + 2).toInt());
    if (strip_update_delay_time_ind != 23)
      strip_update_delay_time=data.substring(strip_update_delay_time_ind, data.indexOf("&",strip_update_delay_time_ind)).toInt();
    if (effect_speed_ind != 12)
      effect_speed=data.substring(effect_speed_ind, data.indexOf("&",effect_speed_ind)).toInt();



    strip_update_prev_time = 0; //мгновенное обновление в след loop

#ifdef USB_LOG
    Serial.println("HTTP Received Data:");
    Serial.println("State: " + String(state));
    Serial.println("Brightness: " + String(stripColor.br));
    Serial.printf("color.r %d\n", stripColor.r);
    Serial.printf("color.g %d\n", stripColor.g);
    Serial.printf("color.b %d\n", stripColor.b);
    Serial.printf("color.w %d\n", stripColor.w);
    Serial.printf("strip_update_delay_time %d\n", strip_update_delay_time );
    Serial.printf("effect_speed %d\n", effect_speed);
#endif

    server.send(200, "text/plain", "Data received successfully");
  }
}
*/