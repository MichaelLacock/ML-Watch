//---------------------------------------------
//---------------- ML-Watch OS ----------------
//----------- v0.3.1 - V4 Hardware ------------
//---------------------------------------------
//----- Made by Michael Lacock, 2019-2021 -----
//  *** Use of example code from Adafruit ***
//---------------------------------------------

// Designed to work with Adafruit Glasses Driver Board,
// 128x128 (SSD1327) OLED Display, and 16-neopixel ring.

// I hacked the button so it can control the NeoPixels.
// It is known as "pin 4" in software.  On the hardware,
// looking at the board USB-C side down and button
// on top, solder to the right solder pad for the button.

#define OSversion   "v0.3.1"

//_Face_Colors
#define MinColor   0x00ffff
#define HourColor   0xff0000

int am = 1;

//_LED Prep_
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

//-------Pin 1 for -T and Pin 13 for -F
#define LED_PIN     4
#define LED_COUNT  16
#define BRIGHTNESS 0
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

//_OLED_Prep_
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1327.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 128 

int Brightness = 2;

#define OLED_RESET -1
Adafruit_SSD1327 display(128, 128, &Wire, OLED_RESET, 1000000);

//_BLUETOOTH_Prep_
#include <bluefruit.h>
BLEClientCts  bleCTime;  // gets time from connected device.

int Hour = 12;
int Min = 0;
int sec = 10;
int oldSec = 9;

int UpdateSec = 4;
int oldUpdateSec = 4;
int UpdateMin = 4;
int oldUpdateMin = 4;
int UpdateHour = 4;
int oldUpdateHour = 4;

//_timmers_setup_
unsigned long previousMillis_TIME_KEEPING = 0;
const long interval_NEW_TICK = 1000; 

unsigned long previousMillis_SECOND_HAND = 0;
const long interval_SECOND_HAND = (interval_NEW_TICK / 2);


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

int SECOND_HAND = 1;

//Apps_Initialize
int clock_APP = 1;
int menu_APP = 0;
int flashlight_APP = 0;
int stopwatch_APP = 0;
int settings_APP = 0;

