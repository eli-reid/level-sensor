#ifndef _WIFI_U_H
#define _WIFI_U_H
#include <Arduino.h>
#include <WiFi.h>
#include <string.h>
#include <map>
#include <dhcpserver/dhcpserver.h>

String translateEncryptionType(wifi_auth_mode_t encryptionType) {
  std::map<int, String> wifiEncryptionType{
    {WIFI_AUTH_OPEN, "Open"}, {WIFI_AUTH_WEP,"WEP"},
    {WIFI_AUTH_WPA_PSK, "WPA_PSK"}, {WIFI_AUTH_WPA2_PSK,"WPA2_PSK"},
    {WIFI_AUTH_WPA_WPA2_PSK, "WPA_WPA2_PSK"}, {WIFI_AUTH_WPA2_ENTERPRISE, "WPA2_ENTERPRISE"},
    {WIFI_AUTH_MAX, "WIFI_AUTH_MAX"}
  };  
  if (encryptionType >= wifiEncryptionType.size())
    return "Invalid Type";
  return wifiEncryptionType[encryptionType];
}

void setWifiAP(){
    WiFi.mode(WIFI_MODE_AP);
}

void setWifiSTA(){
    WiFi.mode(WIFI_MODE_STA);
}

void setWifiAP_STA(){
     WiFi.mode(WIFI_MODE_APSTA);
}
 
void connectToNetwork(const char *ssid, const char *password) {
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

//onWifiscanComplete is an event function that is empty till implented
void (*onWifiScanComplete)() = [](){/*Place holder Lambda*/}; 
esp_timer_handle_t wifiScanTimer;
void _checkWifiScanComplete(void *args){
   if(WiFi.scanComplete()>-1){
      esp_timer_stop(wifiScanTimer);
      onWifiScanComplete();
      }
}
//Setup timer callback function
const esp_timer_create_args_t wifiScanTimerArgs = { .callback = &_checkWifiScanComplete };


//scans for wifi and start async timer to check if scanComplete
void scanWifi(){
   if(WiFi.scanComplete() == -2){
      WiFi.scanNetworks(true);
   }
   esp_timer_create(&wifiScanTimerArgs, &wifiScanTimer);
   esp_timer_start_periodic(wifiScanTimer, 50000);
}

// function called using esp timer API 



//Builds json string of avalible wifi networks
String scanCompleted(){
  String json = "[";
  for (int i = 0; i < WiFi.scanComplete(); ++i){
    if(i) json += ",";
    json += "{";
    json += "\"rssi\":"+String(WiFi.RSSI(i));
    json += ",\"ssid\":\""+WiFi.SSID(i)+"\"";
    json += ",\"bssid\":\""+WiFi.BSSIDstr(i)+"\"";
    json += ",\"channel\":"+String(WiFi.channel(i));
    json += ",\"secure\":"+String(WiFi.encryptionType(i));
    json += "}";
  }
  json += "]";
  WiFi.scanDelete();
  return json;
}
#endif