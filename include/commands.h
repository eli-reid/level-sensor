#ifndef COMMANDS_H
#define COMMANDS_H

#include "webserver.h"
#include "wifiutility.h"
#include "filesystem.h"
#include "config.h"

typedef enum command{
   CMD_SAVE_CONFIG,
   CMD_SAVE_IP_CONFIG,
   CMD_GET_CONFIG,
   CMD_GET_WIFI,
   CMD_CON_WIFI
};

void CommandHandler(AsyncWebSocketClient *client, char* type, int cmd, char* txt, StaticJsonDocument<JSON_DOC_SIZE>& doc){
  const bool post = !strcmp(type,"POST");
  const bool get = !strcmp(type,"GET");
  if ( post + get != 1){
   
  }
  else{
      switch (cmd){
         case CMD_SAVE_IP_CONFIG:
            if(post){
               updateIpConfig(doc, txt);
               break;            
            }
            
         case CMD_GET_CONFIG:
            if(get){
               sendConfig(doc);
               break;
            }

         case CMD_GET_WIFI:
            if(get){
                break;
            }
           
            default:
               Serial.println("COMMAND FAIL!");
               break;
      };
   }
}

void updateIpConfig(StaticJsonDocument<JSON_DOC_SIZE>& doc, char* jsonData){
    if(strlen(jsonData)>0){
        deserializeJson(doc, jsonData, DeserializationOption::NestingLimit(15));            
        updateNetworkConfig(doc);
    }
}

void sendConfig(StaticJsonDocument<JSON_DOC_SIZE>& doc){
   //Gets Updated config and serializes it to string then sends a config event
   updateNetworkConfig(doc);
   char json[JSON_DOC_SIZE];
   serializeJson(doc, json);
   events.send(json,"config", millis());

}


#endif 
