#ifndef __FILESYS_H
#define __FILESYS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>

void startFileSystem(){   
    if(!SPIFFS.begin(false)){
        Serial.println("SPIFFS FAILED!!");
    }    
}

void readFile(fs::FS &fs, const char * path){
   Serial.printf("Reading file: %s\r\n", path);

   File file = fs.open(path);
   if(!file || file.isDirectory()){
       Serial.println("− failed to open file for reading");
       return;
   }

   Serial.println("− read from file:");
   String d="";
   while(file.available()){
      //Serial.write(file.read));
      d += file.readString();
   }
   Serial.print(d);
}

void writeFile(fs::FS &fs, const char * path, const char * message){
   Serial.printf("Writing file: %s\r\n", path);

   File file = fs.open(path, FILE_WRITE);
   if(!file){
      Serial.println("− failed to open file for writing");
      return;
   }
   if(file.print(message)){
      Serial.println("− file written");
   }else {
      Serial.println("− frite failed");
   }
}

/*
    *************************************************************************************************************************************
    |                                                 Config Handler                                                                    |
    *************************************************************************************************************************************
*/
// add injection of StaticJson Document to loadconfig and saveconfig
/* 

void loadConfig(){
   File config = SPIFFS.open("/config.cfg");
   String d = "";
   while(config.available()){
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
} */
#endif