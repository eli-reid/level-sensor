#ifndef _WEBSERVER_H
#define _WEBSERVER_H
#include <ESPAsyncWebServer.h>
#include <AsyncWebSocket.h>
#include <AsyncEventSource.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>
#include <stdio.h>
AsyncWebServer Server(80); 
AsyncWebSocket webSocket("/ws");
AsyncEventSource events("/events");

String processor(const String &var){
    return "";
}

bool (loginHandler)(String user, String password){

    return false;
}

void onRequest(void *arg){}

void setupHttpServer(){  
    Server.serveStatic("/", SPIFFS, "/www/").setDefaultFile("index.html");
    Server.serveStatic("/", SPIFFS, "/www/").setCacheControl("max-age=60");

    Server.on("/login", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/login.html", String(), false, processor);
    });
    Server.on("/login", HTTP_POST, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/status.html", String(), false, processor);
    });

    Server.on("/config.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/config.html", String(), false, processor);
    });
    Server.on("/status.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/status.html", String(), false, processor);
    });
    Server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", String(ESP.getFreeHeap()));
    });

    Server.onNotFound([](AsyncWebServerRequest *request){
        request->send(404);
    });
}

/*
*************************************************************************************************************************************
|                                                   WebEvents Setup                                                                 |
*************************************************************************************************************************************
*/

void setupEvents(){
    events.onConnect([](AsyncEventSourceClient *client){
        if(client->lastId()){
            Serial.printf("Client reconnected! Last message ID that it gat is: %u\n", client->lastId());
        }
        client->send("Connected",NULL,millis(),1000);
    });
    Server.addHandler(&events);
}

/*
*************************************************************************************************************************************
|                                                   WebSocket Setup                                                                 |
*************************************************************************************************************************************
*/

String getStatusJSON(){
   DynamicJsonDocument statusJson(2048);
   String temp;
   statusJson["WIFI_IP"] = WiFi.localIP();
   statusJson["WIFI_SSI"] = WiFi.SSID();
   serializeJsonPretty(statusJson, temp);
   return temp;
}

//place holder function for websocket message event
void (*onCmd)(AsyncWebSocketClient *client, const char* type, int cmd, const char* data) = [] (AsyncWebSocketClient *client,  const char* type, int cmd, const char* txt){/*place holder for custom function*/};

// command format "TYPE|CMD|TEXT"
//TYPE = GET, POST
void wsMessageParse(char *msgData, AsyncWebSocketClient *client){
    try
    {
        char *saveptr;
        const char *type = strtok_r(msgData,"|", &saveptr); 
        const char *cmdTmp = strtok_r(nullptr, "|", &saveptr);
        const char *data = strtok_r(nullptr, "|", &saveptr);
        Serial.println(type);
        Serial.println(cmdTmp);
        Serial.println(data);
        int cmd;
        if(cmdTmp!=nullptr)
            cmd = std::stoi(cmdTmp);
        else 
            cmd =-1;
        Serial.println(cmd);
        if(type ==nullptr || cmd == -1 || data == nullptr)
            client->text("Invailid Commmand Structure Recieved!");
        else
            onCmd(client, type, cmd, data);
        return;
    }
    catch(const std::exception& e)
    {
         Serial.println(e.what());
    }
}

// only gets first frame of message 
void wsMessageHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, void *arg, uint8_t *data, size_t len){
    const AwsFrameInfo * info = (AwsFrameInfo*)arg;
    if(info->final && info->index == 0 && info->len == len){
        Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);
        if(info->opcode == WS_TEXT){
            data[len] = 0;
            Serial.printf("%s\n", (char*)data);
            wsMessageParse((char*)data, client);
        }
    } 
}

void onWSEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
        Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
        client->printf("Hello Client %u :)", client->id());
        client->ping();
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
        break;
    case WS_EVT_DATA:
        wsMessageHandler(server, client, arg, data, len);
        break;
    case WS_EVT_PONG:
        Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
        break;
    case WS_EVT_ERROR:
      break;
    }
}

void setupWebSocket(){
    webSocket.onEvent(onWSEvent);
    Server.addHandler(&webSocket);
}

void startHttpServer(){
    setupHttpServer();
    setupEvents();
    setupWebSocket();
    Server.begin();
}

#endif