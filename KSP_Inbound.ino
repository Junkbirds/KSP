void mcbh(byte messageType, byte msg[], byte msgSize) {

  switch(messageType) {
  case 3:  //Scene Change  Test to see???
    chanMgmt(0);
    menuLcd();
    
  case 8:  //  Altitude ALT(12)
    if (msgSize == sizeof(altitudeMessage)) {
      altitudeMessage myAltitude;
      myAltitude = parseAltitude(msg);
      KRA[ALT][0] = myAltitude.sealevel;
      KRA[ALT][1] = myAltitude.surface; 
    }
    break;
  case 9:  //  APSIDES  APS( )
    if (msgSize == sizeof(apsidesMessage)) {
      apsidesMessage myApsides;
      myApsides = parseApsides(msg);
      KRA[APS][0] = myApsides.periapsis;
      KRA[APS][1] = myApsides.apoapsis; 
    }
    break;

  case 22:  //Velocity VEL(13)
    if (msgSize == sizeof(velocityMessage)) {
      velocityMessage myVelocity;
      myVelocity = parseVelocity(msg);
      KRA[VEL][0] = myVelocity.orbital;
      KRA[VEL][1] = myVelocity.surface;
      KRA[VEL][2] = myVelocity.vertical; 
    }
    break;

  case 23:  //Actionstatus ACT(14)
    
  case 24:  //apsidestime  int32_t  -- NOT KRA[][]
    if (msgSize == sizeof(apsidesTimeMessage)) {
      apsidesTimeMessage myApsidesTime;
      myApsidesTime = parseApsidesTime(msg);
      periapsis = myApsidesTime.periapsis;
      apoapsis = myApsidesTime.apoapsis; 
    }
    break;
    
  case 25:  //target info  TAR(15)
    if (msgSize == sizeof(targetMessage)) {
      targetMessage myTarget;
      myTarget = parseTarget(msg);
      KRA[TAR][0] = myTarget.distance;
      KRA[TAR][1] = myTarget.velocity; 
    }
    break;

  case 27:  // Airspeed  AIR(16)
    if (msgSize == sizeof(airspeedMessage)) {
      airspeedMessage myAirspeed;
      myAirspeed = parseAirspeed(msg);
      KRA[AIR][0] = myAirspeed.IAS;
      KRA[AIR][1] = myAirspeed.mach; 
    }
    break;
       
  case 26:  //  SOI
    soiText = msg;  break;
  case 10:  //  LF
    if (msgSize == sizeof(resourceMessage)) { rMsg(messageType, msg, msgSize, LF);    } break;
  case 11:  //  LFS
    if (msgSize == sizeof(resourceMessage)) { rMsg(messageType, msg, msgSize, LFS);   } break;
  case 12:  //  OX
    if (msgSize == sizeof(resourceMessage)) { rMsg(messageType, msg, msgSize, OX);    } break;
  case 13:  //  OXS
    if (msgSize == sizeof(resourceMessage)) { rMsg(messageType, msg, msgSize, OXS);   } break;
  case 14:  //  SF
    if (msgSize == sizeof(resourceMessage)) { rMsg(messageType, msg, msgSize, SF);    } break;
  case 15:  //  SFS
    if (msgSize == sizeof(resourceMessage)) { rMsg(messageType, msg, msgSize, SFS);   } break;
  case 16:  //  MONO
    if (msgSize == sizeof(resourceMessage)) { rMsg(messageType, msg, msgSize, MONO);  } break;
  case 17:  //  ELEC
    if (msgSize == sizeof(resourceMessage)) { rMsg(messageType, msg, msgSize, ELEC);  } break;
  case 18:  //  EVA
    if (msgSize == sizeof(resourceMessage)) { rMsg(messageType, msg, msgSize, EVA);   } break;
  case 19:  //  ORE
    if (msgSize == sizeof(resourceMessage)) { rMsg(messageType, msg, msgSize, ORE);   } break;
  case 20:  //  AB
    if (msgSize == sizeof(resourceMessage)) { rMsg(messageType, msg, msgSize, AB);    } break;
  case 21:  //  ABS
    if (msgSize == sizeof(resourceMessage)) { rMsg(messageType, msg, msgSize, ABS);   } break;
  }
}

void rMsg(byte messageType, byte msg[], byte msgSize, byte resId){
  resourceMessage myResource;
  myResource = parseResource(msg);
  KRA[resId][0] = myResource.total;
  KRA[resId][1] = myResource.available; 
}

