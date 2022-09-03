#ifndef __CONFIG_H
#define __CONFIG_H

#ifndef JSON_DOC_SIZE
#define JSON_DOC_SIZE 1024
#endif

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>
#include "wifiutility.h"

/*
    *************************************************************************************************************************************
    |                                                 Config Handler                                                                    |
    *************************************************************************************************************************************
*/

void loadConfig(StaticJsonDocument<JSON_DOC_SIZE>& doc){
   File config = SPIFFS.open("/config.cfg");
   String data = "";
   while(config.available()){
      data += config.readString();
   }
   deserializeJson(doc, data, DeserializationOption::NestingLimit(15));   
   config.close();
}

void saveConfig(StaticJsonDocument<JSON_DOC_SIZE>& doc){
   File config = SPIFFS.open("/config.cfg","w");
   serializeJsonPretty(doc, config);
   config.close();
}

void increamentBootCount(StaticJsonDocument<JSON_DOC_SIZE>& doc){
    loadConfig(doc);
    int bootcnt = doc["START_UP"]["BOOT_COUNT"];
    bootcnt += 1;
    doc["START_UP"]["BOOT_COUNT"] = bootcnt;
    saveConfig(doc);
}

int getBootCount(StaticJsonDocument<JSON_DOC_SIZE>& doc){
    loadConfig(doc);
    return doc["START_UP"]["BOOT_COUNT"];
}

float getSensorCal(StaticJsonDocument<JSON_DOC_SIZE>& doc){
        loadConfig(doc);
    return doc["SENSOR"]["CALIBRATION"];
}

void updateNetworkConfig(StaticJsonDocument<JSON_DOC_SIZE>& doc){
    IPAddress lip = WiFi.localIP();
    if(doc["WIFI_STA_CONFIG"]["DCHP_ENABLED"] && 
    strcmp(doc["WIFI_STA_CONFIG"]["IP_ADDR"].as<char*>(),lip.toString().c_str()))
    {
        Serial.println("Updating network values!");
        doc["WIFI_STA_CONFIG"]["IP_ADDR"]=WiFi.localIP();
        doc["WIFI_STA_CONFIG"]["SUBNET"]=WiFi.subnetMask();
        doc["WIFI_STA_CONFIG"]["DEFAULT_GATEWAY"]=WiFi.gatewayIP();
    }

    if(doc["WIFI_STA_CONFIG"]["AUTO_DNS"]){
        doc["WIFI_STA_CONFIG"]["DNS_SERVER1"]=WiFi.dnsIP(0);
        doc["WIFI_STA_CONFIG"]["DNS_SERVER2"]=WiFi.dnsIP(1);
    }

    saveConfig(doc);
   }

#endif
