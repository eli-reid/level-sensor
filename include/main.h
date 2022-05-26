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
   client->text("I'm handling your commands!");
}
