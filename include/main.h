#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include "sensor.h"
#include "webserver.h"
#include "wifiutility.h"
#include "filesystem.h"
#include "config.h"
#include "commands.h"
static const char *TAG = "Level_Sensor";

//doc and JSONstring used for JSON as global memory allocations 
StaticJsonDocument<JSON_DOC_SIZE> doc; 
String JSONstring;

void onDistance(float dist){
    std::stringstream sstream;
    sstream << dist + getSensorCal(doc);
    auto distance = sstream.str();
    events.send(distance.c_str(),"newdistance",millis());
}

void onCommand(AsyncWebSocketClient *client, const char* type, int cmd, const char* txt){
   CommandHandler(client,type,cmd,txt,doc);
}

void onWifiScanComlpeted(){
    if(WiFi.scanComplete() == -2)
        scanWifi();
    else{
        esp_timer_stop(wifiScanTimer);
        events.send(scanCompleted().c_str(),"wifilist",millis());
    }
}

void startSerial(){
    Serial.begin(115200); 
}