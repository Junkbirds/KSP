#include <KerbalSimpitMessageTypes.h>
#include <PayloadStructs.h>
#include <KerbalSimpit.h>
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

#define bc 33  //  Button Count <----  Adjust as necessary

//  Status LED that are NOT associated with buttons
const int kspLedRed = 10;
const int kspLedGrn = 9;
const int lcdLed    = 11;
const int progLed   = 12;

const byte ledBright = 3;

//  LCD info
hd44780_I2Cexp lcd;
const int LCD_COLS = 20;
const int LCD_ROWS = 4;
int lcdRefresh = 500;       //  To control Display update frequency during KSP sessions
int lcdRefreshDebug = 100;  //  To control Display update frequency during DEBUG
bool lcdHasData = false;    // If TRUE there IS data to show on the LCD
bool lcdForce   = false;    // for Menu changes, etc that are not auto updating.

//  String var's for LCD Display
//  ToDo:  Rewrite as char array
String lcdLine1 = "";
String lcdLine2 = "";
String lcdLine3 = "";
String lcdLine4 = "";
                  //....5....0....5....0
String emptyLine = "                    ";

/////  

bool bugFlag = false;  //  DEBUG FLAG.  True uses Serial. false does not.
bool lcdFlag = false;  //  DEBUG FLAG.  True uses LCD as output.  false does not.
bool i2cFlag =  true;  //  i2c flag.  use True for LCD or Slave connections.
bool lcdKsp  = false;  //  WILL BE SET IN LOGIC TO BE OPPOSITE OF lcdFlag!
bool kspFlag = false;  //  Connect to KSP

//  Button/Pin ID's
//  LED pins are -20 from corresponding button

////////                       |<-LED BUTTONS GROUP 1->|<-------POT GROUP (STICK+THROTTLE)------>|<---BUTTON GROUP 1 --->|<---BUTTON GROUP 2 --->|<---BUTTON GROUP 3 --->|<--JB grp->|<-ctr grp->|<- LED GROUP 2- >|<-Ctrl Panel Grp>|<-- Ctrl Panel LED's ->|                                                                                                                                                                                                  
///////  Counter Line /////////--0-----1-----2-----3-----4-----5-----6-----7-----8-----9-----10----11----12----13----14----15----16----17----18----19----20----21----22----23----24----25----26----27----28----29----30----31----32----33----34----35----36----37----38----39----40  }}                                             /////

int button[ bc ]               { 22   ,23   ,24   ,25   ,A0   ,A1   ,A2   ,A3   ,A4   ,A5   ,A6   ,34   ,36   ,38   ,40   ,35   ,37   ,39   ,41   ,42   ,44   ,46   ,48   ,52   ,53   ,50   ,51   ,26   ,27   ,28   ,29   ,30   ,31    };  ///  Button PIN
int buttonLed[ bc ]            { 2    ,3    ,4    ,5    ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,6    ,7    ,8    ,-1   ,-1   ,-1    };  ///  Led PIN
int buttonLedState[ bc ]       { 0    ,0    ,0    ,0    ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,0    ,0    ,0    ,-1   ,-1   ,-1    };  ///  Led State
int buttonLedBlink[ bc ]       { 0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0     };  ///  Led Blink Delay
int buttonType[ bc ]           { 1    ,1    ,1    ,1    ,3    ,3    ,3    ,3    ,3    ,3    ,4    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,2    ,2    ,2    ,0    ,0    ,0     };  ///  0 for NO LED --->  1 for LED  ---> 2 for sticky LED ---> 3 for Joystick Pot ---> 4 for Slide Pot
int buttonToggleType[ bc ]     { 2    ,2    ,2    ,2    ,3    ,3    ,3    ,3    ,3    ,3    ,4    ,1    ,1    ,1    ,1    ,0    ,0    ,0    ,0    ,0    ,1    ,0    ,1    ,1    ,1    ,0    ,0    ,2    ,2    ,2    ,2    ,2    ,2     };  ///  0 for momentary, 1 for momentary (sticky), 2 for sticky, 3+ for Pot
int buttonMap[ bc ]            { 0    ,0    ,0    ,0    ,1    ,1    ,0    ,1    ,0    ,0    ,4    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0     };  ///  Stick Axis Mapping
int16_t buttonLast[ bc ]       { 1    ,1    ,1    ,1    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1     };  ///  Starting Value (1 is OFF  --  0 is ON)
int16_t buttonCurrent[ bc ]    { 1    ,1    ,1    ,1    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1     };  ///  Starting Value (1 is OFF  --  0 is ON)
int16_t buttonState[ bc ]      { 1    ,1    ,1    ,1    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1    ,1     };
byte buttonLedBrightness[ bc ] { 255  ,255  ,255  ,255  ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,-1   ,255  ,255  ,255  ,-1   ,-1   ,-1    };
unsigned long ledBlinkCurrent[bc];
unsigned long ledBlinkPrevious[bc];
//  MUST SET THESE COUNTS to use only the required # of buttons and proper start/stop #'s.  ///////////////////////////////////////////////////////////////////
byte buttonStartNum = 0;
byte buttonStopNum  = bc;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//Joystick/Throttle Pots
// See setStickMap() function  -- KSP_Init
int stickMap[5][4];  // See setStickMap() function
//  Joystick var's  //  WILL IMPACT STICKMAP!!! 
int centerPos = 512;
//>>  TODO:   Allow Deadzone to be set/pulled from EEprom - ADD adjustable deadzones for Each vector of each axis!
int deadZone = 50;  // 50 both sides of center
int limitPos = centerPos + deadZone;
int limitNeg = centerPos - deadZone;
int outputCenter = 0;
int sensorMax = 1023;
int sensorMin = 0;
byte sss = 0;  //stickSensitivityState
byte sssCount = 4;  //count of sensitivity levels
int maxPos[4] = { 32767,  16384,  8192,  4096 };
int maxNeg[4] = {-23768, -11884, -5942, -2971 };
///  Throttle Pot Var's
int tPotMin = 15;  //  Input Minimum (set to (0+5) so that jitter is eliminated)
int tPotMax = 1010;  // Input Maximum (set to (1023-5) so that jitter is eliminated)
int tMax = 32767;  //  Output Max
int tMin = 0;  //  Output Min

