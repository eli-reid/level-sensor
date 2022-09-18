#ifndef __SENSOR_H
#define __SENSOR_H

#include <Arduino.h>
#include <NewPing.h>

const int DISTANCE_PING_DELAY = 500000;

void(*onNewDistance)(float distance) = [](float distance){};

void IRAM_ATTR getDistance(void* args){
    auto levelSensor = NewPing(18,19,400);
    long timeDist =  levelSensor.ping_median();
    double distance = (double) timeDist / 73.746 / 2;
    if(distance > 8)
    { 
        onNewDistance((float)distance);
    }
}

//Timer Setup
const esp_timer_create_args_t distanceDisplayTimerArgs = { .callback = &getDistance };
esp_timer_handle_t distanceDisplayTimer;
void startDistanceTimer(){
    esp_timer_create(&distanceDisplayTimerArgs, &distanceDisplayTimer);
    esp_timer_start_periodic(distanceDisplayTimer, DISTANCE_PING_DELAY);
}

#endif  
