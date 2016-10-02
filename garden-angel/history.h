/*
  history.h

  used to manage data historization and restitution.

*/

time_t LOCAL_TIME=1475394875;

const int HIST_CAPACITY=76;
float HIST_LENGTH=86400*1.5;
int HIST_PERIOD=int (HIST_LENGTH/HIST_CAPACITY);

int index = 0;
int histFull = 0;

time_t dateHist[HIST_CAPACITY];
float tempHist[HIST_CAPACITY];
float humHist[HIST_CAPACITY];
float lightHist[HIST_CAPACITY];

void timestamp() {   
    time_t t = now();
    Serial.print(weekday(t));
    Serial.print(F("/"));
    Serial.print(month(t));
    Serial.print(F("/"));
    Serial.print(year(t));
    Serial.print(F(" "));
    Serial.print(hour(t));
    Serial.print(F(":"));
    Serial.print(minute(t));
    Serial.print(F(":"));
    Serial.print(second(t));
    Serial.print(F(" "));
}

void printDate(time_t t) {   
    Serial.print(weekday(t));
    Serial.print(F("/"));
    Serial.print(month(t));
    Serial.print(F("/"));
    Serial.print(year(t));
    Serial.print(F(" "));
    Serial.print(hour(t));
    Serial.print(F(":"));
    Serial.print(minute(t));
    Serial.print(F(":"));
    Serial.print(second(t));
    Serial.print(F(";"));
}

void pushData(float h, float t, float l) {
  for (int i = 1; i<HIST_CAPACITY; i++) {
    dateHist[i-1] = dateHist[i];
    humHist[i-1] = humHist[i];
    tempHist[i-1] = tempHist[i];
    lightHist[i-1] = lightHist[i];
  }
  dateHist[HIST_CAPACITY-1] = now();
  humHist[HIST_CAPACITY-1] = h;
  tempHist[HIST_CAPACITY-1] = t;
  lightHist[HIST_CAPACITY-1] = l;
}

int storeData(float h, float t, float l) {
    if (index == 0 || (index > 0 && now()-dateHist[index-1]>=HIST_PERIOD)) {
      if (histFull == 0) {
        dateHist[index] = now();
        humHist[index] = h;
        tempHist[index] = t;
        lightHist[index] = l;
    
        index++;
      } else {
        pushData(h, t, l);
      }

      if (index == HIST_CAPACITY) {
       histFull = 1;
      }
      
      return(1);
    }
    return(0);
}

void flushHistory() {
  for (int i = 0; i < index; i++) {
      printDate(dateHist[i]);
      Serial.print(humHist[i]);
      Serial.print(F(";"));
      Serial.print(tempHist[i]);
      Serial.print(F(";"));
      Serial.print(lightHist[i]);
      Serial.print(F(";"));
      Serial.println(i);
   }
   Serial.println(F(""));
}

void flushFullHistory() {
   for (int i = 0; i < HIST_CAPACITY; i++) {
      printDate(dateHist[i]);
      Serial.print(humHist[i]);
      Serial.print(F(";"));
      Serial.print(tempHist[i]);
      Serial.print(F(";"));
      Serial.print(lightHist[i]);
      Serial.print(F(";"));
      Serial.println(i);
   }
   Serial.println(F(""));
}

void flushHistoryRequest() {
  Serial.println(F(""));
  Serial.println(F("date;humidity(%);temperature(°C);light(%);index"));
  
  if (histFull == 1) {
    flushFullHistory(); 
  } else {
    flushHistory();
  }
}

