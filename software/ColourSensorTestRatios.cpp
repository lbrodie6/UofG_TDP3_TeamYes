#include "mbed.h"
#include "TCS3472_I2C.h"
// https://os.mbed.com/users/Allar/code/TCS3472_I2C/
// ensure library has char cast to char fixed

// Pin initialisation
TCS3472_I2C rgb_sensor_R(PTE0, PTE1);
TCS3472_I2C rgb_sensor_L(PTC9, PTC8);
DigitalOut boardled(LED1);
DigitalOut Electromagnet(PTE31,PullUp);
// DigitalOut green(PTA4);
// DigitalOut blue(PTA12);
// DigitalOut lineLED(PTA13);
//AnalogOut led(PTE30);
// AnalogIn line_threshold(PTB0);

void sensorsInitialise(int int_time) {
    rgb_sensor_R.enablePowerAndRGBC();
    rgb_sensor_L.enablePowerAndRGBC();
    rgb_sensor_R.setIntegrationTime(int_time);
    rgb_sensor_L.setIntegrationTime(int_time);
}

// void sensorsGetRatios(int rgb_readings_R[4], int rgb_readings_L[4])/{
//     rgb_sensor_R.getAllColors(rgb_readings_R);
//     rgb_sensor_L.getAllColors(rgb_readings_L);
//     printf("RIGHT: clear: %d, red: %d, green: %d, blue: %d\n", rgb_readings_R[0],rgb_readings_R[1], rgb_readings_R[2], rgb_readings_R[3]);
//     printf("LEFT: clear: %d, red: %d, green: %d, blue: %d\n", rgb_readings_L[0],rgb_readings_L[1], rgb_readings_L[2], rgb_readings_L[3]);
// // Ratios of colours divided by other two colour values values
//     // Right Sensor
//     red_ratio_R = rgb_readings_R[1] / ((rgb_readings_R[2] + rgb_readings_R[3]));
//     green_ratio_R = rgb_readings_R[2] / ((rgb_readings_R[1] + rgb_readings_R[3]));
//     blue_ratio_R = rgb_readings_R[3] / ((rgb_readings_R[1] + rgb_readings_R[2]));
//     // Left Sensor
//     red_ratio_L = rgb_readings_L[1] / ((rgb_readings_L[2] + rgb_readings_L[3]));
//     green_ratio_L = rgb_readings_L[2] / ((rgb_readings_L[1] + rgb_readings_L[3]));
//     blue_ratio_L = rgb_readings_L[3] / ((rgb_readings_L[1] + rgb_readings_L[2]));
//     return(red_ratio_R, green_ratio_R, blue_ratio_R, red_ratio_L, green_ratio_L, blue_ratio_L, rgb_readings_R, rgb_readings_L)
// }

// Main program
int main() {

// Variable initiation
    float threshold[4] = {1250.0, 3.0, 3.0, 2.5};
    int rgb_readings_R[4];
    int rgb_readings_L[4];
    float clear_R = rgb_readings_R[0];
    float clear_L = rgb_readings_L[0];
    float line_constant = 0;
    // Right Sensor
    float red_ratio_R;
    float green_ratio_R;
    float blue_ratio_R;
    // Left Sensor
    float red_ratio_L;
    float green_ratio_L;
    float blue_ratio_L;
// Initialising the sensor values - sent through I2C to sensor peripheral. Integration time sent as an argument to the function
    sensorsInitialise(20);

// Main program loop
    while (1) {
    
    // Collect RGB readings from sensors
        rgb_sensor_R.getAllColors(rgb_readings_R);
        rgb_sensor_L.getAllColors(rgb_readings_L);
    
    // Prints RGB readings to output, e.g. Putty. (For bug testing/troubleshooting)    
        //printf("RIGHT: clear: %d, red: %d, green: %d, blue: %d\n", rgb_readings_R[0],rgb_readings_R[1], rgb_readings_R[2], rgb_readings_R[3]);
        //printf("LEFT: clear: %d, red: %d, green: %d, blue: %d\n", rgb_readings_L[0],rgb_readings_L[1], rgb_readings_L[2], rgb_readings_L[3]);
    
    // Ratios of colours divided by other two colour values values
        // Right Sensor
        red_ratio_R = rgb_readings_R[1] / ((rgb_readings_R[2] + rgb_readings_R[3]));
        green_ratio_R = rgb_readings_R[2] / ((rgb_readings_R[1] + rgb_readings_R[3]));
        blue_ratio_R = rgb_readings_R[3] / ((rgb_readings_R[1] + rgb_readings_R[2]));
        // Left Sensor
        red_ratio_L = rgb_readings_L[1] / ((rgb_readings_L[2] + rgb_readings_L[3]));
        green_ratio_L = rgb_readings_L[2] / ((rgb_readings_L[1] + rgb_readings_L[3]));
        blue_ratio_L = rgb_readings_L[3] / ((rgb_readings_L[1] + rgb_readings_L[2]));

    // Setting logic statements to 0
        // EM Disk
        bool blue_pickup;
        bool red_pickup;
        // Right Sensor
        bool red_R = false;
        bool green_R = false;
        bool blue_R = false;
        // Left Sensor
        bool red_L = false;
        bool green_L = false;
        bool blue_L = false;

    // Decision function for Red, Green, and Blue detection (respectively)
        // Right Sensor
        if (red_ratio_R > threshold[1]) {
        // printf("Red is highest\n");
        red_R = true;
        }
        if (green_ratio_R > threshold[2]) {
        // printf("Green is highest\n");
        green_R = true;
        }
        if (blue_ratio_R > threshold[3]) {
        // printf("Blue is highest\n");
        blue_R = true;
        }

        // Left Sensor
        if (red_ratio_L > threshold[1]) {
        // printf("Red is highest\n");
        red_L = true;
        }
        if (green_ratio_L > threshold[2]) {
        // printf("Green is highest\n");
        green_L = true;
        }
        if (blue_ratio_L > threshold[3]) {
        // printf("Blue is highest\n");
        blue_L = true;
        }

    //Clear values Left-Right comparison
        // line_constant will be a variable with a value between 0.0-1.0, where [0.0 -> Hard LEFT] & [1.0 -> Hard RIGHT]
        // real values for this will vary between about 0.25-0.75 from testing
        line_constant = (1.0*rgb_readings_L[0]/(1.0*rgb_readings_R[0]+1.0*rgb_readings_L[0]));
        
        printf("Line Constant: %d \n", line_constant);
        wait_us(200000); // Replace with PID controller and move wait to after - wait must be >20000us to allow for integration time


        // Red Disk flags for pickup and drop off
        if (red_R == true && red_L == false) {
        // Electromagnet Trigger ON
        red_pickup = true;
        } else if (red_R == true && red_L == true && red_pickup == true) {
        // Electromagnet Trigger OFF
        red_pickup = false;
        }
        else{
                wait_us(1);
        }

        // Blue Disk flags for pickup and drop off
        if (blue_R == true && blue_L == false) {
        // Electromagnet Trigger ON
        Electromagnet = 0;
        } else if (blue_R == true && blue_L == true && blue_pickup == true) {
        // Electromagnet Trigger OFF
        blue_pickup = false;
        }
        else{
            wait_us(1);      
        }
    }
}
