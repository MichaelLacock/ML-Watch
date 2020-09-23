//---------------------------------------------
//---------------- ML-Watch OS ----------------
//----------------- v0.3.0 -C -----------------
//---------------------------------------------
//----- Made by Michael Lacock, 2019/2020 -----
//---------------------------------------------

//_______CHANGE_LOG_______
//--Added support for Adafruit Clue development board.

//_____KNOWN_PROBLEMS______
//--Buttons have no function yet.

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Arcada.h> // For adafruit clue display.
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

#define OSversion   "v0.3.0 -C"

int tick = 462; // 0.462 seconds (counter-act Hardware related delays)

//_Neopixel_Face_Colors - UNCOMMENT TO ENABLE
//#define MinColor   0x00ffff
//#define HourColor   0xff0000
//
//int am = 1;

//_FOR_NEOPIXEL_RING - UNCOMMENT TO ENABLE
//#define LED_PIN     13 //(RESET THIS NUMBER IF NEEDED)
//#define LED_COUNT  16
//#define BRIGHTNESS 0
//Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

//int Brightness = 5;

//_ADAFRUIT_CLUE_DISPLAY_setup_
Adafruit_Arcada arcada;

//_OLED_Prep_ (FOR I2C OLED DISPLAY)
//#define SCREEN_WIDTH 128 
//#define SCREEN_HEIGHT 64 

//#define OLED_RESET     1 
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//_RTC_Prep_
//#include "RTClib.h"
//RTC_PCF8523 rtc;
//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//_BLUETOOTH_Prep_
#include <bluefruit.h>
BLEClientCts  bleCTime;  // Gets time from connected device.

//_initializes_integer_variables
int Hour = 12;
int Min = 0;
int sec = 10;
int oldSec = 9;
int am = 1;

int UpdateSec = 4;
int oldUpdateSec = 4;
int UpdateMin = 4;
int oldUpdateMin = 4;
int UpdateHour = 4;
int oldUpdateHour = 4;

//_LED_Var_
int Active_Face = 0;
int LED_0 = 2;
int LED_1 = 1;
int LED_2 = 0;
int LED_3 = 15;
int LED_4 = 14;
int LED_5 = 13;
int LED_6 = 12;
int LED_7 = 11;
int LED_8 = 10;
int LED_9 = 9;
int LED_10 = 8;
int LED_11 = 7;
int LED_12 = 6;
int LED_13 = 5;
int LED_14 = 4;
int LED_15 = 3;
int Hour_location_onface = 0;
int Min_location_onface = 0;


void setup() {
  
  Serial.begin(115200);

  //_OLED_I2C_SetUp
  //if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
  //Serial.println(F("SSD1306 allocation failed"));
  //for(;;);
  //}

  //_I2C_OLED_DISPLAY_SETUP_
  //display.display();
  //delay(2000);
  //display.clearDisplay();

  //_Adafruit_Clue_Display_setup
  arcada.displayBegin();

  for (int i=0; i<250; i+=10) {
    arcada.setBacklight(i);
    delay(1);
  }
  
  arcada.display->setTextWrap(true);

  //_RTC_I2C_SetUp
  //if (! rtc.begin()) {
  //  Serial.println("Couldn't find RTC");
  //  while (1);
  //}

  //if (! rtc.initialized()) {
    //Serial.println("RTC is NOT running!");
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //rtc.adjust(DateTime(2019, 11, 22, 19, 51, 0));
  //}

  //rtc.adjust(DateTime(2019, 11, 22, 19, 52, 10));
  
  //_Led_Setup_
  //strip.begin();       
  //strip.show();            
  //strip.setBrightness(20);
  //strip.clear();
  //colorWipe(strip.Color( 0,   0,   0), 50);
  //strip.clear();  

  Serial.print("ON");
  
  //_boot_screen_I2C_OLED_DISPLAY_
//  display.setTextSize(1);
//  display.setTextColor(SSD1306_WHITE);
//  display.setCursor(34, 18);
//  display.println(F("ML-Watch OS"));
//  display.setCursor(42, 40);
//  display.println(OSversion);
//  display.display();  

//  delay(4000);
//  display.setTextSize(2);
//  display.clearDisplay();
//  display.display();


  //_boot_screen_ADAFRUIT_CLUE_DISPLAY_
  arcada.display->setTextSize(3);
  arcada.display->setTextColor(ARCADA_WHITE);
  arcada.display->setCursor(20, 90);
  arcada.display->println("ML-Watch OS");
  arcada.display->setCursor(35, 120);
  arcada.display->println(OSversion);

  delay(4000);
  
  arcada.display->fillScreen(0x0000);

//  colorWipe(strip.Color(  0,   0,   0), 50);

  //_BLUETOOTH_SETUP_
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

  Bluefruit.begin();
  Bluefruit.setTxPower(4);
  Bluefruit.setName("MLWATCHclue");
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  bleCTime.begin();
  bleCTime.setAdjustCallback(cts_adjust_callback);

  startAdv();
}

