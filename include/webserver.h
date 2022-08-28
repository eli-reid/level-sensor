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

void onRequest(void *arg){
   
}

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

    Server.on("/scan", HTTP_GET, [](AsyncWebServerRequest *request){
        String json = "[";
        int n = WiFi.scanComplete();
        if(n == -2){
            WiFi.scanNetworks(true);
        } else if(n){
            for (int i = 0; i < n; ++i){
            if(i) json += ",";
            json += "{";
            json += "\"rssi\":"+String(WiFi.RSSI(i));
            json += ",\"ssid\":\""+WiFi.SSID(i)+"\"";
            json += ",\"bssid\":\""+WiFi.BSSIDstr(i)+"\"";
            json += ",\"channel\":"+String(WiFi.channel(i));
            json += ",\"secure\":"+String(WiFi.encryptionType(i));
            //json += ",\"hidden\":"+String(WiFi.isHidden(i)?"true":"false");
            json += "}";
            }
            WiFi.scanDelete();
            if(WiFi.scanComplete() == -2){
            WiFi.scanNetworks(true);
            }
        }
        json += "]";
        request->send(200, "application/json", json);
        json = String();
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
void (*onCmd)(AsyncWebSocketClient *client, char* cmd, char* type, char* data) = [] (AsyncWebSocketClient *client, char* cmd, char* type,  char* txt){};

// command format "CMD|TYPE|TEXT"
//TYPE = GET, POST
void wsMessageParse(char *msgData, AsyncWebSocketClient *client){
        std::vector <char *> result;
        char *tmp = strtok(msgData,"|");
        while (tmp != NULL)
        {           
           result.push_back(tmp);
           tmp = strtok (NULL, "|");
        }
        if(result.size() < 3)
            client->text("Invailid Commmand Structure Recieved!");
        else
            onCmd(client, result.at(0), result.at(1), result.at(2));
        return;
}

// only gets first frame of message 
void wsMessageHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, void *arg, uint8_t *data, size_t len){
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
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