bool flyingVehicle = false;

byte controlMode = 0;  //  rocket, plane, rover, EVA

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Enumerations

char stateName[2][4]    {"On","Off"};  //  ON = 0, OFF = 1

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Timing Vars

unsigned long lcdPrevious = 0;
unsigned long lcdCurrent  = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Prototypes

void debugMsg( int i, int j, char debugText[] = "" );
void(* resetFunc) (void) = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

///  Kerbal Simpit

KerbalSimpit mySimpit(Serial);
rotationMessage myRotation;
translationMessage myTranslation;
wheelMessage myWheel;
int16_t throttleValue = 0;
const byte kspInboundCount = 18;

const byte LF = 0;
const byte LFS = 1;
const byte OX = 2;
const byte OXS = 3;
const byte SF = 4;
const byte SFS = 5;
const byte MONO = 6;
const byte ELEC = 7;
const byte EVA = 8;
const byte ORE = 9;
const byte AB = 10;
const byte ABS = 11;
const byte ALT = 12;
const byte APS = 13;
const byte VEL = 14;
const byte ACT = 15;
const byte TAR = 16;
const byte AIR = 17;

float KRA[ kspInboundCount ][3];
//  Inbound data that is NOT a float
int32_t periapsis = 0;  //Peri TIME
int32_t apoapsis  = 0;  //Apo TIME
String soiText = "12345678901234567890";
byte lcdDisplayGroupId = 0;
byte lcdDisplayGroupIdPrevious = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  setupIo();  //clear
  delay(100);
  checkDebug();
  checkLcd();
  checkProg();
  checkI2c(); 
  if( lcdFlag == true ){ lcdKsp = false; }
  PinSetup();
  delay(100);
  setStickMap(0);
  delay(100);
  ledTest();
  delay(100);
  debugSetup();
  delay(100);
  lcdSetup();
  delay(500);
  kspSetup();
  delay(100);
  soiText = "---";
  mySimpit.inboundHandler(mcbh);
  mySimpit.registerChannel(3);  // Scene change message
}

void loop() {
  if( kspFlag == true ){
    mySimpit.update();
  }
  scanButtons();
  buttonChange();
  timing();  //  FOR LCD/LED's
  ledBlink();
}

void scanButtons(){  //  REQUIRED FUNCTION  (MAIN LOOP)
  for( int i = buttonStartNum; i< buttonStopNum; i++){ 
    if( buttonType[i] > 2 ){           
      potRead(i);     
    }else{                                 
      buttonCurrent[i] =  digitalRead(button[i]);
    }
  }  
}

void buttonChange(){  //  REQUIRED FUNCTION  (MAIN LOOP)
  for( int i = buttonStartNum; i< buttonStopNum; i++){
     if ( buttonCurrent[i] !=  buttonLast[i] ){  // Button Changed    
       if(  buttonToggleType[i] == 1 ){  //  Digital Sticky     
         if( buttonCurrent[i] == 1 && buttonLast[i] == 0 ){  // Pressed and released
           buttonState[i] = !buttonState[i];     //  Assign to buttonState[ i ]
         } 
       }else{
         buttonState[i] = buttonCurrent[i];
       }
         buttonLast[i] = buttonCurrent[i];
         if( bugFlag == true){  //  DEBUG LOGIC
           buttonPressed(i);
           if( lcdFlag == true ){
             if(buttonType[i] > 2){  //  POT
               if(i == 4 || i == 5 || i == 6 ){ lcdStickTest(  4  ); }
               if(i == 7 || i == 8 || i == 9 ){ lcdStickTest(  7  ); }
               if(i == 10){                     lcdStickTest( 10  ); }
               debugMsg(i, -1);
             }else{
               debugMsg(i,-1);
               lcdDisplay(i);
             }
           }else{
             debugMsg(i,-1); 
           }
         }else{
           buttonPressedKsp(i);
         }
       if( buttonType[i] == 1  || buttonType[i] == 2 ){  ledToggle(i); }         // Toggle the state and the LED
    }
  }
}

void timing(){  //  OPTIONAL FUNCTION  (MAIN LOOP)  (FOR LCD & LED)
  for( int i = buttonStartNum; i< buttonStopNum; i++){
    if ( buttonLedBlink[i] >  0 ){
       ledBlinkCurrent[i] = millis();
    }
  }
  
  lcdCurrent = millis();
  if( lcdCheck() == true ){
    if( lcdHasData == true ){
      showDisplay( -1 );
      lcdHasData = false;
    }
    
    if( lcdDisplayGroupId != 0 ){
      showDisplay( lcdDisplayGroupId );
    }
  }
}

void ledBlink(){
    for( int i = buttonStartNum; i< buttonStopNum; i++){
      if( buttonLedBlink[i] > 0){
        if ( ledBlinkCurrent[i] > (ledBlinkPrevious[i] + buttonLedBlink[i] )){  //  Greater than interval
         ledBlinkPrevious[i] = ledBlinkCurrent[i];
         ledToggle(i);
        }
      }
    }
}
