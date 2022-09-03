#include "main.h"
int prevClientCount = 0 ,
     clientCount = 0;
float d = distIn;
char dist[10];

void setup() {
    tcpip_adapter_init();
    startSerial();
    startDistanceTimer();
    startFileSystem();
    startHttpServer();
    increamentBootCount(doc);
    connectToNetwork();
    Serial.print("Current Boot Count: ");
    Serial.println(getBootCount(doc));
    Serial.print("Current Sensor cal: ");
    Serial.println(getSensorCal(doc));
    onCmd = onCommand;
  
}

void loop() {  
    clientCount = events.count();
    if(prevClientCount != clientCount){
        Serial.println(clientCount);
        prevClientCount=clientCount;
    }
    webSocket.cleanupClients();
    
    if(d != distIn){
        d=distIn;
        sprintf(dist,"%f",distIn+getSensorCal(doc));
        events.send(dist,"newdistance",millis());
    }
    delay(100);
}