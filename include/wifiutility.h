#ifndef _WIFI_U_H
#define _WIFI_U_H
#include <Arduino.h>
#include <WiFi.h>
#include <dhcpserver/dhcpserver.h>

String translateEncryptionType(wifi_auth_mode_t encryptionType) {
  switch (encryptionType) {
    case (WIFI_AUTH_OPEN):
      return "Open";
    case (WIFI_AUTH_WEP):
      return "WEP";
    case (WIFI_AUTH_WPA_PSK):
      return "WPA_PSK";
    case (WIFI_AUTH_WPA2_PSK):
      return "WPA2_PSK";
    case (WIFI_AUTH_WPA_WPA2_PSK):
      return "WPA_WPA2_PSK";
    case (WIFI_AUTH_WPA2_ENTERPRISE):
      return "WPA2_ENTERPRISE";
    case (WIFI_AUTH_MAX):
        return "WIFI_AUTH_MAX";
  }
  return "FAIL";
}

const char* ssid = "MyFy";
const char* password =  "edog0049a";

void setWifiAP(){
    WiFi.mode(WIFI_MODE_AP);
}

void setWifiSTA(){
    WiFi.mode(WIFI_MODE_STA);
}

void setWifiAP_STA(){
     WiFi.mode(WIFI_MODE_APSTA);
}
 
void scanNetworks() {
  int16_t numberOfNetworks = WiFi.scanNetworks();
  Serial.print("Number of networks found: ");
  Serial.println(numberOfNetworks);
 
  for (int8_t i = 0; i < numberOfNetworks; i++) {
 
    Serial.print("Network name: ");
    Serial.println(WiFi.SSID(i));
 
    Serial.print("Signal strength: ");
    Serial.println(WiFi.RSSI(i));
 
    Serial.print("MAC address: ");
    Serial.println(WiFi.BSSIDstr(i));
 
    Serial.print("Encryption type: ");
    String encryptionTypeDescription = translateEncryptionType(WiFi.encryptionType(i));
    Serial.println(encryptionTypeDescription);
    Serial.println("-----------------------");
  }
}
 
void connectToNetwork() {
  setWifiSTA();
  tcpip_adapter_dhcpc_start(TCPIP_ADAPTER_IF_AP);
  WiFi.begin(ssid, password);
  int8_t tries = 0;
  while (!WiFi.isConnected()) {
    if(tries>20){
        Serial.println("connection to WiFi FAILED!");
        return;
    }
    delay(1000);
    Serial.println("Establishing connection to WiFi..");
    tries++;
  }
  Serial.println("Connected to network");
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());
  if(WiFi.getMode() == WIFI_AP_STA)
    Serial.println(WiFi.softAPIP());
}

#endif