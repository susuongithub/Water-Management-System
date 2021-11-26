#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define echoPin 4 // attach pin GPIO 4(D2) esp8266 to pin Echo of HC-SR04
#define trigPin 0 //attach pin GPIO D3 esp8266 to pin Trig of HC-SR04
#define BLYNK_PRINT Serial
#define relay1 16
int max_depth ;

char auth[] = "czcNJTOe_hZxEp1nGKl3ylC4gMKFWlvA";
char ssid[] = "WinterFell";
char pass[] = "heyuitsfree";
int level,mode ;
int max_wl = 80 ;
int min_wl = 40 ;


void online();
void offline();
void automatic();
void manuval();

BLYNK_WRITE(V1)
{
  switch(param.asInt()){
    case 1 :{
      mode = 1;
      break;
    }
    case 2 :{
      mode = 0 ;
    }
  }
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  if(WiFi.status() == WL_CONNECTED){
    online();
  }
  else{
    offline();

  }
}
void online(){
  long duration; // variable for the duration of sound wave trave
  int distance; // variable for the distance measurement
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  if(distance <= max_depth){
    level = 100 - ((distance*100)/max_depth) ;
  }if(distance > max_depth ){
    level = 101;
}


BLYNK_WRITE(V1);

if (mode == 1){  automatic();
}
if (mode == 0){
  manuval();
}
  Blynk.run();
}

void automatic(){
  if( level  >= max_wl && level != 101 ){
    Blynk.virtualWrite(V0,level);
    digitalWrite(relay1,HIGH);
  }
  if(level < min_wl  ){
    Blynk.virtualWrite(V0,level);
    digitalWrite(relay1,LOW);
    }
  }

  void manuval (){
    if( level  >= 80 && level != 101 ){

      Blynk.virtualWrite( V0 , level);
    }
    if(level < 50  ){
      Blynk.virtualWrite( V0 , level);
    }
  }


//****************************************************************************
void offline(){
  long duration; // variable for the duration of sound wave trave
  int distance; // variable for the distance measurement
  digitalWrite( trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite( trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite( trigPin, LOW);
  duration = pulseIn( echoPin, HIGH);
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  if( distance <= max_depth){
    level = 100 -((distance*100)/max_depth) ;
  }
  if( distance > max_depth ){
    level = 101 ;
  }
  if( level  >= max_wl && level != 101 ){

    Blynk.virtualWrite( V0 , level);
    digitalWrite( relay1 , HIGH );
  }
  if( level < min_wl &&  level  !=  101 ){
    Blynk.virtualWrite( V0 , level );
    digitalWrite( relay1 , LOW );
    }
  Blynk.run();
}
