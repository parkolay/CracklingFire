//Initial source code, check github for latest rev.

#include <DigiSpark_Adafruit_NeoPixel.h>
#include <DFMiniMp3.h>
/*
 1 "flames" of 3 pixels each.   //was  5 "flames" 
 Each flame can have a brightness of 0 to 254 (play with this scale)
 "Serial" refers to the hardware serial port attached to pins 0 and 1, 
 this is shared with USB serial and causes "noise" 
 it was hoped that removing the serial.print(s) would remove the noise but this was not the case
*/

#define PIN 2              // Which pin are those Neopixels hook up to? 
#define NUMBER_OF_FLAMES 3 // depends on number of neopixel triplets. 5 for 16 NeoPixel ring. 4 for 12 NeoPixel ring was 5
#define FLAME_WIDTH 1      // How wide are the flames (in LEDs)
#define FLICKER_CHANCE 9   // increase this to increase the chances an individual flame will flicker

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
DigiSpark_Adafruit_NeoPixel strip = DigiSpark_Adafruit_NeoPixel(NUMBER_OF_FLAMES * FLAME_WIDTH, PIN, NEO_GRB + NEO_KHZ800);

uint32_t rez_range = 256*3;
#define D_ false

// ***************** Flame:*****************************
struct flame_element
{
  int brightness;
  int step;
  int max_brightness;
  long rgb[3];
  byte state;
  } flames[NUMBER_OF_FLAMES];
  
  int new_brightness = 0;
  unsigned long rgb[3]; //reusable temporary array
  uint8_t scaleD_rgb[3];
  byte acc;
 
 #define SCALERVAL 256*3
 const int flamecolors[22][3] = {
{ SCALERVAL, 0,  0},
{ SCALERVAL, 0,  0},
{ SCALERVAL, 0,  0},
{ SCALERVAL, 0,  0},
{ SCALERVAL, 0,  0},
{ SCALERVAL, 0,  0},
{ SCALERVAL, 0,  0},
{ SCALERVAL, 0,  0},
{ SCALERVAL, SCALERVAL*.4,  },
{ SCALERVAL, SCALERVAL*.4,  0},
{ SCALERVAL, SCALERVAL*.4,  0},
{ SCALERVAL, SCALERVAL*.4,  0},
{ SCALERVAL, SCALERVAL*.3,  0},
{ SCALERVAL, SCALERVAL*.3,  0},
{ SCALERVAL, SCALERVAL*.3,  0},
{ SCALERVAL, SCALERVAL*.3,  0},
{ SCALERVAL, SCALERVAL*.3,  0},
{ SCALERVAL, SCALERVAL*.3,  0},
{ SCALERVAL, SCALERVAL*.3,  },
{ SCALERVAL, SCALERVAL*.3,  SCALERVAL}, // white
{ 0, SCALERVAL*.2,  SCALERVAL}, // that one blue flame
{ SCALERVAL,  SCALERVAL*.3,  SCALERVAL*.5}
};

//***************************** Sounds; ****************************
// implement a notification class,
// its member methods will get called 
//
class Mp3Notify
{
public:
  static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action)
  {
    if (source & DfMp3_PlaySources_Sd) 
    {
        //Serial.print("SD Card, ");
    }
    if (source & DfMp3_PlaySources_Usb) 
    {
        //Serial.print("USB Disk, ");
    }
    if (source & DfMp3_PlaySources_Flash) 
    {
        //Serial.print("Flash, ");
    }
    //Serial.println(action);
  }
  static void OnError(uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
//    Serial.println();
//    Serial.print("Com Error ");
//    Serial.println(errorCode);
  }
  static void OnPlayFinished(DfMp3_PlaySources source, uint16_t track)
  {
//    Serial.print("Play finished for #");
//    Serial.println(track);  
  }
  static void OnPlaySourceOnline(DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "online");
  }
  static void OnPlaySourceInserted(DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "inserted");
  }
  static void OnPlaySourceRemoved(DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "removed");
  }
};

// instance a DFMiniMp3 object, 
// defined with the above notification class and the hardware serial class
//
DFMiniMp3<HardwareSerial, Mp3Notify> mp3(Serial);

// Some arduino boards only have one hardware serial port, so a software serial port is needed instead.
// comment out the above definition and uncomment these lines
//SoftwareSerial secondarySerial(10, 11); // RX, TX
//DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(secondarySerial);

  
void setup() 
{
  SetupFlames();
  SetupSound();
}

void loop() 
{
  UpdateFlames();
  UpdateSound();
} //loop()
