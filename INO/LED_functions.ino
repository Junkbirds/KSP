////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//    LED Section
////////////////////////////////////////////////////////////////////////////////////

void blinkLedInit(){
   digitalWrite(kspLedRed, HIGH);
   delay(1000);
   digitalWrite(kspLedGrn, HIGH);
   delay(1000);
   digitalWrite(lcdLed, HIGH);
   delay(1000);
   digitalWrite(progLed, HIGH);
   delay(1000);
   digitalWrite(kspLedRed, LOW);
   delay(1000);
   digitalWrite(kspLedGrn, LOW);
   delay(1000);
   digitalWrite(lcdLed, LOW);
   delay(1000);
   digitalWrite(progLed, LOW);
   delay(1000);  
}

void ledToggle(int i){
  //buttonLedState[i] = !buttonLedState[i];
  digitalWrite(buttonLed[i], !buttonLedState[i]);
  buttonLedState[i] = !buttonLedState[i];
  if(  buttonLedState[i] == HIGH ){
    //analogWrite(buttonLed[i], buttonLedBrightness);
  }else{
    ledOff(i, 0);
  }   
}

void ledOff(int i, int j){
  digitalWrite(buttonLed[i], LOW);
  if( j > -1 ){
    //  set the Led State to off as well.
    buttonLedState[i] = 0;
  }  
}

void ledTest(){  // OPTIONAL FUNCTION (SETUP)
  //Blinks all the LED's connected as defined in the buttonLed[] Array.
  for( int h = 0; h < 1; h++ ){
    for( int i = buttonStartNum; i< buttonStopNum; i++){
      //  Compare buttonCurrent with buttonLast
      if ( buttonType[i] == 1 || buttonType[i] == 2 ){  //  LED
        ledToggle(i); 
        Serial.print(" LED Toggle ON ---  >" );
        Serial.println(i);
          delay(100);
      }
    }
    delay(500);
    for( int i = (buttonStopNum-1); i > -1; i--){
      //  Compare buttonCurrent with buttonLast
      if ( buttonType[i] == 1 || buttonType[i] == 2 ){  //  LED
        ledToggle(i);
        Serial.print(" LED Toggle Off ---  >" );
        Serial.println(i);
          delay(100);
      }
    } 
  } 
}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//    ledFade() --- UNUSED
////////////////////////////////////////////////////////////////////////////////////
void ledFade(){
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
    // sets the value (range from 0 to 255):
    for( int i = buttonStartNum; i< buttonStopNum; i++){
    //  Compare buttonCurrent with buttonLast
      if ( buttonType[i] == 1 || buttonType[i] == 2 ){  //  LED
        analogWrite(buttonLed[i], fadeValue);
      }
    }
    delay(30);
  }
  // fade out from max to min in increments of 5 points:
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
    // sets the value (range from 0 to 255):
    for( int i = buttonStartNum; i< buttonStopNum; i++){
    //  Compare buttonCurrent with buttonLast
      if ( buttonType[i] == 1 || buttonType[i] == 2 ){  //  LED
        analogWrite(buttonLed[i], fadeValue);
      }
    }
    delay(30);
  }
    for( int i = buttonStartNum; i< buttonStopNum; i++){
    //  Compare buttonCurrent with buttonLast
      if ( buttonType[i] == 1 || buttonType[i] == 2 ){  //  LED
        analogWrite(buttonLed[i], 0);
      }
    } 
}
