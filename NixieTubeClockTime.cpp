#include <WiFi.h>
#include "time.h"
#include "EasyNixie.h"       //Nixie library
#define OUT_EN 23   //connect Pin ArduinoUNO 3 to OUT_EN
#define SHCP   22   //connect Pin ArduinoUNO 2 to SHCP
#define STCP   21   //connect Pin ArduinoUNO 6 to STCP
#define DSIN   19   //connect Pin ArduinoUNO 7 to DSIN 
#define TOUCH_PIN  27
#define LED1   14
#define LED2   12
EasyNixie en(OUT_EN,SHCP, STCP, DSIN); //Initialisation 'en' protocole
uint8_t nixieSelect;
const char* ssid = "Wifi Name";
const char* password = "Wifi password";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -18000;
const int daylightOffset_sec = 3600;
int minuteInt, hourInt, _time, day, prevDay;

char minuteChar[3], hourChar[3], dayChar[3];
unsigned long lastSetTime = 0;
const unsigned long cooldown = 5000;
bool copyIsTouched; 
bool lightsOn = false;

u16_t delayTicks = 100;
const long timeUpdateCooldown = 2000;
long lastUpdated = 0;
unsigned long now = millis();


void syncTime() {
  Serial.print("in syncTime(): ");
  Serial.println(millis());
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) // Ensure Connection
  {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  }
  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void printLocalTime() {
  Serial.print("in printLocalTime(): ");
  Serial.println(millis());
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
}

void updateTime() {
  Serial.print("in updateTime(): ");
  Serial.println(millis());
  struct tm timeinfo;
  getLocalTime(&timeinfo);
  strftime(hourChar, 3, "%H", &timeinfo);
  strftime(minuteChar, 3, "%M", &timeinfo);
  strftime(dayChar, 3, "d", &timeinfo);
}

void setLEDLights(bool turnOn) {  
  char buffer[30];
  sprintf(buffer, "in setLEDLights(turnOn: %d): ", turnOn);
  Serial.print(buffer);
  Serial.println(millis());
  if (turnOn)
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
  } else
  {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);

  }
}

void setNixieLights(bool turnOn, uint8_t select) {
  char buffer[44];
  sprintf(buffer, "in setNixieLights(turnOn: %d, select: %d): ", turnOn, select);
  Serial.print(buffer);
  Serial.println(millis());
  int a = minuteInt%10;
  int b = minuteInt/10;
  int c = hourInt%10;
  int d = hourInt/10;
  int ledLight = 0;
  if (turnOn)
  {
    ledLight = 3;
  }
      
  for (int i = 0; i < 4; i++)
  {
    en.SetNixie(a,ledLight,turnOn && (select & 1) && i >= 0);           //en.SetNixie Last number 
    en.SetNixie(b,ledLight,turnOn && (select & 2) && i >= 1);           //en.SetNixie Minute 
    en.SetNixie(c,ledLight,turnOn && (select & 4) && i >= 2);           //en.SetNixie Hour
    en.SetNixie(d,ledLight,turnOn && (select & 8) && i >= 3);           //en.SetNixie First Number 
    en.Latch();                      //Send your set to EaseNixie 
    delay(50);
  }
  
}


void startUpSequence() {
  Serial.print("in startUpSequence(): ");
  Serial.println(millis());
  for (uint8_t i = 0; i < 4; i++)
  {
    for (uint8_t j = 0; j < 9; j++)
    {
      setNixieLights(false, pow(2,i)); // Could be done with bit shifting
      delay(150);
    }
    delay(200);
  }
  setLEDLights(true);
  setNixieLights(false, 15);
  delay(5000);
  setNixieLights(false, 0);
  setLEDLights(false);
}

void setup() {
  Serial.begin(115200);
  pinMode(TOUCH_PIN, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  setNixieLights(false, 0);
  setLEDLights(false);
  syncTime(); // Call to the ntp server
  //startUpSequence();
  
  printLocalTime();

  struct tm timeinfo;
  getLocalTime(&timeinfo);
  day = timeinfo.tm_mday;
  prevDay = day;

}

void loop() {

  now = millis();
  if (now - lastUpdated > timeUpdateCooldown) {
    updateTime();
    lastUpdated = now;
  }

  int touchValue = touchRead(TOUCH_PIN);
  bool touchingNow = touchValue < 50;
  char buffer[44];
  sprintf(buffer, "TouchRead: %d", touchValue);
  Serial.println(buffer);

  if (prevDay != day) {
    syncTime();
    prevDay = day;
  }

  minuteInt = (minuteChar[0] - '0') * 10 + (minuteChar[1] - '0');
  hourInt = (hourChar[0] - '0') * 10 + (hourChar[1] - '0');

  if (touchingNow && !lightsOn && (now - lastSetTime > cooldown)) {
    lightsOn = true;
    setNixieLights(true, 15);
    setLEDLights(true);
    lastSetTime = now;

  } else if (!touchingNow && lightsOn && (now - lastSetTime > cooldown)) {
    setNixieLights(false, 0);
    setLEDLights(false);
    lightsOn = false;
    lastSetTime = now;
  }
  
  delay(delayTicks);

}


