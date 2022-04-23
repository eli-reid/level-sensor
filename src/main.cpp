#include "main.h"
int prevClientCount = 0 ,
     clientCount = 0;
float d = distIn;
char dist[10];

void setup() {
    startSerial();
    startFileSystem();
    startHttpServer();
    startDistanceTimer();
    increamentBootCount();
    connectToNetwork();
    Serial.print("Current Boot Count: ");
    Serial.println(getBootCount());
    onCmd = CommandHandler;
  
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
        sprintf(dist,"%f",distIn);
        events.send(dist,"newdistance",millis());
    }
    delay(100);
}