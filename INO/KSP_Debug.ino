/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  DEBUG FUNCTIONS

void debugSetup(){  //  OPTIONAL SETUP FUNCTION
  if( bugFlag == true ){
  Serial.begin(115200);
  delay(250);
  Serial.println(".");
  Serial.println(".");
  Serial.println(" Joystick Button Debug ");
  printStickMap();
  delay(250);
  }  
}

void printStickMap(){  //  DEBUG FUNCTION
  for( int x = 0; x < 8; x++ ){
    for( int z = 0; z < 4; z++ ){
      Serial.print(stickMap[x][z]);
      Serial.print(", ");
    }
    Serial.println(" "); 
  }
}

void debugMsg( int i, int j, char debugText[] = "" ){  //DEBUG FUNCTION
  if( j == -1 ){
    Serial.print(" IO # ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(buttonState[i]);
  }
  if( j == 0 ){
    Serial.print(" IO # ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(buttonLast[i]);    
  }
}

void lcdDisplay( int b ){  //  DEBUG FUNCTION - called from buttonChange()
  //  b is i (index into button arrays)
  lcdLine1 = "";
  lcdLine1 = buildSpaces( "Button ", String(b),"=",String(buttonState[b]),20);
  if( buttonType[b] == 2 ){ //  Pot
    lcdLine1 = buildSpaces( "Button ", String(b)," = ",String( buttonState[b] ),20);
  }else{
    lcdLine1 = buildSpaces( "Button ", String(b)," = ",String( stateName[buttonState[b]] ),20);
  }
  lcdLine2 = emptyLine;
  lcdLine3 = emptyLine;
  lcdLine4 = emptyLine;
  lcdHasData = true;
}

void lcdStickTest( int startingPin ){  //  DEBUG FUNCTION - called from buttonChange()
// test 3 consecutive ARRAY values (assuming x, y, z)
    if( startingPin != 10 ){
      lcdLine1 = buildSpaces( " X = ", String(buttonState[startingPin]  ), "", "", 20);
      lcdLine2 = buildSpaces( " Y = ", String(buttonState[startingPin+1]), "", "", 20);
      lcdLine3 = buildSpaces( " Z = ", String(buttonState[startingPin+2]), "", "", 20);
      if( startingPin == 4 ){
        lcdLine4 = buildSpaces( " Stick = ", "L" , "", "", 20);
      }else{
        lcdLine4 = buildSpaces( " Stick = ", "R" , "", "", 20);
      }      
    }else{      
      lcdLine1 = buildSpaces( " T = ", String(buttonState[10]), "", "", 20);                    
      lcdLine2 = emptyLine;
      lcdLine3 = emptyLine;
      lcdLine4 = emptyLine; 
    }
    lcdHasData = true;
}

  // In Debug mode - button presses are assigned here

void buttonPressed(int i){    //DEBUG FUNCTION
  //  check for toggle type if presses spam   
  if( i == 27 ){
   if( buttonState[i] == 0 ){
     //buttonLedBlink[3] = 250;
     analogWrite(buttonLed[3], ledBright);
   }else{
     //buttonLedBlink[3] = 0;
     //buttonLedState[3] = 0;
     ledOff(3, -1);
   }
   if( bugFlag == true ){ 
     Serial.print("ButtonLedBlink[3] = ");
     Serial.println(buttonLedBlink[3]);
     Serial.print("ButtonState[0] = ");
     Serial.println(buttonState[0]);
   }
  }
  if( i == 11 ){
   if( buttonState[i] == 0 ){
    //lcd.blink();    
   }else{
    //lcd.noBlink();
   }
  }
  if( i == 12 ){
   if( buttonState[i] == 0 ){
    //lcd.backlight();    
   }else{
    //lcd.noBacklight();
   }
  }
  if( i == 13 ){
   if( buttonState[i] == 0 ){
    //lcd.display();    
   }else{
    //lcd.noDisplay();
   }
  }
  if( i == 23 ) {
    // start the menu!
   if( buttonState[i] == LOW ){
    sss = ( sss + 1 ) % sssCount;
    menuLcd();
    } 
  }
  if( i == 25 ) {  //  Display current joystick state
    if( buttonState[i] == LOW ){
    menuLcd();
    }
  }       
  if( i == 26){
    if( buttonState[i] == LOW ){
    controlMode = ( controlMode + 1 ) % 4;
    menuLcd();
    }
      
    if( buttonState[i] == 0 && buttonState[28] == 0 && buttonState[29] == 0 ){
      resetFunc();        
    }      
  }
}
