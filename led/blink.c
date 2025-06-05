#include <stdio.h>
#include <pigpio.h>

#define LED 17

int main(){

    if(gpioInitialise() < 0){
        printf("GPIO Init Failed\n");
        return 1;
    } else {
        printf("GPIO Init Successfully\n");
    }

    gpioSetMode(LED, PI_OUTPUT);

    for(int i = 0; i < 10; i++){
        gpioWrite(LED, 1);
    
        gpioDelay(100000);
    
        gpioWrite(LED, 0);

        gpioDelay(100000);
    }

    gpioTerminate();
}