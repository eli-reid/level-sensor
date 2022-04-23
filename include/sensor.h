#ifndef __SENSOR_H
#define __SENSOR_H

#include <Arduino.h>
#include <NewPing.h>

#define DISTANCE_PING_DELAY  500000

float distIn = 0;
NewPing levelSensor = NewPing(18,19,400);

void IRAM_ATTR getDistance(void* args){
    long timeDist =  levelSensor.ping_median();
    float distance = timeDist / 73.746 / 2;
    if(distance > 8)
    { 
        distIn = distance;
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
