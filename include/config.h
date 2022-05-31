#ifndef __CONFIG_H
#define __CONFIG_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>
/*
    *************************************************************************************************************************************
    |                                                 Config Handler                                                                    |
    *************************************************************************************************************************************
*/

StaticJsonDocument<2048> doc; 

void loadConfig(){

   File config = SPIFFS.open("/config.cfg");
   String d = "";
   while(config.available()){
      //Serial.write(file.read));
      d += config.readString();
   }
   deserializeJson(doc, d, DeserializationOption::NestingLimit(15));   

   config.close();
}

void saveConfig(){
   File config = SPIFFS.open("/config.cfg","w");
   serializeJsonPretty(doc,config);
   config.close();
}

void increamentBootCount(){
    loadConfig();
    int bootcnt = doc["START_UP"]["BOOT_COUNT"];
    bootcnt += 1;
    doc["START_UP"]["BOOT_COUNT"] = bootcnt;
    saveConfig();
}

int getBootCount(){
    loadConfig();
    return doc["START_UP"]["BOOT_COUNT"];
}
float getSensorCal(){
        loadConfig();
    return doc["SENSOR"]["CALIBRATION"];
}

#endif
