#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <MQ2.h>
#include "ArduinoJson.h"

CTBotReplyKeyboard tbl;

const char* WIFI_SSID = "yourssid";
const char* WIFI_PASSWORD = "yourpass";

const char* MQTT_USER = "yourMQTTuser"; 
const char* MQTT_PASSWORD = "yourMQTTpass"; 

const char* MQTT_SERVER = "your.mqtt.address";
const long MQTT_PORT = yourmqttport;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

int buzz = D1;
int fan = D2;
int sensApi = D0;
int sensAsap = A0;

int lpg, co, smoke, sensorValue;

MQ2 mq2(sensAsap);

void setup(){
  setupWifi();
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  pinMode(sensApi, INPUT);
  pinMode(buzz, OUTPUT);
  mq2.begin();
  Serial.begin(9600);
}

void loop() {
  float* sensAsapValue = mq2.read(true);
  lpg = mq2.readLPG();
  co = mq2.readCO();
  smoke = mq2.readSmoke();
  sensorValue = digitalRead(sensApi);
  
  Serial.println(lpg);
  Serial.println(co);
  Serial.print(smoke);
  Serial.print("\n");
  Serial.print(sensorValue);
  Serial.print("\n");
  alarm();
  sendMQTT();
  delay(500);
}

void alarm(){
  if(sensorValue == 0){
    digitalWrite(buzz, HIGH);
    delay(3000);
  }else if(co>100 || lpg>100 || smoke>100){
    digitalWrite(fan, HIGH);
    delay(3000);
  }else if(co>100 || lpg>100 || smoke>100 && sensorValue ==0){
    digitalWrite(fan, HIGH);
    digitalWrite(buzz, HIGH);
  }else{
    digitalWrite(buzz, LOW);
    digitalWrite(fan, LOW);
  }
}

void sendMQTT(){
  if (!mqttClient.connected()) {
    mqttReconnect();
  }
  DynamicJsonDocument doc(2048);
  doc["lpg"] = lpg;
  doc["co"] = co;
  doc["smoke"] = smoke;
  doc["api"] = sensorValue;
  String jsonString;
  serializeJson(doc,jsonString);
  Serial.println(jsonString);
  Serial.print("\n");
  mqttClient.publish("monitoring",jsonString.c_str());
  delay(1000*5);
  mqttClient.loop();
}

void setupWifi() {
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void mqttReconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("Tes",MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      mqttClient.subscribe("monitoring"); 
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
