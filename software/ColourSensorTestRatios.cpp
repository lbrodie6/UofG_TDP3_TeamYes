#include "mbed.h"
#include "TCS3472_I2C.h"
// https://os.mbed.com/users/Allar/code/TCS3472_I2C/
// ensure library has char cast to char fixed

// Pin initialisation
TCS3472_I2C rgb_sensor_L(PTE0, PTE1);
TCS3472_I2C rgb_sensor_R(PTC9, PTC8);
DigitalOut boardled(LED1);
DigitalOut Electromagnet(PTE31,PullUp);

//Initialising pins for PWM motor control
    //Right
PwmOut PwmR(PTA12);
DigitalOut Rforward(PTA2);
DigitalOut Rbackward(PTD4);
    //Left
PwmOut PwmL(PTA1);
DigitalOut Lforward(PTA5);
DigitalOut Lbackward(PTA4);

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

double getLineConstant(int clear_R, int clear_L) {
        int denominator = ((clear_L*10) + (clear_R*11));
        int numerator = (clear_L);
        float ratio = static_cast<float>(10*numerator) / denominator;
        int percent = static_cast<int>(ratio*100);
        return ratio;
}

float getTurningRatio(float control) {
    float turning_ratio = 0.5+control; 
    return turning_ratio;
}

void controlLeft(float dutycycle, bool direction) {
    if (PwmL != dutycycle){
       PwmL = dutycycle;
   }
//    if (direction == 1){
//        Lforward = 1;
//        Lbackward =0;
//    }
//    else{
//        Lforward = 0;
//        Lbackward = 1;
//    }
}    
void controlRight(float dutycycle, bool direction) {
    if (PwmR != dutycycle){
       PwmR = dutycycle;
   }
//    if (direction == 1){
//        Rforward = 1;
//        Rbackward =0;
//    }
//    else{
//        Rforward = 0;
//        Rbackward = 1;
//    }
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
    double line_constant;
    // Right Sensor
    float red_ratio_R;
    float green_ratio_R;
    float blue_ratio_R;
    // Left Sensor
    float red_ratio_L;
    float green_ratio_L;
    float blue_ratio_L;
    // PID
    float Kp = 1.0;
    float Ki = 0;
    float Kd = 0;
    float ref = 0.50;

// Initialising the sensor values - sent through I2C to sensor peripheral. Integration time sent as an argument to the function
    sensorsInitialise(20);


// Main program loop
    while (1) {
        Lforward = 1;
        Lbackward = 0;
        Rforward = 1;
        Rbackward = 0;
    // Collect RGB readings from sensors
        rgb_sensor_R.getAllColors(rgb_readings_R);
        rgb_sensor_L.getAllColors(rgb_readings_L);

        line_constant = getLineConstant(rgb_readings_R[0],rgb_readings_L[0]);
        int percent = static_cast<int>(line_constant*10000);
        //printf("Line constant percent --- \nPercent (LC*100): %d \n", percent);
    // Prints RGB readings to output, e.g. Putty. (For bug testing/troubleshooting)    
        //printf("RIGHT: clear: %d, red: %d, green: %d, blue: %d\n", rgb_readings_R[0],rgb_readings_R[1], rgb_readings_R[2], rgb_readings_R[3]);
        //printf("LEFT: clear: %d, red: %d, green: %d, blue: %d\n", rgb_readings_L[0],rgb_readings_L[1], rgb_readings_L[2], rgb_readings_L[3]);

        //PID
        float error_P = ref - line_constant; 
        float control = Kp * error_P;
        float dutycycle_R = 0.2*(getTurningRatio(control));
        float dutycycle_L = 0.2*(1.0-getTurningRatio(control));

        controlLeft(dutycycle_L,1);
        controlRight(dutycycle_R,1);


        
        int DCL = static_cast<int>(dutycycle_L*10000);
        int DCR = static_cast<int>(dutycycle_R*10000);
        printf("DCR = %d ||||||| DCL = %d\n", DCR, DCL);
    
    
    // // Ratios of colours divided by other two colour values values
    //     // Right Sensor
        
    //     red_ratio_R = rgb_readings_R[1] / ((rgb_readings_R[2] + rgb_readings_R[3]));
    //     green_ratio_R = rgb_readings_R[2] / ((rgb_readings_R[1] + rgb_readings_R[3]));
    //     blue_ratio_R = rgb_readings_R[3] / ((rgb_readings_R[1] + rgb_readings_R[2]));
    //     // Left Sensor
    //     red_ratio_L = rgb_readings_L[1] / ((rgb_readings_L[2] + rgb_readings_L[3]));
    //     green_ratio_L = rgb_readings_L[2] / ((rgb_readings_L[1] + rgb_readings_L[3]));
    //     blue_ratio_L = rgb_readings_L[3] / ((rgb_readings_L[1] + rgb_readings_L[2]));

    // // Setting logic statements to 0
    //     // EM Disk
    //     bool blue_pickup;
    //     bool red_pickup;
    //     // Right Sensor
    //     bool red_R = false;
    //     bool green_R = false;
    //     bool blue_R = false;
    //     // Left Sensor
    //     bool red_L = false;
    //     bool green_L = false;
    //     bool blue_L = false;

    // // Decision function for Red, Green, and Blue detection (respectively)
    //     // Right Sensor
    //     if (red_ratio_R > threshold[1]) {
    //     // printf("Red is highest\n");
    //     red_R = true;
    //     }
    //     if (green_ratio_R > threshold[2]) {
    //     // printf("Green is highest\n");
    //     green_R = true;
    //     }
    //     if (blue_ratio_R > threshold[3]) {
    //     // printf("Blue is highest\n");
    //     blue_R = true;
    //     }

    //     // Left Sensor
    //     if (red_ratio_L > threshold[1]) {
    //     // printf("Red is highest\n");
    //     red_L = true;
    //     }
    //     if (green_ratio_L > threshold[2]) {
    //     // printf("Green is highest\n");
    //     green_L = true;
    //     }
    //     if (blue_ratio_L > threshold[3]) {
    //     // printf("Blue is highest\n");
    //     blue_L = true;
    //     }

    
        wait_us(40000); // Replace with PID controller and move wait to after - wait must be >20000us to allow for integration time
        // // Red Disk flags for pickup and drop off
        // if (red_R == true && red_L == false) {
        // // Electromagnet Trigger ON
        // red_pickup = true;
        // } else if (red_R == true && red_L == true && red_pickup == true) {
        // // Electromagnet Trigger OFF
        // red_pickup = false;
        // }
        // else{
        //         wait_us(1);
        // }

        // // Blue Disk flags for pickup and drop off
        // if (blue_R == true && blue_L == false) {
        // // Electromagnet Trigger ON
        // Electromagnet = 0;
        // } else if (blue_R == true && blue_L == true && blue_pickup == true) {
        // // Electromagnet Trigger OFF
        // blue_pickup = false;
        // }
        // else{
        //     wait_us(1);      
        // }
    }
}
