#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

WiFiClient espClient;

const char* ssid = "University of Washington"; // Wifi network SSID
const char* password = ""; // Wifi network password

String ip = "172.28.219.179"; // Sieg Master IP
String api_token = "rARKEpLebwXuW01cNVvQbnDEkd2bd56Nj-hpTETB"; // Sieg Master API Token

//Sieg lower floor
int lowerLights[] = {10, 23, 11, 15, 7, 14, 22, 21, 16};
int upperLights[] = {18, 20, 12, 25, 26, 5, 8, 19, 13, 24, 9, 17};

//light settings
int transitionTime = 10;
int patternTransition = 4;

typedef struct {
  String col;
  String bri;
  String sat;
} randomValues;

randomValues rv;

unsigned long currentTime, lastPattern, timerOne, timerTwo; //timing

long lastUpLight, currentUpLight, lastLowLight, currentLowLight;

bool debug = false;

void setup() {
  Serial.begin(115200);
  setup_wifi();
}

void loop() {
  currentTime = millis(); //Update time 
  iterateLights();
  
  if (currentTime - timerOne > 70000) {
    lowerLightsPattern();
  }
  
  if (currentTime - timerTwo > 180000) {
    for( int i = 0; i < 3; i++){
      upperLightsPatternWave();
    }
  }
  
  wifiReconnect();
}

void iterateLights(){
  for(int i; i = 0; i < 2){
    lowerLightsCycle();
    delay(100);
    upperLightsCycle();
    delay(100);
  }
}

void lowerLightsCycle(){
      randomLightValues();
      currentLowLight = lowerLights[random(0,9)];
      while(currentLowLight == lastLowLight){
        currentLowLight = upperLights[random(0,12)];
      }
      changeLight(currentLowLight, transitionTime, "on", "true", "bri", rv.bri, "hue", rv.col, "sat", rv.sat);
      lastLowLight = currentLowLight;
}

void upperLightsCycle(){
      randomLightValues();
      currentUpLight = upperLights[random(0,12)];
      while(currentUpLight == lastUpLight){
        currentUpLight = upperLights[random(0,12)];
      }
      changeLight(currentUpLight, transitionTime, "on", "true", "bri", rv.bri, "hue", rv.col, "sat", rv.sat);
      lastUpLight = currentUpLight;
}

void lowerLightsPattern(){
      Serial.println("LOWER PATTERN");
      Serial.println("LOWER PATTERN");
      Serial.println("LOWER PATTERN");
      randomValues();
      changeLight(22, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      changeLight(15, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      changeLight(10, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      delay(400);
      changeLight(21, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      changeLight(7, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      changeLight(23, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      delay(400);
      changeLight(16, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      changeLight(14, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      changeLight(11, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      timerOne = currentTime;
      delay(5000);
      for (int i = 0; i < 9; i++){
        changeLight(lowerLights[i], 0, "on", "true", "bri", "50", "hue", rv.col, "sat", "0");
      }
      for (int i = 0; i < 9; i++){
        changeLight(lowerLights[i], 0, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      }
      for (int i = 0; i < 9; i++){
        changeLight(lowerLights[i], 0, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      }
      delay(1000);
      for (int i = 0; i < 2; i++){
        changeGroup(1, 10, "on", "true", "bri", "100", "hue", rv.col, "sat", "254");
        delay(1000);
        changeGroup(1, 10, "on", "true", "bri", "255", "hue", rv.col, "sat", "254");
        delay(1000);
      }
}

void upperLightsPatternWave(){
      Serial.println("UPPER PATTERN");
      Serial.println("UPPER PATTERN");
      Serial.println("UPPER PATTERN");
      randomLightValues();
      for(int i = 0; i < 12; i++) {
        changeLight(upperLights[i], patternTransition, "on", "true", "bri", "50", "hue", rv.col, "sat", "254");
        changeLight(upperLights[i], patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      }
      timerTwo = currentTime;
      delay(3000);
}

void randomLightValues(){
  rv.col = String(random(38000, 60000));
  rv.bri = String(random(190, 254));
  rv.sat = String(random(200, 254));
  if (debug){
    Serial.println("NEW VALUES");
  }
}

void wifiReconnect(){
  if (WiFi.status() != WL_CONNECTED) {
      delay(1);
      Serial.print("WIFI Disconnected. Attempting reconnection.");
      setup_wifi();
      return; //End this loop cycle if WiFi disconnected
    }
}
void setup_wifi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }
  Serial.println("Connected to the WiFi network");
}