void setup() {
  
  Serial.begin(115200);

  //_OLED_I2C_SetUp
  if ( ! display.begin(0x3D) ) {
     Serial.println("Unable to initialize OLED");
     while (1) yield();
  }

  display.display();
  delay(2000);
  display.clearDisplay();
  
  //_Led_Setup_
  strip.begin();       
  strip.show();            
  strip.setBrightness(20);
  strip.clear();
  colorWipe(strip.Color( 0,   0,   0), 50);
  strip.clear();  
  
  //_boot_screen
  display.setTextSize(1);
//  display.setTextColor(SSD1306_WHITE);
  display.setTextColor(SSD1327_WHITE);
  display.setCursor(34, 50);
  display.println(F("ML-Watch OS"));
  display.setCursor(42, 70);
  display.println(OSversion);
  display.display();  
  
  Serial.print("ON");

  delay(4000);
  display.setTextSize(2);
  display.clearDisplay();
  display.display();
  
  colorWipe(strip.Color(  0,   0,   0), 50);

  //_BLUETOOTH_SETUP_
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

  Bluefruit.begin();
  Bluefruit.setTxPower(4);
  Bluefruit.setName("MLWATCHv4");
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
  
  //_TIME_KEEPING_
  unsigned long currentMillis_TIME_KEEPING = millis();
  
  if (oldSec == sec) {
    sec = (sec + 1);
    // Due to chip lag we need to subtract the delay; a minute is only 48 seconds for the hardware.
    // This is a temporary workaround for keeping time when not in rage of BLE.
    if (sec > 48) {      
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


  //_CLOCK_APP_
  if (clock_APP == 1) {
    strip.clear();
  
    //_Minute
    if (Min == 0 || Min == 1) {
        strip.setPixelColor(LED_0, MinColor);  
    }
    if (Min >= 2 && Min <= 4) {
        strip.setPixelColor(LED_1, MinColor);  
    }
    if (Min >= 5 && Min <= 8) {
        strip.setPixelColor(LED_2, MinColor);       
    }
    if (Min >= 9 && Min <= 14) {
        strip.setPixelColor(LED_3, MinColor);   
    }
    if (Min == 15 || Min == 16) {
        strip.setPixelColor(LED_4, MinColor);      
    }
    if (Min >= 17 && Min <= 19) {
        strip.setPixelColor(LED_5, MinColor);     
    }
    if (Min >= 20 &&  Min <= 25) {
        strip.setPixelColor(LED_6, MinColor);       
    }
    if (Min >= 26 &&  Min <= 29) {
        strip.setPixelColor(LED_7, MinColor);     
    }
    if (Min == 30 || Min == 31) {
        strip.setPixelColor(LED_8, MinColor);      
    }
    if (Min >= 32 && Min <= 34) {
        strip.setPixelColor(LED_9, MinColor);       
    }
    if (Min == 35 || Min == 36) {
        strip.setPixelColor(LED_10, MinColor);       
    }
    if (Min >= 37 && Min <= 44) {
        strip.setPixelColor(LED_11, MinColor);        
    }
    if (Min == 45 || Min == 46) {
        strip.setPixelColor(LED_12, MinColor);      
    }
    if (Min >= 47 && Min <= 54) {
        strip.setPixelColor(LED_13, MinColor);       
    }
    if (Min == 55 || Min == 56) {
        strip.setPixelColor(LED_14, MinColor);        
    }
    if (Min >= 57 && Min <= 59) {
        strip.setPixelColor(LED_15, MinColor);       
    }
  
    //_Hour
    if (Hour == 12 && Min < 45) {
        strip.setPixelColor(LED_0, HourColor);      
    }
    if (Hour == 12 && Min >= 45 || Hour == 1 && Min <= 29) {
        strip.setPixelColor(LED_1, HourColor);   
    }
    if (Hour == 1 && Min >= 30 || Hour == 2 && Min <= 14) {
        strip.setPixelColor(LED_2, HourColor);  
    }
    if (Hour == 2 && Min >= 15) {
        strip.setPixelColor(LED_3, HourColor);  
    }
    if (Hour == 3 && Min < 45) {
        strip.setPixelColor(LED_4, HourColor); 
    }
    if (Hour == 3 && Min >= 45 || Hour == 4 && Min <= 29) {
        strip.setPixelColor(LED_5, HourColor); 
    }
    if (Hour == 4 && Min >= 30 || Hour == 5 && Min <= 14) {
        strip.setPixelColor(LED_6, HourColor); 
    }
    if (Hour == 5 && Min >= 15) {
        strip.setPixelColor(LED_7, HourColor);  
    }
    if (Hour == 6 && Min < 45) {
        strip.setPixelColor(LED_8, HourColor);
    }
    if (Hour == 6 && Min >= 45 || Hour == 7 && Min <= 29) {
        strip.setPixelColor(LED_9, HourColor);  
    }
    if (Hour == 7 && Min >= 30 || Hour == 8 && Min <= 14) {
        strip.setPixelColor(LED_10, HourColor);  
    }
    if (Hour == 8 && Min >= 15) {
        strip.setPixelColor(LED_11, HourColor); 
    }
    if (Hour == 9 && Min < 45) {
        strip.setPixelColor(LED_12, HourColor); 
    }
    if (Hour == 9 && Min >= 45 || Hour == 10 && Min <= 29) {
        strip.setPixelColor(LED_13, HourColor); 
    }
    if (Hour == 10 && Min >= 30 || Hour == 11 && Min <= 14) {
        strip.setPixelColor(LED_14, HourColor);
    }
    if (Hour == 11 && Min >= 15) {
        strip.setPixelColor(LED_15, HourColor);
    }
  
    strip.setBrightness(Brightness);
    strip.show(); 

    if (currentMillis_TIME_KEEPING - previousMillis_SECOND_HAND >= interval_SECOND_HAND) {
      previousMillis_SECOND_HAND = currentMillis_TIME_KEEPING;
      if (SECOND_HAND == 1) {
        SECOND_HAND = 0;
      } else {
        SECOND_HAND = 1;
      }
    }

    if (SECOND_HAND == 1) {
      display.clearDisplay();
      display.setCursor(18, 55);
      display.print(Hour);
      display.print(':');
      if (Min < 10) {
        display.print("0");
      }
      display.print(Min);
      if (am == 1) {
        display.print(" am");
      } 
      else {
        display.print(" pm");
      }
      display.println();
      display.display(); 
    } else {
      display.clearDisplay();
      display.setCursor(18, 55);
      display.print(Hour);
      display.print(' ');
      if (Min < 10) {
        display.print("0");
      }
      display.print(Min);
    
      if (am == 1) {
        display.print(" am");
      } 
      else {
        display.print(" pm");
      }
      display.println();
      display.display();
    }
  }

  //_MENU_APP_TEST_
  if (menu_APP == 1) {
//    strip.clear();
//    strip.show();
    
    display.clearDisplay();
    display.setCursor(18, 55);
    display.print("Menu Test");
    display.display();
  }

  //_TIME_KEEPING_
  if (currentMillis_TIME_KEEPING - previousMillis_TIME_KEEPING >= interval_NEW_TICK) {
    previousMillis_TIME_KEEPING = currentMillis_TIME_KEEPING;
    
    oldSec = sec;
    oldUpdateHour = UpdateHour;
    oldUpdateMin = UpdateMin;
    oldUpdateSec = UpdateSec;
  }
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { 
    strip.setPixelColor(i, color);         
    strip.show();                          
    delay(wait);                           
  }
}

void Test_LED() {
        strip.clear();
        strip.begin();       
        strip.show();            
        strip.clear();

        strip.setPixelColor(LED_0, HourColor);
        strip.setBrightness(2);
        strip.show();
        
}

void Clear_All()  {

  display.clearDisplay();
  display.display();
  colorWipe(strip.Color(  0,   0,   0), 50);

}

//_CODE_FROME_ADAFRUIT_
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
  BLEConnection* conn = Bluefruit.Connection(conn_handle);

  Serial.println("Connected");
  
  Serial.print("Discovering CTS ... ");
  if ( bleCTime.discover(conn_handle) )
  {
    Serial.println("Discovered");
    
    // Current Time Service requires pairing to work
    // request Pairing if not bonded
    Serial.println("Attempting to PAIR with the iOS device, please press PAIR on your phone ... ");
    conn->requestPairing();
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