void loop() {

  bleCTime.getCurrentTime();
  UpdateHour = bleCTime.Time.hour;
  UpdateMin = bleCTime.Time.minute;
  UpdateSec = bleCTime.Time.second;

  if (oldSec == sec) {
    sec = (sec + 1);
    if (sec == 60) {
      sec = 0;
      Min = (Min + 1);
      if (Min == 60) {
        Min = 0;
        Hour = (Hour + 1);
        if (Hour == 12) {
          if (am == 1) {
            am = 0;
          }
          else {
            am = 1;
          }
        }
        if (Hour == 13) {
          Hour = 1;
        }
      }
    }
  }

  if (UpdateHour != oldUpdateHour) {
    Hour = bleCTime.Time.hour;
    if (Hour > 12) {
      Hour = Hour - 12;
      am = 0;
    } 
    else {
      am = 1;
    }
    if (Hour == 12) {
      am = 0;
    } 
    if (Hour == 0) {
      Hour = 12;
      am = 1;
    }
  }

  if (UpdateMin != oldUpdateMin) {
    Min = bleCTime.Time.minute;
  }

  if (UpdateSec != oldUpdateSec) {
    sec = bleCTime.Time.second;
  }

  //Serial.println(sec);
  //Serial.println(oldSec);

//  Serial.println();
//  Serial.print(Hour);
//  Serial.print(":");
//  Serial.print(Min);
//  Serial.print(":");
//  Serial.print(sec);

  //_16_NEOPIXEL_RING_CODE

  //strip.clear();
  
  //_Minute
//  if (Min == 0 || Min == 1) {
//      strip.setPixelColor(LED_0, MinColor);  
//  }
//  if (Min >= 2 && Min <= 4) {
//      strip.setPixelColor(LED_1, MinColor);  
//  }
//  if (Min >= 5 && Min <= 8) {
//      strip.setPixelColor(LED_2, MinColor);       
//  }
//  if (Min >= 9 && Min <= 14) {
//      strip.setPixelColor(LED_3, MinColor);   
//  }
//  if (Min == 15 || Min == 16) {
//      strip.setPixelColor(LED_4, MinColor);      
//  }
//  if (Min >= 17 && Min <= 19) {
//      strip.setPixelColor(LED_5, MinColor);     
//  }
//  if (Min >= 20 &&  Min <= 25) {
//      strip.setPixelColor(LED_6, MinColor);       
//  }
//  if (Min >= 26 &&  Min <= 29) {
//      strip.setPixelColor(LED_7, MinColor);     
//  }
//  if (Min == 30 || Min == 31) {
//      strip.setPixelColor(LED_8, MinColor);      
//  }
//  if (Min >= 32 && Min <= 34) {
//      strip.setPixelColor(LED_9, MinColor);       
//  }
//  if (Min == 35 || Min == 36) {
//      strip.setPixelColor(LED_10, MinColor);       
//  }
//  if (Min >= 37 && Min <= 44) {
//      strip.setPixelColor(LED_11, MinColor);        
//  }
//  if (Min == 45 || Min == 46) {
//      strip.setPixelColor(LED_12, MinColor);      
//  }
//  if (Min >= 47 && Min <= 54) {
//      strip.setPixelColor(LED_13, MinColor);       
//  }
//  if (Min == 55 || Min == 56) {
//      strip.setPixelColor(LED_14, MinColor);        
//  }
//  if (Min >= 57 && Min <= 59) {
//      strip.setPixelColor(LED_15, MinColor);       
//  }
//
//  //_Hour
//  if (Hour == 12 && Min < 45) {
//      strip.setPixelColor(LED_0, HourColor);      
//  }
//  if (Hour == 12 && Min >= 45 || Hour == 1 && Min <= 29) {
//      strip.setPixelColor(LED_1, HourColor);   
//  }
//  if (Hour == 1 && Min >= 30 || Hour == 2 && Min <= 14) {
//      strip.setPixelColor(LED_2, HourColor);  
//  }
//  if (Hour == 2 && Min >= 15) {
//      strip.setPixelColor(LED_3, HourColor);  
//  }
//  if (Hour == 3 && Min < 45) {
//      strip.setPixelColor(LED_4, HourColor); 
//  }
//  if (Hour == 3 && Min >= 45 || Hour == 4 && Min <= 29) {
//      strip.setPixelColor(LED_5, HourColor); 
//  }
//  if (Hour == 4 && Min >= 30 || Hour == 5 && Min <= 14) {
//      strip.setPixelColor(LED_6, HourColor); 
//  }
//  if (Hour == 5 && Min >= 15) {
//      strip.setPixelColor(LED_7, HourColor);  
//  }
//  if (Hour == 6 && Min < 45) {
//      strip.setPixelColor(LED_8, HourColor);
//  }
//  if (Hour == 6 && Min >= 45 || Hour == 7 && Min <= 29) {
//      strip.setPixelColor(LED_9, HourColor);  
//  }
//  if (Hour == 7 && Min >= 30 || Hour == 8 && Min <= 14) {
//      strip.setPixelColor(LED_10, HourColor);  
//  }
//  if (Hour == 8 && Min >= 15) {
//      strip.setPixelColor(LED_11, HourColor); 
//  }
//  if (Hour == 9 && Min < 45) {
//      strip.setPixelColor(LED_12, HourColor); 
//  }
//  if (Hour == 9 && Min >= 45 || Hour == 10 && Min <= 29) {
//      strip.setPixelColor(LED_13, HourColor); 
//  }
//  if (Hour == 10 && Min >= 30 || Hour == 11 && Min <= 14) {
//      strip.setPixelColor(LED_14, HourColor);
//  }
//  if (Hour == 11 && Min >= 15) {
//      strip.setPixelColor(LED_15, HourColor);
//  }
//
//        
//  strip.setBrightness(Brightness);
//  strip.show(); 

  //_ADAFRUIT_CLUE_DISPLAY_CODE_
  arcada.display->fillScreen(0x0000);
  arcada.display->setTextSize(4);
  arcada.display->setTextColor(ARCADA_WHITE);
  arcada.display->setCursor(35, 105);
  arcada.display->print(Hour);
  arcada.display->print(":");

  if (Min < 10) {
    arcada.display->print("0");
  }
  arcada.display->print(Min);

  if (am == 1) {
    arcada.display->print(" am");
  } 
  else {
    arcada.display->print(" pm");
  }

  delay(tick);

  arcada.display->fillScreen(0x0000);
  arcada.display->setTextSize(4);
  arcada.display->setTextColor(ARCADA_WHITE);
  arcada.display->setCursor(35, 105);
  arcada.display->print(Hour);
  arcada.display->print(" ");

  if (Min < 10) {
    arcada.display->print("0");
  }
  arcada.display->print(Min);

  if (am == 1) {
    arcada.display->print(" am");
  } 
  else {
    arcada.display->print(" pm");
  }

  delay(tick);

  //_I2C_OLED_DISPLAY_CODE_
//  display.clearDisplay();
//  display.setCursor(23, 35);
//  display.print(Hour);
//  display.print(':');
//  
//  if (Min < 10) {
//    display.print("0");
//  }
//  display.print(Min);
//
//  if (am == 1) {
//    display.print(" am");
//  } 
//  else {
//    display.print(" pm");
//  }
//  display.println();
//  display.display(); 
//
//  delay(tick);
//  
//  display.clearDisplay();
//  display.setCursor(23, 35);
//  display.print(Hour);
//  display.print(' ');
//  if (Min < 10) {
//    display.print("0");
//  }
//  display.print(Min);
//
//  if (am == 1) {
//    display.print(" am");
//  } 
//  else {
//    display.print(" pm");
//  }
//  display.println();
//  display.display(); 
//
//  delay(tick);

  oldSec = sec;
  
  oldUpdateHour = UpdateHour;
  oldUpdateMin = UpdateMin;
  oldUpdateSec = UpdateSec;

}

