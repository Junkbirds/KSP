void potRead(int i){  //  REQUIRED FUNCTION - called from scanButtons();
    int j = buttonMap[ i ];
    int k = buttonMap[ i ] + 2;
    int l = 0;
    int tempRead = 0;
    tempRead = analogRead(button[i]);  //  Read in the Raw Analog value into tempRead. 
    if( buttonType[ i ] == 3 ){  ///  Joystick
      if( tempRead > limitNeg && tempRead < limitPos){  //  check if out of DeadZone
      buttonCurrent[ i ] = 0;
      }else{
        if( tempRead > centerPos ){ l = j; }else{ l = k; }
          ///  sss = Stick Sensitivity Setting 
          buttonCurrent[ i ] = constrain(map(tempRead,stickMap[ l ][ 0 ],stickMap[ l ][ 1 ],stickMap[ l ][ 2 ],stickMap[ l ][ 3 ]),maxNeg[ sss ],maxPos[ sss ]);
      } 
    } else {  // buttonType == 4  (Throttle POT)
      l=j;
      buttonCurrent[i] = constrain(map(tempRead,stickMap[ l ][ 0 ],stickMap[ l ][ 1 ],stickMap[ l ][ 2 ],stickMap[ l ][ 3 ]),tMin,tMax);   
    }
}
