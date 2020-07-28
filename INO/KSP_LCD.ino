//  LCD Display functions.  Avoids the use of the clear()
//  each write fills 20 characters
bool lcdCheck(){
 if( lcdFlag == true ){
   if( lcdCurrent > (lcdPrevious + lcdRefresh) ){
     lcdPrevious = lcdCurrent;
     return true;
   }
 }
 return false; 
}

void showDisplay( int i ){
  if( i == -1 ){
    //  debug
    printLcd();
  }
  if( i == 1 ){
    lcdFuel3();
  }
  if( i == 2 ){
    lcdSoi();
  }
  if( i == 4 ){
    lcdVelocity();
  }
  if( i == 2 ){
    lcdSoi();
  }
  if( i == 5 ){
    lcdApsides();
  }
  if( i == 6 ){
    lcdTarget();
  }
}
int calcFuel( float t, float a ){
  if( t == 0.0 ){
  return 0;
  }else{
    if( a == 0.0 ){
      return 0;
    }else{
      return (( a / t ) * 100);
    }
  }
}
void lcdTarget(){
    //  KRA[TAR][0] = myTarget.distance;
    //  KRA[TAR][1] = myTarget.velocity; 
  lcdLine1 = buildSpaces( "Dist : ", String(KRA[TAR][0]), "", "", 20);
  lcdLine2 = buildSpaces( "Velo : ", String(KRA[TAR][1]), "", "", 20);
  lcdLine3 = emptyLine;
  lcdLine4 = emptyLine;
  lcdHasData = true;  
}
void lcdSoi(){
          //....5....0....5....0
  lcdLine1 = buildSpaces( "SOI : ", String(soiText), "", "", 20);
  lcdLine2 = emptyLine;
  lcdLine3 = emptyLine;
  lcdLine4 = emptyLine;
  lcdHasData = true;  
}
void clearLcd(){
          //....5....0....5....0
 lcdLine1 = emptyLine;
 lcdLine2 = emptyLine;
 lcdLine3 = emptyLine;
 lcdLine4 = emptyLine;
 lcdHasData = true;
}
void printLcd(){
  lcd.setCursor(0,0);
  lcd.print(lcdLine1);
  lcd.setCursor(0,1);
  lcd.print(lcdLine2);
  lcd.setCursor(0,2);
  lcd.print(lcdLine3);
  lcd.setCursor(0,3);
  lcd.print(lcdLine4); 
}

void menuLcd(){
  //lcd.cursor();
  String m = "";
  if( controlMode == 0 ){
    m = "Rocket";
    buttonMap[7] = 1;
    buttonMap[8] = 0;
    //setStickMap[sss];
  }
  if( controlMode == 1 ){
    m = "Plane";
    buttonMap[7] = 1;
    buttonMap[8] = 0; 
    //setStickMap[sss];   
  }
  if( controlMode == 2 ){
    m = "Rover";
    buttonMap[7] = 0;
    buttonMap[8] = 1;
    //setStickMap[sss];
  }
  if( controlMode == 3 ){
    m = "EVA";
  }
  
  lcdLine1 = buildSpaces( "Mode = ", m , "", "", 20);
  lcdLine2 = buildSpaces( "Sensitivty = ", String(sss), "", "", 20);
  lcdLine3 = emptyLine;
  lcdLine4 = emptyLine;
  //lcd.noCursor();
  lcdHasData = true;
}

void lcdVelocity(){
         //....5....0....5....0
  lcdLine1 = "      Velocity      ";
  lcdLine2 = buildSpaces( "Orbt= ", String(KRA[VEL][0]), " m/s", "", 20);
  lcdLine3 = buildSpaces( "Surf= ", String(KRA[VEL][1]), " m/s", "", 20);
  lcdLine4 = buildSpaces( "Vert= ", String(KRA[VEL][2]), " m/s", "", 20);
  
  lcdHasData = true;
  
}
void lcdApsides(){
         //....5....0....5....0  
  lcdLine1 = buildSpaces( "Apo= ", String(KRA[APS][1]), " m", "", 20);
  lcdLine2 = buildSpaces( "Apo@ ", String(apoapsis), " s", "", 20);
  lcdLine3 = buildSpaces( "Per= ", String(KRA[APS][0]), " m", "", 20);
  lcdLine4 = buildSpaces( "Per@ ", String(periapsis), " s", "", 20);

  lcdHasData = true;
  
}

void lcdFuel3(){

  int xLF = calcFuel(KRA[LF][0], KRA[LF][1]);
  int xLFS = calcFuel(KRA[LFS][0], KRA[LFS][1]);
  int xOX = calcFuel(KRA[OX][0], KRA[OX][1]);
  int xOXS = calcFuel(KRA[OXS][0], KRA[OXS][1]);
  int xSF = calcFuel(KRA[SF][0], KRA[SF][1]);
  int xSFS = calcFuel(KRA[SFS][0], KRA[SFS][1]);

         //....5....0....5....0
  lcdLine1 = "    STAGE    TOTAL  ";

  lcdLine2  = "LF";
  lcdLine2 += (fuelPadding(xLFS));
  lcdLine2 += xLFS;
  lcdLine2 += ("%   ");
  lcdLine2 += (fuelPadding(xLF));
  lcdLine2 += (xLF);
  lcdLine2 += ("%");

  lcdLine3  = "OX";
  lcdLine3 += (fuelPadding(xOXS));
  lcdLine3 += xOXS;
  lcdLine3 += ("%   ");
  lcdLine3 += (fuelPadding(xOX));
  lcdLine3 += (xOX);
  lcdLine3 += ("%");

  lcdLine4  = "SF";
  lcdLine4 += (fuelPadding(xSFS));
  lcdLine4 += xSFS;
  lcdLine4 += ("%   ");
  lcdLine4 += (fuelPadding(xSF));
  lcdLine4 += (xSF);
  lcdLine4 += ("%");

  lcdHasData = true;
}

String fuelPadding(int x){
  if( x == 100 ){
    return "  ";    //2 Spaces
  }
  if( x > 9 ){
    return "   ";   //3 Spaces
  }
    return "    "; //4 Spaces
        //  .....
}

String buildSpaces( String w, String x, String y, String z, int i ){
  String line = "";
  int j = 0;
  int k = 0;
  j = w.length() + x.length();
  line = w;
  line += x;
  if(y != ""){
    line += y;
    j += + y.length();
  }
  if(z != ""){
    line += z;
    j += + z.length();
  }  
  k = i - j;
  for( int a = 0; a < k ; a++ ){
    line+= " "; 
  }
return line;
}
