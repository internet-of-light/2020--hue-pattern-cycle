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
int upperLights[] = {12, 5, 13, 17, 20, 26, 19, 9, 18, 25, 8, 24};


//light settings
int transitionTime = 5;
int patternTransition = 1;

typedef struct {
  String col;
  String bri;
  String sat;
} randomValues;

randomValues rv;

unsigned long currentTime, lastPattern, lightLastUpdate; //timing

bool debug = false;

void setup() {
  Serial.begin(115200);
  setup_wifi();
}

void loop() {
  currentTime = millis(); //Update time 
  iterateLights();
  if (currentTime - lightLastUpdate > 20000) {
    lowerLightsPattern();
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
      int light = random(0,9);
      int lightNum = lowerLights[light];
      changeLight(lightNum, transitionTime, "on", "true", "bri", rv.bri, "hue", rv.col, "sat", rv.sat);
}

void upperLightsCycle(){
      randomLightValues();
      int light = random(0,12);
      int lightNum = upperLights[light];
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
      changeLight(23, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      delay(400);
      changeLight(16, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      changeLight(14, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      changeLight(11, patternTransition, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      lightLastUpdate = currentTime;
      delay(5000);
      for (int i = 0; i < 9; i++){
        changeLight(lowerLights[i], 0, "on", "false", "bri", rv.bri, "hue", rv.col, "sat", "254");
      }
      for (int i = 0; i < 9; i++){
        changeLight(lowerLights[i], 5, "on", "true", "bri", "254", "hue", rv.col, "sat", "254");
      }
      delay(1000);
}

void randomLightValues(){
  rv.col = String(random(30000, 60000));
  rv.bri = String(random(190, 254));
  rv.sat = String(random(180, 254));
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
