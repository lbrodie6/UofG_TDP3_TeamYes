#include "mbed.h"
#include "TCS3472_I2C.h"
//https://os.mbed.com/users/Allar/code/TCS3472_I2C/

//Pin initialisation
TCS3472_I2C rgb_sensor(PTC2, PTC1);
DigitalOut boardled(LED1);
//DigitalOut red(PTA5);
//DigitalOut green(PTA4);
//DigitalOut blue(PTA12);
//DigitalOut lineLED(PTA13);
AnalogOut led(PTE30);
//AnalogIn line_threshold(PTB0);
int main() {
//Initialising the sensor values - sent through I2C to sensor microcontroler
    rgb_sensor.enablePowerAndRGBC();
    rgb_sensor.setIntegrationTime(20);
//Variable initiation
    int line_threshold = 500;
    int red_threshold = 50;
    int green_threshold = 40;
    int blue_threshold = 40;
    bool red_high;
    bool green_high;
    bool blue_high;
    bool on_line;
    int rgb_readings[4];
    int red_ratio;
    int green_ratio;
    int blue_ratio;
    while(1) {
        rgb_sensor.getAllColors(rgb_readings);
        printf("clear: %d, red: %d, green: %d, blue: %d\n", rgb_readings[0], rgb_readings[1], rgb_readings[2], rgb_readings[3]);
    //Ratios of colours divided by clear values
        red_ratio = rgb_readings[1] / (rgb_readings[0]/100); 
        green_ratio = rgb_readings[2] / (rgb_readings[0]/100);              
        blue_ratio = rgb_readings[3] / (rgb_readings[0]/100);               
    //Setting logic statements to 0
        red_high = false;
        green_high = false;
        blue_high = false;
        on_line = false;
    //If conditions for Red, Green, and Blue detection (respectively)
        if (rgb_readings[1] > rgb_readings[2] && rgb_readings[1] > rgb_readings[3] && red_ratio > red_threshold){
            //printf("Red is highest\n");
            red_high = true;
        }
        if (rgb_readings[2] > rgb_readings[1] && rgb_readings[2] > rgb_readings[3] && green_ratio > green_threshold){
            //printf("Green is highest\n");
            green_high = true;
        }
        if (rgb_readings[3] > rgb_readings[1] && rgb_readings[3] > rgb_readings[2] && blue_ratio > blue_threshold){
            //printf("Blue is highest\n");
            blue_high = true;
        }
    //If conditions for line detection
        if (rgb_readings[0] < line_threshold && red_high == false && green_high == false && blue_high == false){
            on_line = true;
        }
    //Sets output pins to high or low
        //red = red_high;
        //green = green_high;
        //blue = blue_high;
        //lineLED = on_line;
        wait_us(2000);
    }
}