//_STUFF_FOR_NEOPIXEL_RING_

//void colorWipe(uint32_t color, int wait) {
//  for(int i=0; i<strip.numPixels(); i++) { 
//    strip.setPixelColor(i, color);         
//    strip.show();                          
//    delay(wait);                           
//  }
//}
//
//void Clear_All()  {
//  display.clearDisplay();
//  display.display();
//  colorWipe(strip.Color(  0,   0,   0), 50);
//
//}

//_BLUEFRUIT_CODE_FROME_ADAFRUIT_
void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_GENERIC_CLOCK);

  // Include CTS client UUID
  Bluefruit.Advertising.addService(bleCTime);

  // Includes name
  Bluefruit.Advertising.addName();
  
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}

void connect_callback(uint16_t conn_handle)
{
  Serial.println("Connected");
  
  Serial.print("Discovering CTS ... ");
  if ( bleCTime.discover(conn_handle) )
  {
    Serial.println("Discovered");
    
    // iOS requires pairing to work, it makes sense to request security here as well
    Serial.print("Attempting to PAIR with the iOS device, please press PAIR on your phone ... ");
    if ( Bluefruit.requestPairing(conn_handle) )
    {
      //Disconnect = 0;
      Serial.println("Done");
      Serial.println("Enabling Time Adjust Notify");
      bleCTime.enableAdjust();

      Serial.print("Get Current Time chars value");
      bleCTime.getCurrentTime();

      Serial.print("Get Local Time Info chars value");
      bleCTime.getLocalTimeInfo();

      Serial.println();
    }

    Serial.println();
  }
}

void cts_adjust_callback(uint8_t reason)
{
  const char * reason_str[] = { "Manual", "External Reference", "Change of Time Zone", "Change of DST" };

  Serial.println("iOS Device time changed due to ");
  Serial.println( reason_str[reason] );
}

void printTime(void)
{
  const char * day_of_week_str[] = { "n/a", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
  
  Serial.printf("%04d-%02d-%02d ", bleCTime.Time.year, bleCTime.Time.month, bleCTime.Time.day);
  Serial.printf("%02d:%02d:%02d ", bleCTime.Time.hour, bleCTime.Time.minute, bleCTime.Time.second);
  Serial.print(day_of_week_str[bleCTime.Time.weekday]);
  
  int utc_offset =  bleCTime.LocalInfo.timezone*15; // in 15 minutes unit
  Serial.printf(" (UTC %+d:%02d, ", utc_offset/60, utc_offset%60);
  Serial.printf("DST %+.1f)", ((float) bleCTime.LocalInfo.dst_offset*15)/60 );
  Serial.println();
}

void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) reason;

  //Disconnect = 1;

  Serial.println();
  Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
}
