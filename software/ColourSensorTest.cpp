#include "mbed.h"
#include "TCS3472_I2C.h"
// https://os.mbed.com/users/karlmaxwell67/code/TCS3472_I2C/

TCS3472_I2C rgb_sensor( PTE0, PTE1 );
DigitalOut red(LED1);
AnalogOut led(PTE30);
int main() {
    rgb_sensor.enablePowerAndRGBC();
    rgb_sensor.setIntegrationTime( 100 );

    int rgb_readings[4];
    while(1) {
        rgb_sensor.getAllColors( rgb_readings );
        printf( "clear: %d, red: %d, green: %d, blue: %d\n", rgb_readings[0], rgb_readings[1], rgb_readings[2], rgb_readings[3] );

        if (rgb_readings[1] > rgb_readings[2] && rgb_readings[1] > rgb_readings[3]){
            printf("Red is highest\n");
        }
        if (rgb_readings[2] > rgb_readings[1] && rgb_readings[2] > rgb_readings[3]){
            printf("Green is highest\n");
        }
        if (rgb_readings[3] > rgb_readings[1] && rgb_readings[3] > rgb_readings[2]){
            printf("Blue is highest\n");
        }
        wait_us( 100000 );
    }
}
