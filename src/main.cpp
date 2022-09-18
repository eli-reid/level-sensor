#include "main.h"

void setup() {
    esp_netif_init();
    startSerial();
    startDistanceTimer();
    startFileSystem();
    startHttpServer();
    increamentBootCount(doc);

    connectToNetwork(doc["WIFI_STA_CONFIG"]["SSID"], doc["WIFI_STA_CONFIG"]["PASSWORD"]);
   
    //Set Callback Functions
    onCmd = onCommand;
    onNewDistance = onDistance;
    onWifiScanComplete = onWifiScanComlpeted;

    //log output 
    printf("Current Boot Count: %i \n", getBootCount(doc));
}

void loop() {  
    webSocket.cleanupClients();
    delay(100);
}
