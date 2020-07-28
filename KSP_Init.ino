void checkLcd(){
   if( digitalRead(30) == LOW ){
    //LCD ON
    lcdFlag = true;
    i2cFlag = true;
    analogWrite(lcdLed, ledBright);
    delay(100);
    int status;
    status = lcd.begin(LCD_COLS, LCD_ROWS);
    if(status) // non zero status means it was unsuccesful
    {
      // begin() failed
      Serial.print("LCD initalization failed: ");
      Serial.println(status);
      // blink error code using the onboard LED if possible
      hd44780::fatalError(status); // does not return
    }
    lcd.lineWrap();  //  just to prevent line 1 to 3 and 2 to 4 wrapping.  (this wraps 1-2, 2-3, and 3-4)   
   }else{
    lcdFlag = false;
    i2cFlag = false;
   }
}


void lcdSetup(){  // SETUP FUNCTION
  if( lcdFlag == true  ||  kspFlag == true){
    lcd.init();  //initialize the lcd
    lcd.backlight();  //open the backlight
    lcd.begin(20, 4);
    delay(100);
    lcd.setCursor(0,0);
    lcd.print("Junkbirds Test 3.1");
    if( digitalRead(31) == LOW){
      lcd.setCursor(0,1);
      lcd.print("BOO HOO");
      delay(5000);
    }else{
    delay(1000);
    }
    clearLcd();
  }else{
    lcd.init();
    lcd.noBacklight();
    lcd.noDisplay();
    delay(1000);
  }
}

void kspSetup(){
  if(kspFlag == true){
    Serial.begin(115200);
    delay(100);
    while (!mySimpit.init()){
      delay(100);
    }
    digitalWrite(kspLedRed, LOW);
    analogWrite(kspLedGrn, ledBright);
  } 
            //....5....0....5....0
  //lcdLine1 = emptyLine;
  lcdLine2 = "   Junkbirds  KSP   ";
  lcdLine3 = "    junkbirdfood    ";
  lcdLine4 = "     @gmail.com     ";
  lcdHasData  =  true;
}

void checkReset(){
  int a = -1;
  int b = -1;
  int c = -1;
  a = digitalRead(51);
  b = digitalRead(27);
  c = digitalRead(28);  
  if( a == 0 && b == 0 && c == 0 ){
    resetFunc();
  }
}

void checkDebug(){
   if( digitalRead(29)== LOW ){  // DEBUG SWITCH
    bugFlag = true;
    analogWrite(kspLedGrn, ledBright);
    lcdRefresh = lcdRefreshDebug;  //  UPDATES LCD REFRESH FOR DEBUGGING!
    kspFlag = false;
   }else{
    bugFlag = false;
    kspFlag = true;
   }
}

void i2cSetup(){
  Wire.begin(); // join i2c bus (address optional for master)
  //Wire.onReceive(receiveEvent);
}

void setupIo(){
   pinMode(kspLedRed,  OUTPUT);
   pinMode(kspLedGrn, OUTPUT);
   pinMode(lcdLed, OUTPUT);
   pinMode(progLed, OUTPUT);
   pinMode(29, INPUT_PULLUP);
   pinMode(30, INPUT_PULLUP);
   pinMode(31, INPUT_PULLUP);
   analogWrite(kspLedRed, ledBright);
   digitalWrite(kspLedGrn, HIGH);
   digitalWrite(lcdLed, HIGH);
   digitalWrite(progLed, HIGH);
   delay(250);
   digitalWrite(kspLedGrn, LOW);
   digitalWrite(lcdLed, LOW);
   digitalWrite(progLed, LOW);

   clearKspResourceArray();
}

void checkProg(){
  //Future function
  if( digitalRead(31) == LOW ){
    //digitalWrite(progLed, HIGH);
    analogWrite(progLed, ledBright);
  }
}
void checkI2c(){
    if( i2cFlag == true || lcdFlag == true ){  // USE I2C comm for LCD OR Slave
    i2cSetup(); 
  }
}

void joystickStartup(){
Serial.println("-->   Kerbal Custom Controller - junkbirdfood@gmail.com ");
delay(1000);
Serial.println("-->   Waiting for input  --  Go ahead!  Press a button!");
}

void PinSetup(){  //  REQUIRED FUNCTION (SETUP)
  // set pinMode  
  for( int i = buttonStartNum; i< buttonStopNum; i++){
    if( buttonType[i] > 2 ){  //  its an analog pin - INPUT?
      pinMode(button[i], INPUT);     
    }else{
      pinMode(button[i],INPUT_PULLUP); 
      if( buttonType[i] == 1 ){  // SWITCH WITH AN LED
        pinMode(buttonLed[i], OUTPUT);
        digitalWrite(buttonLed[i], LOW);
      }
    }
  }  
}

void setStickMap(byte S){  // REQUIRED FUNCTION  (SETUP)

//  sensorMin,sensorMax,maxNeg,maxPos
//  paired sets for reversing axis values
//
//  when changing sensitivity make sure to
//  call setStickMap(sss) to enable the change.
//
//  to reverse an axis during use, change th
//  associated buttonMap[#] value for that axis
//  For an example, use find and search for
//  void menuLcd(){
  /*
int stickMap[5][4] { 
  552, 1023,     0,  1000,
  552, 1023,     0, -1000,
    0,  472, -1000,     0,
    0,  472,  1000,     0
 };
*/
  stickMap[  0  ][  0  ] = centerPos + deadZone;
  stickMap[  0  ][  1  ] = sensorMax;
  stickMap[  0  ][  2  ] = outputCenter;
  stickMap[  0  ][  3  ] = maxPos[S];

  stickMap[  1  ][  0  ] = centerPos + deadZone;
  stickMap[  1  ][  1  ] = sensorMax;
  stickMap[  1  ][  2  ] = outputCenter;
  stickMap[  1  ][  3  ] = maxNeg[S];

  stickMap[  2  ][  0  ] = sensorMin;
  stickMap[  2  ][  1  ] = centerPos - deadZone;
  stickMap[  2  ][  2  ] = maxNeg[S];
  stickMap[  2  ][  3  ] = outputCenter;

  stickMap[  3  ][  0  ] = sensorMin;
  stickMap[  3  ][  1  ] = centerPos - deadZone;
  stickMap[  3  ][  2  ] = maxPos[S];
  stickMap[  3  ][  3  ] = outputCenter;

///  Throttle Pot Var's

  stickMap[  4  ][  0  ] = tPotMin;
  stickMap[  4  ][  1  ] = tPotMax;
  stickMap[  4  ][  2  ] = tMax;
  stickMap[  4  ][  3  ] = tMin;
}

void clearKspResourceArray(){
//byte i = kspInboundCount;

  for ( int i = 0; i < kspInboundCount; i++){
    KRA[i][0] = 0.0;
    KRA[i][1] = 0.0;
  }
}
