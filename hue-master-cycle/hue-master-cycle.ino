#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

WiFiClient espClient;

const char* ssid = "University of Washington"; // Wifi network SSID
const char* password = ""; // Wifi network password

String ip = "172.28.219.179"; // Sieg Master IP
String api_token = "rARKEpLebwXuW01cNVvQbnDEkd2bd56Nj-hpTETB"; // Sieg Master API Token

//Sieg lower floor
int lowerLights[] = {10, 57, 11, 15, 7, 14, 22, 21, 16};
int upperLights[] = {18, 20, 12, 25, 26, 5, 8, 19, 13, 24, 9, 17};

//light settings
int transitionTime = 5;
int patternTransition = 1;

typedef struct {
  String col;
  String bri;
  String sat;
} randomValues;

randomValues rv;

unsigned long currentTime, lastPattern, timerOne, timerTwo; //timing

bool debug = true;

void setup() {
  Serial.begin(115200);
  setup_wifi();
}

void loop() {
  currentTime = millis(); //Update time 
  iterateLights();
  if (currentTime - timerOne > 60000) {
    lowerLightsPattern();
  }
  if (currentTime - timerTwo > 180000) {
    for( int i = 0; i < 3; i++){
      upperLightsPatternWave();
    }
    delay(5000);
    timerTwo = currentTime;
  }
  if (WiFi.status() != WL_CONNECTED) {
      delay(1);
      Serial.print("WIFI Disconnected. Attempting reconnection.");
      setup_wifi();
      return; //End this loop cycle if WiFi disconnected
    }
}

void iterateLights(){  
  lowerLightsCycle();
  upperLightsCycle();
  delay(800);
}

void lowerLightsCycle(){
      randomLightValues();
      int lightNum = lowerLights[random(0,9)];
      changeLight(lightNum, transitionTime, "on", "true", "bri", rv.bri, "hue", rv.col, "sat", rv.sat);
}

void upperLightsCycle(){
      randomLightValues();
      int lightNum = upperLights[random(0,12)];
      changeLight(lightNum, transitionTime, "on", "true", "bri", rv.bri, "hue", rv.col, "sat", rv.sat);
}

void lowerLightsPattern(){
      randomValues();
      changeLight(22, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      changeLight(15, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      changeLight(10, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      delay(400);
      changeLight(21, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      changeLight(7, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      changeLight(57, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      delay(400);
      changeLight(16, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      changeLight(14, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      changeLight(11, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      timerOne = currentTime;
      delay(5000);
      for (int i = 0; i < 9; i++){
        changeLight(lowerLights[i], 0, "on", "false", "bri", rv.bri, "hue", rv.col, "sat", "254");
      }
      for (int i = 0; i < 9; i++){
        changeLight(lowerLights[i], 5, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      }
      delay(1000);
}

void upperLightsPatternWave(){
      randomLightValues();
      for(int i = 0; i < 12; i++) {
        changeLight(upperLights[i], patternTransition, "on", "true", "bri", "50", "hue", rv.col, "sat", "254");
        changeLight(upperLights[i], patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      }
      delay(1000);
}

void randomLightValues(){
  rv.col = String(random(38000, 60000));
  rv.bri = String(random(190, 254));
  rv.sat = String(random(180, 254));
  if (debug){
    Serial.println("NEW VALUES");
    Serial.println("NEW VALUES");
    Serial.println("NEW VALUES");
    Serial.println("NEW VALUES");
    Serial.println("NEW VALUES");
    Serial.println("NEW VALUES");
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
