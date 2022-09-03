#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "sensor.h"
#include "webserver.h"
#include "wifiutility.h"
#include "filesystem.h"
#include "config.h"
#include "commands.h"
//doc and JSONstring used for JSON as global memory allocations 
StaticJsonDocument<JSON_DOC_SIZE> doc; 
char JSONstring[JSON_DOC_SIZE];

void startSerial(){
    Serial.begin(115200); 
}

void onCommand(AsyncWebSocketClient *client, char* type, int cmd, char* txt){
   CommandHandler(client,type,cmd,txt,doc);
}
