#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define GPIO_OFFSET 0x0 // /dev/gpiomem/ is stored in virtual memory via mmap(), no offset needed
#define MEM_BLOCK 4096 // 4KB memory block for storing gpiomem register data

//RPI 4 Model B GPIO register addresses via (https://cs140e.sergio.bz/docs/BCM2835-ARM-Peripherals.pdf)
//https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf
#define GPSEL1 0x4  //GPIO Function Select for pins 10-19
#define GPSET0 0x1C //GPIO Set Pin Output
#define GPCLR0 0x28 //GPIO Clear Pin Output
#define GPLEV0 0x34 //GPIO Read Level

int main(){
    int fd = open("/dev/gpiomem", O_RDWR | O_SYNC); //Opens gpiomem with READ/WRITE perms and flags for SYNC to 
                                                    //prevent program from continuing before writes are finished
    if(fd < 0){
        perror("Access gpiomem fail\n");
        return 1;
    }

    //Creating a 32 bit unsigned int pointer to gpiomem for direct GPIO register access and manipulation via mmap()
    volatile uint32_t *gpio = mmap(
                                        NULL,                   //Kernel chooses address
                                        MEM_BLOCK,              //Size of data to be mapped
                                        PROT_READ | PROT_WRITE, //Allows pages to be read and written to
                                        MAP_SHARED,             //Allows other processes to use mapping
                                        fd,                     //gpiomem file descriptor
                                        GPIO_OFFSET);           //Start of address for mapping

    if(*gpio == MAP_FAILED){
        perror("Failed to map gpio");
        close(fd);
        return 1;
    }

    //1. Set gpio 17 as gpio output
    uint32_t led_reg = gpio[GPSEL1 / 4]; //Access GPSEL1 register

    led_reg = led_reg & ~(7 << 21); //Clears the 3 bit address in GPSEL1 corresponding to pin 17 located at bits 21-23 to 000
                                    //by first setting 3 bits to 111 (decimal 7 in binary) then shifts those bits to the left by 21
                                    //then NOT operator '~' flips all bits
                                    //00000111 00000000 00000000 00000000 gets flipped to
                                    //11111000 11111111 11111111 11111111
                                    //setting bits for GPIO 17 (21-23) to 0 / clear

    led_reg = led_reg | (1 << 21); //Move 001 to register 21-23 to assign GPIO 17 as an output (https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf p.67)
    gpio[GPSEL1 /4 ] = led_reg; //Move updated bits 21-23 back into the GPSEL1 register
    
    //2. Set gpio 17 to high
    //3. Delay
    //4. Set gpio 17 to low
    //5. Clear gpio 17
    //6. Close

}