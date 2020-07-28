/*----------------------------------------------------------------------
 * ESP8266 NODEMCU NTP BINARY INTERNET CLOCK WITH 
 * NEOPIXEL STICKS AND HIGH PRESSURE BULB BY TUENHIDIY.
----------------------------------------------------------------------*/
#include <Adafruit_NeoPixel.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <time.h>
#include "timezone.h"

#define STICK                D4    // ESP8266 NODEMCU CONTROL PIN

#define _RED                Adafruit_NeoPixel::Color(255, 0, 0)           //    RED
#define _GREEN              Adafruit_NeoPixel::Color(0, 255, 0)           //    GREEN
#define _BLUE               Adafruit_NeoPixel::Color(0, 0, 255)           //    BLUE
#define _ORANGE             Adafruit_NeoPixel::Color(255, 165, 0)         //    ORANGE
#define _YELLOW             Adafruit_NeoPixel::Color(255, 255, 0)         //    YELLOW
#define _MAGENTA            Adafruit_NeoPixel::Color(255, 0, 255)         //    MAGENTA
#define _PURPLE             Adafruit_NeoPixel::Color(128, 0, 128)         //    PURPLE
#define _CYAN               Adafruit_NeoPixel::Color(0, 255, 255)         //    CYAN
#define _PINK               Adafruit_NeoPixel::Color(255, 192, 203)       //    PINK
#define _WHITE              Adafruit_NeoPixel::Color(255, 255, 255)       //    WHITE
#define _BLACK              Adafruit_NeoPixel::Color(0, 0, 0)             //    BLACK

byte H0[4] = {7, 6, 5, 4};              // Hour - Ten digit
byte H1[4] = {3, 2, 1, 0};              // Hour - Unit digit

byte M0[4] = {8, 9, 10, 11};            // Minute - Ten digit
byte M1[4] = {12, 13, 14, 15};          // Minute - Unit digit


const char *WIFI_NETWORK_NAME = "FPTTelecom";     // Change to your wifi network name
const char *WIFI_PASSWORD     = "12345678";       // Change to your wifi password

const char *TIME_SERVER       = "asia.pool.ntp.org";
int myTimeZone = VST;                             // Change this to your time zone (see in timezone.h)

byte H0_Number, H1_Number, M1_Number, M0_Number;

byte m0, m1, h0, h1;
byte prvem0, prvem1,prveh0, prveh1;

unsigned long samplingtimem0 = 0;
unsigned long samplingtimem1 = 0;
unsigned long samplingtimeh0 = 0;
unsigned long samplingtimeh1 = 0;
unsigned long samplingtime = 0;

void GetTime();
void DrawBinary(byte number, byte coordinates[4], uint32_t frontcolor, uint32_t backcolor);

void Effect_M0(uint32_t frontcolor, uint32_t backcolor);
void Effect_M1(uint32_t frontcolor, uint32_t backcolor);
void Effect_H0(uint32_t frontcolor, uint32_t backcolor);
void Effect_H1(uint32_t frontcolor, uint32_t backcolor);

void Effect_Test();

time_t now;
struct tm *timeinfo;;

byte hour;
byte mins;
byte sec;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, STICK, NEO_GRB + NEO_KHZ800);

void setup()
{

  WiFi.begin(WIFI_NETWORK_NAME, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  configTime(3600*myTimeZone, 0, TIME_SERVER);

  while (now < EPOCH_1_1_2019)
  {
    now = time(nullptr);
    delay(500);
  }
  strip.begin();
  strip.setBrightness(50);
  strip.show();
  pinMode(STICK, OUTPUT);
  
  if (WiFi.status() == WL_CONNECTED)
  {
    for(int i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, _RED);         
      strip.show();
      delay(200);                       
    }
  }
  for(int i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, _BLACK);         
      strip.show();
      delay(200);
    }
    
  GetTime();
  DrawBinary(H0_Number, H0, _RED, _BLACK);
  DrawBinary(H1_Number, H1, _GREEN, _BLACK);
  DrawBinary(M0_Number, M0, _BLUE, _BLACK);
  DrawBinary(M1_Number, M1, _YELLOW, _BLACK);
}

void loop() 
{
  GetTime();    
  Effect_H0(_RED, _BLACK);
  Effect_H1(_GREEN, _BLACK);
       
  Effect_M0(_BLUE, _BLACK);
  Effect_M1(_YELLOW, _BLACK);
}
void GetTime()
{
  time(&now);
  timeinfo    = localtime(&now);

  hour        = timeinfo->tm_hour;
  mins        = timeinfo->tm_min;
  sec         = timeinfo->tm_sec;
                 
  H0_Number = ((hour/10) %10);
  H1_Number = (hour%10);
    
  M0_Number = ((mins/10) %10);
  M1_Number = (mins%10);
}
  
void DrawBinary(byte number, byte coordinates[4], uint32_t frontcolor, uint32_t backcolor)
{     
  for (int i = 0; i < 4; i++)
  {
    if (bitRead(number, i))
    {
      strip.setPixelColor(coordinates[i], frontcolor);
      strip.show();
    }
    else
    {
      strip.setPixelColor(coordinates[i], backcolor);
      strip.show();
    }
  }
}

void Effect_H0(uint32_t frontcolor, uint32_t backcolor)
{ 
  if ( (unsigned long) (micros() - samplingtimeh0) > 855  )
  {    
    h0 = H0_Number;    
    if (h0 != prveh0)  
      {   
        DrawBinary(H0_Number, H0, frontcolor, backcolor);
        prveh0 = h0;
      }
      samplingtimeh0 = micros(); 
    }
}

void Effect_H1(uint32_t frontcolor, uint32_t backcolor)
{ 
  if ( (unsigned long) (micros() - samplingtimeh1) > 755  )
  {    
    h1 = H1_Number;    
    if (h1 != prveh1)  
      {   
        DrawBinary(H1_Number, H1, frontcolor, backcolor);
        prveh1 = h1;
      }
      samplingtimeh1 = micros(); 
    }
}

void Effect_M0(uint32_t frontcolor, uint32_t backcolor)
{ 
  if ( (unsigned long) (micros() - samplingtimem0) > 655  )
  {    
    m0 = M0_Number;    
    if (m0 != prvem0)  
      {   
        DrawBinary(M0_Number, M0, frontcolor, backcolor);
        prvem0 = m0;
      }
      samplingtimem0 = micros(); 
    }
}

void Effect_M1(uint32_t frontcolor, uint32_t backcolor)
{ 
  if ( (unsigned long) (micros() - samplingtimem1) > 555  )
  {    
    m1 = M1_Number;    
    if (m1 != prvem1)  
      {   
        DrawBinary(M1_Number, M1, frontcolor, backcolor);
        prvem1 = m1;
      }
      samplingtimem1 = micros(); 
    }
}
void Effect_Test()
{
  if ( (unsigned long) (millis() - samplingtime) > 15000  )
  {    
  
    for(int i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, _RED);         
      strip.show();
      delay(200);
    }
    for(int i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, _GREEN);         
      strip.show();
      delay(200);
    }
    for(int i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, _BLUE);         
      strip.show();
      delay(200);
    }
    for(int i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, _BLACK);         
      strip.show();
      delay(200);
    }
    samplingtime = millis(); 
  }  
}
