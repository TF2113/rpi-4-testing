#include <stdio.h>
#include <pigpio.h>

#define TRIG 23
#define ECHO 24
#define LED 17

int main(){
    if(gpioInitialise() < 0){
        printf("pigpio initialisation failed\n");
        return 1;
    }

    printf("pigpio Intialised\n\n");

    gpioSetMode(TRIG,PI_OUTPUT);
    gpioSetMode(ECHO,PI_INPUT);
    gpioSetMode(LED,PI_OUTPUT);

    printf("Sensor Init\n\n");

    gpioWrite(TRIG, 0);
    gpioDelay(2000000);

    uint32_t startTick, endTick;

    for(int i = 0; i <10; i++){
        printf("Measurement %d\n", i+1);

        gpioWrite(TRIG, 1);
        gpioDelay(10);
        gpioWrite(TRIG, 0);

        while(gpioRead(ECHO) == 0);
        startTick = gpioTick();

        while(gpioRead(ECHO) == 1);
        endTick = gpioTick();

        int durationTick = endTick - startTick;
        float distance = durationTick / 58.0; // distance (cm) = ((time(microseconds) x (speed of sound in air) 0.0343) / (remove return distance) 2)

        printf("Distance: %.2f cm\n", distance);

        if(distance < 5){
            printf("! PROX!\n");
            for(int i = 0; i < 5; i++){
                gpioWrite(LED, 1);
                gpioDelay(100000);
                gpioWrite(LED, 0);
                gpioDelay(100000);
            }
            gpioDelay(1000000);
        }
        printf("\n");
        gpioDelay(1000000);
    }

    gpioTerminate();
    return 0;
}