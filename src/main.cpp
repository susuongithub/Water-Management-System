#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define echoPin 4 // attach pin GPIO 4(D2) esp8266 to pin Echo of HC-SR04
#define trigPin 0 //attach pin GPIO D3 esp8266 to pin Trig of HC-SR04
#define BLYNK_PRINT Serial
#define relay1 16
float max_depth = 10;

char auth[] = "czcNJTOe_hZxEp1nGKl3ylC4gMKFWlvA";
char ssid[] = "WinterFell";
char pass[] = "heyuitsfree";
float  level ;
int mode ;
int max_wl = 60 ;
int min_wl = 25 ;


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
  //digitalWrite(relay1,HIGH);
  offline();


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
  if(digitalRead(relay1)== LOW ){
    Blynk.virtualWrite(V4,255);
  }
  else{
    Blynk.virtualWrite(V4,0);
  }
  long duration; // variable for the duration of sound wave trave
  float distance; // variable for the distance measurement
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
  Blynk.virtualWrite(V2,distance);
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
  if(level != 101){Blynk.virtualWrite(V0,level);}

  if( level  >= max_wl && level != 101 ){
    //Blynk.virtualWrite(V0,level);
    digitalWrite(relay1,HIGH);

  }
  if(level < min_wl  ){
    //Blynk.virtualWrite(V0,level);
    digitalWrite(relay1,LOW);
    delay(1000);
    }
  }

  void manuval (){
    Blynk.virtualWrite(V0,level);
    /*if( level  >= max_wl && level != 101 ){

      Blynk.virtualWrite( V0 , level);
    }
    if(level < min_wl  ){
      Blynk.virtualWrite( V0 , level);
    }*/
    Blynk.run();
  }


//****************************************************************************
void offline(){
  long duration; // variable for the duration of sound wave trave
  float distance; // variable for the distance measurement
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
    delay(1000);
    }
  Blynk.run();
}
