
#include "mbed.h"
#include "TCS3472_I2C.h"
// https://os.mbed.com/users/karlmaxwell67/code/TCS3472_I2C/

TCS3472_I2C rgb_sensor( PTE0, PTE1 );
DigitalOut led(LED1);
DigitalOut red(PTA5)
DigitalOut green(PTA4)
DigitalOut blue(PTA12)
DigitalOut lineLED(PTA13)
AnalogOut led(PTE30);
//AnalogIn line_threshold(PTB0);
int main() {
    rgb_sensor.enablePowerAndRGBC();
    rgb_sensor.setIntegrationTime( 100 );
    int line_threshold = 5000;
    bool red_high;
    bool green_high;
    bool blue_high;
    bool on_line;
    int line_threshold;
    int rgb_readings[4];
    while(1) {
        rgb_sensor.getAllColors( rgb_readings );
        printf( "clear: %d, red: %d, green: %d, blue: %d\n", rgb_readings[0], rgb_readings[1], rgb_readings[2], rgb_readings[3] );

        if (rgb_readings[1] > rgb_readings[2] && rgb_readings[1] > rgb_readings[3]){
            red_high = true;
        }
        if (rgb_readings[2] > rgb_readings[1] && rgb_readings[2] > rgb_readings[3]){
            green_high = true;
        }
        if (rgb_readings[3] > rgb_readings[1] && rgb_readings[3] > rgb_readings[2]){
            blue_high = true;
        }
        if (rgb_readings[0] < line_threshold){
            on_line = true;
        }
        if (red_high == true){
            red = 1;
        }
        if (green_high == true){
            green = 1;
        }
        if (blue_high == true){
            blue = 1;
        }
        if (on_line == true){
            lineLED = 1;
        }
        
        wait_us(100000);
    }
}