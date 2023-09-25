#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_PRINT Serial

#define relay D4

#include <Servo.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

char auth[] = "";
char ssid[] = "name";
char pass[] = "pass";

Servo fishfeeder;
int StartPosition = 0;
int MovePosition = 180;
int OpenTime = 1000;

int startTime1;
int endTime1;
int startTime2;
int endTime2;


WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 28800;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);


void setup(){
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);

  Serial.begin(9600);
  fishfeeder.attach(D8);
  fishfeeder.write(StartPosition);
  Blynk.begin(auth, ssid, pass);
}

BLYNK_WRITE(V0){
  fishfeeder.write(MovePosition);
  delay(OpenTime);
  fishfeeder.write(StartPosition);
}

BLYNK_WRITE(V1){
  bool value1 = param.asInt();
  if(value1 == 1){
    fishfeeder.write(MovePosition);
  }
  else{
    fishfeeder.write(StartPosition);
  }
}

BLYNK_WRITE(V2) {
  bool value2 = param.asInt();
  if (value2 == 1) {
    digitalWrite(relay, LOW);
  } else {
    digitalWrite(relay, HIGH);
  }
}

BLYNK_WRITE(V4){
 startTime1 = param[0].asInt();
 endTime1 = param[1].asInt();
 if(startTime1 == 0 && endTime1 == 0)
 {
  startTime1 = 999999;
  endTime1 = 999999;
 }
}

BLYNK_WRITE(V5){
 startTime2 = param[0].asInt();
 endTime2 = param[1].asInt();
 if(startTime2 == 0 && endTime2 == 0)
 {
  startTime2 = 999999;
  endTime2 = 999999;
 }
}




void loop(){
  timeClient.update();
  int HH = timeClient.getHours();
  int MM = timeClient.getMinutes();
  int SS = timeClient.getSeconds();
  int server_time = 3600*HH + 60*MM + SS;

  if(startTime1 == server_time)
  {
      fishfeeder.write(MovePosition);
  }

  if(endTime1 == server_time)
  {
      fishfeeder.write(StartPosition);
  }

  if(startTime2 == server_time)
  {
      digitalWrite(relay, LOW);
  }
  if(endTime2 == server_time)
  {
      digitalWrite(relay, HIGH);
  }

  Blynk.run();
}
