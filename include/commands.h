#ifndef COMMANDS_H
#define COMMANDS_H

#include "webserver.h"
#include "wifiutility.h"
#include "filesystem.h"
#include "config.h"

enum command{
   CMD_SAVE_CONFIG,
   CMD_SAVE_IP_CONFIG,
   CMD_GET_CONFIG,
   CMD_GET_WIFI,
   CMD_CON_WIFI
};

//********************command functions**********************


//********************config functions**********************

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

//********************WIFI functions**********************

//onWifiscanComplete is an event function that is empty till implented
void (*onWifiScanComplete)() = [](){/*Place holder Lambda*/}; 

// function called using esp timer API 
void _checkWifiScanComplete(void* args){
   if(WiFi.scanComplete()>-1)
      onWifiScanComplete();
}

//Setup timer callback function
const esp_timer_create_args_t wifiScanTimerArgs = { .callback = &_checkWifiScanComplete };
esp_timer_handle_t wifiScanTimer;

//scans for wifi and start async timer to check if scanComplete
void scanWifi(){
   if(WiFi.scanComplete() == -2){
      WiFi.scanNetworks(true);
   }
   esp_timer_create(&wifiScanTimerArgs, &wifiScanTimer);
   esp_timer_start_periodic(wifiScanTimer, 1000);
}

//********************end of command functions**********************

// called from onCMD event, call proper functions for each commmand
void CommandHandler(AsyncWebSocketClient *client, char* type, int cmd, char* txt, StaticJsonDocument<JSON_DOC_SIZE>& doc){
  const bool post = !strcmp(type,"POST");
  const bool get = !strcmp(type,"GET");
  if (post == get){
      Serial.println("COMMAND FAIL!");
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
               scanWifi();
               break;
            }
           
            default:
               Serial.println("COMMAND FAIL!");
               break;
      };
   }
}


#endif 