void chanMgmt(byte L) {
  lcdDisplayGroupId = L;
  if( lcdDisplayGroupId == lcdDisplayGroupIdPrevious ){
    //Nothing to change
  }else{
    //Deregister channels!
    switch(lcdDisplayGroupIdPrevious){
      case 0:  //No existing channels  -- nothing to de-register
        break;
      case 1:
        mySimpit.deregisterChannel(10);  //LF
        mySimpit.deregisterChannel(11);  //LFStage
        mySimpit.deregisterChannel(12);  //OX
        mySimpit.deregisterChannel(13);  //OXStage
        mySimpit.deregisterChannel(14);  //SF
        mySimpit.deregisterChannel(15);  //SFStage        
        break;
      case 2:
        mySimpit.deregisterChannel(26);  //SOI_MESSAGE
        break;
      case 3:
        mySimpit.deregisterChannel(16);  //Mono
        mySimpit.deregisterChannel(17);  //Elec
        break;
      case 4:
        mySimpit.deregisterChannel(22);  // Velocity
        break;
      case 5:
        mySimpit.deregisterChannel(9);   // Aspides
        mySimpit.deregisterChannel(24);  // AspidesTime
        break;
      case 6:
        mySimpit.deregisterChannel(25);   // Target
        break;        
    }
    //Register Channels!
    switch(lcdDisplayGroupId){  //De-register
      case 0:  //no channels to register
        break;
      case 1:
        mySimpit.registerChannel(10);  //LF
        mySimpit.registerChannel(11);  //LFStage
        mySimpit.registerChannel(12);  //OX
        mySimpit.registerChannel(13);  //OXStage
        mySimpit.registerChannel(14);  //SF
        mySimpit.registerChannel(15);  //SFStage          
        break;
      case 2:
        mySimpit.registerChannel(26);  // SOI_MESSAGE
        break;
      case 3:
        mySimpit.registerChannel(16);  // Mono
        mySimpit.registerChannel(17);  // Elec
        break;
      case 4:
        mySimpit.registerChannel(22);  // Velocity
        break;
      case 5:
        mySimpit.registerChannel(9);   // Aspides
        mySimpit.registerChannel(24);  // AspidesTime
        break;
      case 6:
        mySimpit.registerChannel(25);   // Target
        break;
       
    }  
  }
  lcdDisplayGroupIdPrevious = lcdDisplayGroupId;
}
  /*
  mySimpit.registerChannel(8);  //  ALTITUDE_MESSAGE                        //ALTITUDE
  mySimpit.registerChannel(9);  //  APSIDES_MESSAGE                         //APSIDES
  mySimpit.registerChannel(10); //  LF_MESSAGE   (Liquid fuel)              //RESOURCE
  mySimpit.registerChannel(11); //  LF_STAGE_MESSAGE                        //RESOURCE
  mySimpit.registerChannel(12); //  OX_MESSAGE   (Oxidizer fuel)            //RESOURCE
  mySimpit.registerChannel(13); //  OX_STAGE_MESSAGE                        //RESOURCE
  mySimpit.registerChannel(14); //  SF_MESSAGE   (Solid fuel)               //RESOURCE
  mySimpit.registerChannel(15); //  SF_STAGE_MESSAGE                        //RESOURCE
  mySimpit.registerChannel(16); //  MONO_MESSAGE   (MONOProp)               //RESOURCE
  mySimpit.registerChannel(17); //  ELECTRIC_MESSAGE   (Electric Charge)    //RESOURCE
  mySimpit.registerChannel(18); //  EVA_MESSAGE   (EVA Prop)                //RESOURCE
  mySimpit.registerChannel(19); //  ORE_MESSAGE   (ORE)                     //RESOURCE
  mySimpit.registerChannel(20); //  AB_MESSAGE   (ABLATOR)                  //RESOURCE
  mySimpit.registerChannel(21); //  AB_STAGE_MESSAGE   (Ablator)            //RESOURCE
  mySimpit.registerChannel(22); //  VELOCITY_MESSAGE   (Velocity)           //Velocity
  mySimpit.registerChannel(23); //  ACTIONSTATUS_MESSAGE  (Action Group)    //CUSTOM
  mySimpit.registerChannel(24); //  APSIDESTIME_MESSAGE                     //apsidesTimeMessage
  mySimpit.registerChannel(25); //  TARGETINFO_MESSAGE                      //targetInfoMessage
  mySimpit.registerChannel(26); //  SOI_MESSAGE                             //ASCII STRING
  mySimpit.registerChannel(27); //  AIRSPEED_MESSAGE                        //Veolcity ????
  */
