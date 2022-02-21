float gob = 100;
int rec = 0;
int diffrence;
int emergency = 20;
int brake1 = 4;
int brake2 = 3;
int handle = 15;
int handleS = 14;
int str = 5;
int back = 6;
int gear = 7;
int pulse = 0;
int lastpulse;
int lastpulse2;
int minvalue;
int maxvalue;
int middlevalue;
int value=1;
int encoderA = 8;
int encoderB = 10;
int endstopL = 11;
int endstopR = 12;
float mingob;
int dis = 300;
float maxgob;
int save;

void setup() {
  pinMode(brake1, OUTPUT);
  pinMode(brake2, OUTPUT);
  pinMode(handle, OUTPUT);
  pinMode(handleS, OUTPUT);
  pinMode(str, OUTPUT);
  pinMode(back, OUTPUT);
  pinMode(gear, INPUT_PULLUP);
  pinMode(encoderA,INPUT_PULLUP);
  pinMode(encoderB,INPUT_PULLUP);
  pinMode(endstopL,INPUT_PULLUP);
  pinMode(endstopR,INPUT_PULLUP);
  Serial.begin(9600);
  Serial.setTimeout(5);

//point set right ***************************************************************
  lastpulse = digitalRead(encoderA);
   lastpulse2 = digitalRead(encoderB);
   while (value != 0) {
     digitalWrite(handleS, HIGH);
     int rate = digitalRead(encoderA);
     int rate2 = digitalRead(encoderB);
     if (lastpulse == 0) {
       if (rate == 1) {
         if (rate2 == 0) {
           pulse++;
       } else {
           pulse--;
       }
     }
   }
   lastpulse = rate;
   lastpulse2 = rate2;
   value = digitalRead(endstopL);
 }
 
  digitalWrite(handleS, LOW);
  maxvalue = pulse;
  value = 1;

  
 //point set left ***************************************************************
  while (value != 0) {
     digitalWrite(handle, HIGH);
     int rate = digitalRead(encoderA);
     int rate2 = digitalRead(encoderB);
     if (lastpulse == 0) {
       if (rate == 1) {
         if (rate2 == 0) {
           pulse++;
       } else {
           pulse--;
       }
     }
   }
   lastpulse = rate;
   lastpulse2 = rate2;
   value = digitalRead(endstopR);
   }
   
   digitalWrite(handle, LOW);
   minvalue = pulse;

   //calculate **************************************************************
   middlevalue = round((maxvalue + minvalue) / 2);

   
   maxvalue = maxvalue - middlevalue;
   minvalue = minvalue - middlevalue;
   middlevalue = middlevalue - middlevalue;
   pulse = minvalue;
   
   rec = pulse;
   mingob = abs(minvalue / gob);
   maxgob = maxvalue / gob;

   //first angle set***************************************************************
   int ser = 0;
while (ser == 0) {
  if (Serial.available()) {
    rec = Serial.parseInt();
    ser = 1;
    if (rec < 0) {
      rec = round(rec * mingob);
    } else {
      rec = round(rec * maxgob);
    }

    if (rec > maxvalue) {
    rec = maxvalue;
  }
  if (rec < minvalue) {
    rec = minvalue;
  }

   lastpulse = digitalRead(encoderA);
  lastpulse2 = digitalRead(encoderB);

  if (pulse != rec) {
  while (pulse != rec) {

    
    if (rec > maxvalue) {
    rec = maxvalue;
  }
  if (rec < minvalue) {
    rec = minvalue;
  }
    if(digitalRead(endstopR) == 0 ) {
      minvalue = pulse;
    }
    if(digitalRead(endstopL) == 0) {
      maxvalue = pulse;
    }
  
     if (rec > pulse) {
       digitalWrite(handle, LOW);
       digitalWrite(handleS, HIGH);
    } else if (rec < pulse) {
      digitalWrite(handle, HIGH);
      digitalWrite(handleS, LOW);
    }

     
     int rate = digitalRead(encoderA);
     int rate2 = digitalRead(encoderB);
     if (lastpulse == 0) {
       if (rate == 1) {
         if (rate2 == 0) {
           pulse++;
       } else {
           pulse--;
       }
     }
   }
   lastpulse = rate;
   lastpulse2 = rate2;
   }
   } else {
    digitalWrite(handle, LOW);
       digitalWrite(handleS, LOW);
   }
  
  }
}
}

void loop() {
  //Serial receive*************************************************************
  if (Serial.available()) {
    rec = Serial.parseInt();
    //edit****************************
    if (rec < 0) {
      rec = round(rec * mingob);
    } else {
      rec = round(rec * maxgob);
    }
  }

  //received value edit*******************************************************
  if (rec > maxvalue) {
    rec = maxvalue;
  }
  if (rec < minvalue) {
    rec = minvalue;
  }

  //diffrence calculate*****************************************************
  if (pulse > 0) {
    if (rec > 0) {
      diffrence = abs(pulse - rec);
    } else {
      diffrence = abs(pulse + abs(rec));
    }
  } else {
    if (rec > 0) {
      diffrence = abs(abs(pulse) + rec);
    } else {
      diffrence = abs(abs(pulse) - abs(rec));
    }
  }

  //rotate*******************************************************************************
  lastpulse = digitalRead(encoderA);
  lastpulse2 = digitalRead(encoderB);

  if (diffrence > 0) {
    
    if (rec > pulse) {
       digitalWrite(handle, LOW);
       digitalWrite(handleS, HIGH);
    } else if (rec < pulse) {
      digitalWrite(handle, HIGH);
      digitalWrite(handleS, LOW);
    }
    
  } else {
    digitalWrite(handle, LOW);
    digitalWrite(handleS, LOW);
  }


  //detect rotate angle********************
     int rate = digitalRead(encoderA);
     int rate2 = digitalRead(encoderB);
     if (lastpulse == 0) {
       if (rate == 1) {
         if (rate2 == 0) {
           pulse++;
       } else {
           pulse--;
       }
     }
   }
   //detect rotate angle end*****************
}
