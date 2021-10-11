#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

#define ONE_WIRE_BUS_1 2
#define ONE_WIRE_BUS_2 0

// Replace with your network credentials
const char* ssid = "yourssid";
const char* password = "yourpass";

// Initialize Telegram BOT
#define BOTtoken "1978xxxxxx:XxXxXxXxXxXxXxXxXx"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "xxxxxxxx" //your telegram ID (if needs)

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

OneWire oneWire_1(ONE_WIRE_BUS_1);
OneWire oneWire_2(ONE_WIRE_BUS_2);
    
DallasTemperature sensor_1(&oneWire_1);
DallasTemperature sensor_2(&oneWire_2); 

int Lampu_1 = LOW;
int Lampu_2 = LOW;
int Lampu_3 = LOW;
int AC_1 = LOW;
int AC_2 = LOW;
int AC_3 = LOW;
int Ruangan_1 = LOW;
int Ruangan_2 = LOW;

int t1,t2;

const byte R1 = 14; //AC3
const byte R2 = 12; //AC2
const byte R3 = 13; //AC1
const byte R4 = 16; //Lampu1
const byte R5 = 5; ///Lampu2
const byte R6 = 4; ///Lampu3

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    if (bot.messages[i].type ==  F("callback_query")) {
      String chat_id = String(bot.messages[i].chat_id);
      String text = bot.messages[i].text;
      Serial.print("Call back button pressed with text: ");
      Serial.println(text);

      if (text == F("L1_ON")) {
        Lampu_1 = HIGH;
        digitalWrite(R4, HIGH);
        String keyboardJson = F("[[{ \"text\" : \"Matikan Lampu\", \"callback_data\" : \"L1_OFF\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "Lampu 1 is ON", "", keyboardJson);
      } else if (text == F("L1_OFF")) {
        Lampu_1 = LOW;
        digitalWrite(R4, LOW);
        String keyboardJson = F("[[{ \"text\" : \"Nyalakan Lampu\", \"callback_data\" : \"L1_ON\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "Lampu 1 is OFF", "", keyboardJson);
      } else if (text == F("L2_ON")) {
        Lampu_2 = HIGH;
        digitalWrite(R5, HIGH);
        String keyboardJson = F("[[{ \"text\" : \"Matikan Lampu\", \"callback_data\" : \"L2_OFF\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "Lampu 2 is ON", "", keyboardJson);
      } else if (text == F("L2_OFF")) {
        Lampu_2 = LOW;
        digitalWrite(R5, LOW);
        String keyboardJson = F("[[{ \"text\" : \"Nyalakan Lampu\", \"callback_data\" : \"L2_ON\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "Lampu 2 is OFF", "", keyboardJson);
      } else if (text == F("L3_ON")) {
        Lampu_3 = HIGH;
        digitalWrite(R6, HIGH);
        String keyboardJson = F("[[{ \"text\" : \"Matikan Lampu\", \"callback_data\" : \"L3_OFF\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "Lampu 3 is ON", "", keyboardJson);
      } else if (text == F("L3_OFF")) {
        Lampu_3 = LOW;
        digitalWrite(R6, LOW);
        String keyboardJson = F("[[{ \"text\" : \"Nyalakan Lampu\", \"callback_data\" : \"L3_ON\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "Lampu 3 is OFF", "", keyboardJson);
      } else if (text == F("AC1_ON")) {
        AC_1 = HIGH;
        digitalWrite(R3, HIGH);
        String keyboardJson = F("[[{ \"text\" : \"Matikan AC\", \"callback_data\" : \"AC1_OFF\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "AC 1 Ruangan 1 is ON", "", keyboardJson);
      } else if (text == F("AC1_OFF")) {
        AC_1 = LOW;
        digitalWrite(R3, LOW);
        String keyboardJson = F("[[{ \"text\" : \"Nyalakan AC\", \"callback_data\" : \"AC1_ON\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "AC 1 Ruangan 1 is OFF", "", keyboardJson);
      } else if (text == F("AC2_ON")) {
        AC_2 = HIGH;
        digitalWrite(R2, HIGH);
        String keyboardJson = F("[[{ \"text\" : \"Matikan AC\", \"callback_data\" : \"AC2_OFF\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "AC 2 Ruangan 1 is ON", "", keyboardJson);
      } else if (text == F("AC2_OFF")) {
        AC_2 = LOW;
        digitalWrite(R2, LOW);
        String keyboardJson = F("[[{ \"text\" : \"Nyalakan AC\", \"callback_data\" : \"AC2_ON\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "AC 1 Ruangan 1 is OFF", "", keyboardJson);
      } else if (text == F("AC3_ON")) {
        AC_3 = HIGH;
        digitalWrite(R1, HIGH);
        String keyboardJson = F("[[{ \"text\" : \"Matikan AC\", \"callback_data\" : \"AC3_OFF\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "AC Ruangan 2 is ON", "", keyboardJson);
      } else if (text == F("AC3_OFF")) {
        AC_3 = LOW;
        digitalWrite(R1, LOW);
        String keyboardJson = F("[[{ \"text\" : \"Nyalakan AC\", \"callback_data\" : \"AC3_ON\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "AC Ruangan 2 is OFF", "", keyboardJson);
      } else if (text == F("OTOMATIS_R1_ON")) {
        Ruangan_1 = HIGH;
        String keyboardJson = F("[[{ \"text\" : \"Matikan Kendali Otomatis\", \"callback_data\" : \"OTOMATIS_R1_OFF\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "Kendali Otomatis Ruangan 1 is Active", "", keyboardJson);
      } else if (text == F("OTOMATIS_R1_OFF")) {
        Ruangan_1 = LOW;
        String keyboardJson = F("[[{ \"text\" : \"Nyalakan Kendali Otomatis\", \"callback_data\" : \"OTOMATIS_R1_ON\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "Kendali Otomatis Ruangan 1 is Nonactive", "", keyboardJson);
      } else if (text == F("OTOMATIS_R2_ON")) {
        Ruangan_2 = HIGH;
        String keyboardJson = F("[[{ \"text\" : \"Matikan Kendali Otomatis\", \"callback_data\" : \"OTOMATIS_R2_OFF\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "Kendali Otomatis Ruangan 2 is Active", "", keyboardJson);
      } else if (text == F("OTOMATIS_R2_OFF")) {
        Ruangan_2 = LOW;
        String keyboardJson = F("[[{ \"text\" : \"Nyalakan Kendali Otomatis\", \"callback_data\" : \"OTOMATIS_R2_ON\" }]]");
        bot.sendMessageWithInlineKeyboard(chat_id, "Kendali Otomatis Ruangan 2 is Nonactive", "", keyboardJson);
      }
    }else{
      String chat_id = String(bot.messages[i].chat_id);
//      if (chat_id != CHAT_ID){
//        bot.sendMessage(chat_id, "Unauthorized user", "");
//        continue;
//      }
      
      // Print the received message
      String text = bot.messages[i].text;
      Serial.println(text);
  
      String from_name = bot.messages[i].from_name;
  
      if (text == "/LAMPU1") {
        if(Lampu_1 == LOW){
          String keyboardJson = F("[[{ \"text\" : \"Nyalakan Lampu\", \"callback_data\" : \"L1_ON\" }]]");
          bot.sendMessageWithInlineKeyboard(chat_id, "Lampu 1 Mati\nNyalakan Lampu?", "", keyboardJson);
        }else{
          String keyboardJson = F("[[{ \"text\" : \"Matikan Lampu\", \"callback_data\" : \"L1_OFF\" }]]");
          bot.sendMessageWithInlineKeyboard(chat_id, "Lampu 1 Nyala \nMatikan Lampu?", "", keyboardJson);
        }
      }
      if (text == "/LAMPU2") {
        if(Lampu_2 == LOW){
          String keyboardJson = F("[[{ \"text\" : \"Nyalakan Lampu\", \"callback_data\" : \"L2_ON\" }]]");
          bot.sendMessageWithInlineKeyboard(chat_id, "Lampu 2 Mati \nNyalakan Lampu?", "", keyboardJson);
        }else{
          String keyboardJson = F("[[{ \"text\" : \"Matikan Lampu\", \"callback_data\" : \"L2_OFF\" }]]");
          bot.sendMessageWithInlineKeyboard(chat_id, "Lampu 2 Nyala \nMatikan Lampu?", "", keyboardJson);
        }
      }
      if (text == "/LAMPU3") {
        if(Lampu_3 == LOW){
          String keyboardJson = F("[[{ \"text\" : \"Nyalakan Lampu\", \"callback_data\" : \"L3_ON\" }]]");
          bot.sendMessageWithInlineKeyboard(chat_id, "Lampu 3 Mati\nNyalakan Lampu?", "", keyboardJson);
        }else{
          String keyboardJson = F("[[{ \"text\" : \"Matikan Lampu\", \"callback_data\" : \"L3_OFF\" }]]");
          bot.sendMessageWithInlineKeyboard(chat_id, "Lampu 3 Nyala \nMatikan Lampu?", "", keyboardJson);
        }
      }

      if (text == "RUANGAN1") {
          if(Ruangan_1 == LOW){
            String ket = "-- **RUANGAN 1** --\n\nSuhu Ruangan : " + String(t1) + "°C\n\nKendali Otomatis Nonaktif\nNyalakan Kendali Otomatis?";
            String keyboardJson = F("[[{ \"text\" : \"Nyalakan Kendali Otomatis\", \"callback_data\" : \"OTOMATIS_R1_ON\" }],");
            if(AC_1 == LOW && AC_2 == LOW){
              keyboardJson += F("[{ \"text\" : \"Nyalakan AC 1\", \"callback_data\" : \"AC1_ON\" }, { \"text\" : \"Nyalakan AC 2\", \"callback_data\" : \"AC2_ON\" }],");
            }else if(AC_1 == LOW && AC_2 == HIGH){
              keyboardJson += F("[{ \"text\" : \"Nyalakan AC 1\", \"callback_data\" : \"AC1_ON\" }, { \"text\" : \"Matikan AC 2\", \"callback_data\" : \"AC2_OFF\" }],");
            }else if(AC_1 == HIGH && AC_2 == LOW){
              keyboardJson += F("[{ \"text\" : \"Matikan AC 1\", \"callback_data\" : \"AC1_OFF\" }, { \"text\" : \"Nyalakan AC 2\", \"callback_data\" : \"AC2_ON\" }],");
            }else {
              keyboardJson += F("[{ \"text\" : \"Matikan AC 1\", \"callback_data\" : \"AC1_OFF\" }, { \"text\" : \"Matikan AC 2\", \"callback_data\" : \"AC2_OFF\" }],");
            }
            if(Lampu_1 == LOW && Lampu_2 == LOW){
              keyboardJson += F("[{ \"text\" : \"Nyalakan Lampu 1\", \"callback_data\" : \"L1_ON\" }, { \"text\" : \"Nyalakan Lampu 2\", \"callback_data\" : \"L2_ON\" }]]");
            }else if(Lampu_1 == LOW && Lampu_2 == HIGH){
              keyboardJson += F("[{ \"text\" : \"Nyalakan Lampu 1\", \"callback_data\" : \"L1_ON\" }, { \"text\" : \"Matikan Lampu 2\", \"callback_data\" : \"L2_OFF\" }]]");
            }else if(Lampu_1 == HIGH && Lampu_2 == LOW){
              keyboardJson += F("[{ \"text\" : \"Matikan Lampu 1\", \"callback_data\" : \"L1_OFF\" }, { \"text\" : \"Nyalakan Lampu 2\", \"callback_data\" : \"L2_ON\" }]]");
            }else {
              keyboardJson += F("[{ \"text\" : \"Matikan Lampu 1\", \"callback_data\" : \"L1_OFF\" }, { \"text\" : \"Matikan Lampu 2\", \"callback_data\" : \"L2_OFF\" }]]");
            }
            bot.sendMessageWithInlineKeyboard(chat_id, ket, "", keyboardJson);
          }else{
            String keyboardJson = F("[[{ \"text\" : \"Matikan Kendali Otomatis\", \"callback_data\" : \"OTOMATIS_R1_OFF\" }],");
            if(Lampu_1 == LOW && Lampu_2 == LOW){
              keyboardJson += F("[{ \"text\" : \"Nyalakan Lampu 1\", \"callback_data\" : \"L1_ON\" }, { \"text\" : \"Nyalakan Lampu 2\", \"callback_data\" : \"L2_ON\" }]]");
            }else if(Lampu_1 == LOW && Lampu_2 == HIGH){
              keyboardJson += F("[{ \"text\" : \"Nyalakan Lampu 1\", \"callback_data\" : \"L1_ON\" }, { \"text\" : \"Matikan Lampu 2\", \"callback_data\" : \"L2_OFF\" }]]");
            }else if(Lampu_1 == HIGH && Lampu_2 == LOW){
              keyboardJson += F("[{ \"text\" : \"Matikan Lampu 1\", \"callback_data\" : \"L1_OFF\" }, { \"text\" : \"Nyalakan Lampu 2\", \"callback_data\" : \"L2_ON\" }]]");
            }else {
              keyboardJson += F("[{ \"text\" : \"Matikan Lampu 1\", \"callback_data\" : \"L1_OFF\" }, { \"text\" : \"Matikan Lampu 2\", \"callback_data\" : \"L2_OFF\" }]]");
            }
            bot.sendMessageWithInlineKeyboard(chat_id, "-- **RUANGAN 1** --\n\nSuhu Ruangan : " + String(t1) + "°C\n\nKendali Otomatis Aktif\nMatikan Kendali Otomatis?", "", keyboardJson);
        }
      }

      if (text == "RUANGAN2") {
          if(Ruangan_2 == LOW){
            String ket = "-- **RUANGAN 2** --\n\nSuhu Ruangan : " + String(t2) + "°C\n\nKendali Otomatis Nonaktif\nNyalakan Kendali Otomatis?";
            String keyboardJson = F("[[{ \"text\" : \"Nyalakan Kendali Otomatis\", \"callback_data\" : \"OTOMATIS_R2_ON\" }],");
            if(AC_3 == LOW){
              keyboardJson += F("[{ \"text\" : \"Nyalakan AC\", \"callback_data\" : \"AC3_ON\" }],");
            }else {
              keyboardJson += F("[{ \"text\" : \"Matikan AC\", \"callback_data\" : \"AC3_OFF\" }],");
            }
            if(Lampu_3 == LOW){
              keyboardJson += F("[{ \"text\" : \"Nyalakan Lampu\", \"callback_data\" : \"L3_ON\" }]]");
            }else {
              keyboardJson += F("[{ \"text\" : \"Matikan Lampu\", \"callback_data\" : \"L3_OFF\" }]]");
            }
            bot.sendMessageWithInlineKeyboard(chat_id, ket, "", keyboardJson);
          }else{
            String keyboardJson = F("[[{ \"text\" : \"Matikan Kendali Otomatis\", \"callback_data\" : \"OTOMATIS_R2_OFF\" }]]");
            bot.sendMessageWithInlineKeyboard(chat_id, "-- **RUANGAN 2** --\n\nSuhu Ruangan : " + String(t2) + "°C\n\nKendali Otomatis Aktif\nMatikan Kendali Otomatis?", "", keyboardJson);
        }
      }
   
      if (text == "/onALL") {
         digitalWrite(R1, HIGH);digitalWrite(R2, HIGH);
         digitalWrite(R3, HIGH);digitalWrite(R4, HIGH);
         digitalWrite(R5, HIGH);digitalWrite(R6, HIGH);
        bot.sendMessage(chat_id, "all relay on", "");
      }
   
      if (text == "/offALL") {
         digitalWrite(R1, LOW);digitalWrite(R2, LOW);
         digitalWrite(R3, LOW);digitalWrite(R4, LOW);
         digitalWrite(R5, LOW);digitalWrite(R6, LOW);
        bot.sendMessage(chat_id, "all relay on", "");
      }
   
      if (text == "/options") {
        String keyboardJson = "[[\"RUANGAN1\", \"RUANGAN2\"],";
//               keyboardJson += "[\"/LAMPU1\", \"/LAMPU2\",\"/LAMPU3\"],";
               keyboardJson += "[\"STATUS\"]]";
        bot.sendMessageWithReplyKeyboard(chat_id, "Choose from one of the following options", "", keyboardJson, true);
      }
   
      if (text == "/start") {
        String welcome = "**Telegram Bot Pengendali Beban Ruangan Dosen Teknik Mesin PNUP**\n\n**Selamat Datang**, " + from_name + ".\n\n";
        welcome += "/options : untuk menampilkan pilihan\n";
        bot.sendMessage(chat_id, welcome, "Markdown");
      } 
      if (text == "STATUS") {
        String Status_Lampu_1;
        String Status_Lampu_2;
        String Status_Lampu_3;
        String Status_AC_1;
        String Status_AC_2;
        String Status_AC_3;
        
        if (Lampu_1 == HIGH) {
          Status_Lampu_1 = "Lampu 1 ON";
        } else {
          Status_Lampu_1 = "Lampu 1 OFF";
        }
        if (Lampu_2 == HIGH) {
          Status_Lampu_2 = "Lampu 2 ON";
        } else {
          Status_Lampu_2 = "Lampu 2 OFF";
        }
        if (Lampu_3 == HIGH) {
          Status_Lampu_3 = "Lampu ON";
        } else {
          Status_Lampu_3 = "Lampu OFF";
        }
        if (AC_1 == HIGH) {
          Status_AC_1 = "AC 1 ON";
        } else {
          Status_AC_1 = "AC 1 OFF";
        }
        if (AC_2 == HIGH) {
          Status_AC_2 = "AC 2 ON";
        } else {
          Status_AC_2 = "AC 2 OFF";
        }
        if (AC_3 == HIGH) {
          Status_AC_3 = "AC ON";
        } else {
          Status_AC_3 = "AC OFF";
        }
        String statuss = "**Keadaan Ruangan**\n\n__Ruangan 1__\nSuhu Ruangan : " + String(t1) + "°C\n" + Status_Lampu_1 + "\n" + Status_Lampu_2 + "\n" + Status_AC_1 + "\n" + Status_AC_2 + "\n\n__Ruangan 2__\nSuhu Ruangan : " + String(t2) + "°C\n" + Status_Lampu_3 + "\n" + Status_AC_3;
        bot.sendMessage(chat_id, statuss, "Markdown");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(R5, OUTPUT);
  pinMode(R6, OUTPUT);

  sensor_1.begin();
  sensor_2.begin();

//  digitalWrite(R1, HIGH);
//  digitalWrite(R2, HIGH);
//  digitalWrite(R3, HIGH);
//  digitalWrite(R4, HIGH);
//  digitalWrite(R5, HIGH);
//  digitalWrite(R6, HIGH);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  // Print ESP32 Local IP Address
  Serial.println("\nTerhubung...");
  Serial.print("WiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
  bot.sendMessage(CHAT_ID, "Terhubung...", "Markdown");
}

void loop() {
  sensor_1.requestTemperatures();
  sensor_2.requestTemperatures();
  t1 = sensor_1.getTempCByIndex(0);
  t2 = sensor_2.getTempCByIndex(0);
   
  if ( millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

  if(Ruangan_1 == HIGH){
    ruanganIotomatis();
  }

  if(Ruangan_2 == HIGH){
    ruanganIIotomatis();
  }
}

void ruanganIotomatis(){
  if(t1<=28){
    digitalWrite(R3, LOW);
    AC_1 = LOW;
    digitalWrite(R2, LOW);
    AC_2 = LOW;
  }else{
    digitalWrite(R3, HIGH);
    AC_1 = HIGH;
    digitalWrite(R2, HIGH);
    AC_2 = HIGH;
  }
}
void ruanganIIotomatis(){
  if(t2<=28){
    digitalWrite(R1, LOW);
    AC_3 = LOW;
  }else{
    digitalWrite(R1, HIGH);
    AC_3 = HIGH;
  }
}
