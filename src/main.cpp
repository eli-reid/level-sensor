#include "main.h"
int prevClientCount = 0;
int clientCount = 0;

void setup() {
    esp_netif_init();
    startSerial();
    startDistanceTimer();
    startFileSystem();
    startHttpServer();
    increamentBootCount(doc);
    connectToNetwork("MyFy","edog0049a");

    //Callback Functions
    onCmd = onCommand;
    onNewDistance = onDistance;
    onWifiScanComplete = onWifiScanComlpeted;

    //log output 
    Serial.print("Current Boot Count: ");
    Serial.println(getBootCount(doc));
    Serial.print("Current Sensor cal: ");
    Serial.println(getSensorCal(doc));
}

void loop() {  
    clientCount = events.count();
    if(prevClientCount != clientCount){
        Serial.println(clientCount);
        prevClientCount=clientCount;
    }
    webSocket.cleanupClients();
    delay(100);
}