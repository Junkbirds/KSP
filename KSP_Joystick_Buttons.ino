////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//    KSP Buttons Section  ( only runs while kspFlag = true! )
////////////////////////////////////////////////////////////////////////////////////

void buttonPressedKsp( int i ){
  
  if(i == 0){  //  Toggle ABORT

    if(buttonCurrent[i] == LOW){
      mySimpit.activateAction(ABORT_ACTION);
    }else{
      mySimpit.deactivateAction(ABORT_ACTION);
    }
  }
  if(i == 1){  //  Toggle RCS

    if(buttonCurrent[i] == LOW){
      mySimpit.activateAction(RCS_ACTION);
    }else{
      mySimpit.deactivateAction(RCS_ACTION);
    }
  } 
  
  if(i == 2){  //  Toggle SAS
    if(buttonCurrent[i] == LOW){
      mySimpit.activateAction(SAS_ACTION);
    }else{
      mySimpit.deactivateAction(SAS_ACTION);
    }
  }  
  
  if(i == 3){  // STAGE
    if(buttonState[27] == LOW ){
      if(buttonCurrent[i] == LOW){
        mySimpit.activateAction(STAGE_ACTION);
      }
    }
  }

  if( i == 4 || i == 5 || i == 6 ){

      //Translation
      myTranslation.X    = buttonCurrent[4];
      myTranslation.Y    = buttonCurrent[5];
      myTranslation.Z    = buttonCurrent[6];
      myTranslation.mask = 7;
      mySimpit.send(TRANSLATION_MESSAGE, myTranslation);    
  }
  
  if( i == 7 || i == 8 || i == 9 ){
    if( controlMode == 0 ){  //RocketMode
      //rotation
      myRotation.yaw   = buttonCurrent[7];
      myRotation.pitch = buttonCurrent[8];
      myRotation.roll  = buttonCurrent[9];
      myRotation.mask  = 7;
      mySimpit.send(ROTATION_MESSAGE, myRotation);
    }
    if(controlMode == 1){  //Plane Mode
      myRotation.yaw   = buttonCurrent[9];
      myRotation.pitch = buttonCurrent[8];
      myRotation.roll  = buttonCurrent[7];
      myRotation.mask  = 7;
      mySimpit.send(ROTATION_MESSAGE, myRotation);  
    }
    if(controlMode == 2){ // Rover
      myWheel.throttle = buttonCurrent[8];
      //  wheel needs steering reversed.
      myWheel.steer = buttonCurrent[7];
      myWheel.mask = 3;
      mySimpit.send(WHEEL_MESSAGE, myWheel);       
    }
  }
  
  if(i == 10){  //  Throttle
    throttleValue = buttonCurrent[i];
    mySimpit.send(THROTTLE_MESSAGE, throttleValue);
  }
  
  if(i == 11){  //  Action Group lights - Toggle
    if(buttonState[i] == LOW){
      mySimpit.toggleAction(LIGHT_ACTION);
    } 
  }

  if(i == 14){  //  Action Group Solar
    if(buttonCurrent[i] == LOW){
      mySimpit.toggleCAG(1);
    }     
  }

  if(i == 19){  //  Action Group Chutes
    if(buttonCurrent[i] == LOW){
      mySimpit.activateCAG(2);
    }     
  }

   if(i == 20){  //  Action Group Brakes - Toggle
    if(buttonState[i] == LOW){
      mySimpit.toggleAction(BRAKES_ACTION);
    }  
  }
  
   if(i == 21){  //  Action Group Brakes - momentary
    if(buttonState[i] == LOW){
      mySimpit.activateAction(BRAKES_ACTION);
    }else{
      mySimpit.deactivateAction(BRAKES_ACTION);
    }     
  }
   
   if(i == 22){  //  Action Group Gear - toggle
     if(buttonState[i] == LOW){
       mySimpit.toggleAction(GEAR_ACTION);     
     }
   }

  if( i == 23 ) {
    if( buttonState[i] == LOW ){
      sss = ( sss + 1 ) % sssCount;
      setStickMap(sss);
      menuLcd();
    }
  }

  if( i == 24 ){
    chanMgmt(6); // Target data
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
  }
  
  if( i == 12 ){
    chanMgmt(2);
  }
  
  if( i == 13 ){
    
  }
  
  if( i == 15 ){

  //lcdFuel3();   
  chanMgmt(0);
  }
  
  if( i == 16 ){
    chanMgmt(1);
  }
  
  if( i == 17){  
    chanMgmt(4);
  }
  
  if( i == 18){
    chanMgmt(5);
  }
  
  if( i == 27 ){  // STAGE Safety Switch!
    // it's a toggle
    if( buttonState[i] == 0 ){
       analogWrite(buttonLed[3], ledBright);
    }else{
       //analogWrite(buttonLed[3], 0);
       ledOff(3, -1);
    }
  }
}
