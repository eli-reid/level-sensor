#ifndef COMMANDS_H
#define COMMANDS_H

#include "webserver.h"
#include "wifiutility.h"
#include "filesystem.h"
#include "config.h"

enum command_e{
   CMD_SAVE_CONFIG,
   CMD_SAVE_IP_CONFIG,
   CMD_GET_CONFIG,
   CMD_GET_WIFI,
   CMD_CON_WIFI
};

//deserialize json char array into jsondocument 
void updateIpConfig(StaticJsonDocument<JSON_DOC_SIZE>& doc, const char* jsonData){
    if(strlen(jsonData)>0){
        deserializeJson(doc, jsonData, DeserializationOption::NestingLimit(15));            
        updateNetworkConfig(doc);
    }
}

//send event 'config' with config JSON as a string 
void sendConfig(StaticJsonDocument<JSON_DOC_SIZE>& doc){
   //Gets Updated config and serializes it to string then sends a config event
   updateNetworkConfig(doc);
   String json;
   serializeJson(doc, json);
   events.send(json.c_str(),"config", millis());
}

// called from onCMD event, call proper functions for each commmand
void CommandHandler(AsyncWebSocketClient *client,const char* type, int cmd, const char* txt, StaticJsonDocument<JSON_DOC_SIZE>& doc){
  const bool post = !strcmp(type,"POST");
  const bool get = !strcmp(type,"GET");
  if (post == get){
      Serial.println("COMMAND FAIL!");
  }
  else{
      switch (cmd){
         case CMD_SAVE_IP_CONFIG:
            if(post)
               updateIpConfig(doc, txt);
            break;            
         case CMD_GET_CONFIG:
            if(get)
               sendConfig(doc);
            break;
         case CMD_GET_WIFI:
            if(get)
               scanWifi();
            break;
         default:
            Serial.println("COMMAND FAIL!");
            break;
      }
   }
}
#endif 