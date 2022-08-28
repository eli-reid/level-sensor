#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "sensor.h"
#include "webserver.h"
#include "wifiutility.h"
#include "filesystem.h"

 void startSerial(){
    Serial.begin(115200); 
}

void CommandHandler(AsyncWebSocketClient *client, char* cmd, char* type, char* txt){
   if(strcmp(cmd,"GET")==0){
      client->text("I'm Getting your commands!");
      loadConfig();
      IPAddress lip = WiFi.localIP();
       Serial.println(lip.toString());
      auto r = doc["WIFI_STA_CONFIG"]["IP_ADDR"].as<String>();
      Serial.println(r);
      if(doc["WIFI_STA_CONFIG"]["DCHP_ENABLED"]){
         Serial.println("Updating network values!");
         doc["WIFI_STA_CONFIG"]["IP_ADDR"]=WiFi.localIP();
         doc["WIFI_STA_CONFIG"]["SUBNET"]=WiFi.subnetMask();
         doc["WIFI_STA_CONFIG"]["DEFAULT_GATEWAY"]=WiFi.gatewayIP();
         doc["WIFI_STA_CONFIG"]["DNS_SERVER1"]=WiFi.dnsIP(0);
         doc["WIFI_STA_CONFIG"]["DNS_SERVER2"]=WiFi.dnsIP(1);
         saveConfig();
      }
      char data[1024];
      serializeJson(doc,data);
      Serial.print("sending config!");
      events.send(data,"config", millis());
   }
   if(strcmp(cmd,"POST")==0){
      if(strcmp(type,"saveConfig")==0){
         Serial.println(txt);
         deserializeJson(doc, txt,DeserializationOption::NestingLimit(15)); 
         saveConfig();
      }
      client->text("I'm Posting your commands!");
      }
}
