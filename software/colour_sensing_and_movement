
#include "mbed.h"
#include "TCS3472_I2C.h"
#include <string>
// https://os.mbed.com/users/Allar/code/TCS3472_I2C/
// ensure library has char cast to char fixed

// Pin initialisation
// Initialising pins for colour sensors
TCS3472_I2C rgb_sensor_R(PTE0, PTE1);
TCS3472_I2C rgb_sensor_L(PTC9, PTC8);

// Initialising EM pin
PwmOut Electromagnet(PTE31);

// Initialising line-detection

DigitalIn R3(PTA13);
DigitalIn R2(PTD5);
DigitalIn R1(PTD0);
DigitalIn L1(PTD2);
DigitalIn L2(PTD3);
DigitalIn L3(PTA17);


//Initialising pins for PWM motor control
    //Right
PwmOut PwmL(PTA12);
DigitalOut Lforward(PTA2);
DigitalOut Lbackward(PTD4);
    //Left
PwmOut PwmR(PTA1);
DigitalOut Rforward(PTA5);
DigitalOut Rbackward(PTA4);


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

void controlLeft(float dutycycle) {
    // Direction control
    if (dutycycle > 0){
        Lforward = 1;
        Lbackward = 0;
    }
    else if(dutycycle < 0){
        Lforward = 0;
        Lbackward = 1;
        dutycycle = dutycycle*(-1.0); // changes sign of negative dutycycle
    }
    else{
        Lforward = 0;
        Lbackward = 0;
    }
    // PWM control
    if (PwmL != dutycycle){
    PwmL = (dutycycle);
    }
}    
void controlRight(float dutycycle) {
    // Direction control
    if (dutycycle > 0){
        Rforward = 1;
        Rbackward = 0;
    }
    else if(dutycycle < 0){
        Rforward = 0;
        Rbackward = 1;
        dutycycle = dutycycle*(-1.0); // Changes sign of negative dutycycle
    }
    else{
        Rforward = 0;
        Rbackward = 0;
    }
    // PWM control
    if (PwmR != dutycycle){
    PwmR = (dutycycle);
    }
}



    // void sensorsGetRatios(int rgb_readings_R[4], int rgb_readings_L[4]){
    //  rgb_sensor_R.getAllColors(rgb_readings_R);
    //  rgb_sensor_L.getAllColors(rgb_readings_L);
    //  printf("RIGHT: clear: %d, red: %d, green: %d, blue: %d\n", rgb_readings_R[0],rgb_readings_R[1], rgb_readings_R[2], rgb_readings_R[3]);
    //  printf("LEFT: clear: %d, red: %d, green: %d, blue: %d\n", rgb_readings_L[0],rgb_readings_L[1], rgb_readings_L[2], rgb_readings_L[3]);
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
//}

// Main program
int main() {
// Variable initiation
    float threshold[4] = {1250.0, 0.65, 0.65, 0.65};
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
    
    // // PID
    // float Kp = 3.5;
    // float Ki = 0;
    // float Kd = 0;
    // float ref = 0.50;
    // int run_count = 0;
    
    // // LR control
    // int Lth = 1600;
    // int Rth = 1600;
    
    //

    // EM control
    bool disk = 1;

// Initialising the sensor values - sent through I2C to sensor peripheral. Integration time sent as an argument to the function
    sensorsInitialise(20);

// Line Detection pinmode assignment
// L3.mode(PullUp);
// L2.mode(PullUp);
// L1.mode(PullUp);
// R1.mode(PullUp);
// R2.mode(PullUp);
// R3.mode(PullUp);

// EM Dutycycle


// Main program loop
    while (1) {
    // Collect RGB readings from sensors
        rgb_sensor_R.getAllColors(rgb_readings_R);
        rgb_sensor_L.getAllColors(rgb_readings_L);
        char cases;
        char last_case;

    if (disk == 1){
        Electromagnet = (0.15);
    }
    else {
        Electromagnet = (1.0);
    }

int F_stop;
//LR cases
if       (L3==1 && L2==1 && L1==1 && R1==1 && R2==1 && R3==1 && F_stop < 200){
    cases='B';
    F_stop++; 
}else if (L3==1 && L2==1 && L1==1 && R1==1 && R2==1 && R3==1 && F_stop >= 200){
    cases='F';
    F_stop++;
    if (F_stop >= 400){
        F_stop = 0;
        }
}else if (L3==1 && L2==1 && L1==1 && R3==0){
    cases='R';
    F_stop = 0;
}else if (L3==1 && L2==1 && L1==1 && R2==0 && R3==1){
    cases='R';
    F_stop = 0;
}else if (L3==1 && L2==1 && L1==1 && R1==0 && R2==1 && R3==1){
    cases='r';
    F_stop = 0;
}else if (L3==0 && R1==1 && R2==1 && R3==1){
    cases='L';
    F_stop = 0;
}else if (L3==1 && L2==0 && R1==1 && R2==1 && R3==1){
    cases='L';
    F_stop = 0;
}else if (L3==1 && L2==1 && L1==0 && R1==1 && R2==1 && R3==1){
    cases='l';
    F_stop = 0;
}else {
    cases='X';
    F_stop = 0;
}

//        //printf("Line constant percent --- \nPercent (LC*100): %d \n", percent);
//     // Prints RGB readings to output, e.g. Putty. (For bug testing/troubleshooting)    
//           //printf("R: clear: %d, red: %d, green: %d, blue: %d\n", rgb_readings_R[0],rgb_readings_R[1], rgb_readings_R[2], rgb_readings_R[3]);
//           //printf("L: clear: %d, red: %d, green: %d, blue: %d\n", rgb_readings_L[0],rgb_readings_L[1], rgb_readings_L[2], rgb_readings_L[3]);

        //PID
        // line_constant = getLineConstant(rgb_readings_R[0],rgb_readings_L[0]);
        // int percent = static_cast<int>(line_constant*10000);
        // float error_P = ref - line_constant; 
        // float control = Kp * error_P;      // control term for dz
        // float dutycycle_R = 0.25+(-(control)); // between -1 to 1
        // float dutycycle_L = 0.25+((control));

        // controlLeft(1.0);
        // controlRight(-1.0);
        
        //LR control
    float d_speed = 0.45;
    float D_speed = 0.7;
    float t_speed = 0.35;
    float T_speed = 0.67;
    int d_time = 250000;
    int D_time = 200000;
    int t_time = 200000;
    int T_time = 400000;

        switch(cases) {
        case 'B':
            controlLeft(-d_speed*0.3);
            controlRight(-d_speed*0.3);
        break;
        case 'F':
            controlLeft(d_speed*0.4);
            controlRight(d_speed*0.4);
        break;
        case 'l':
            controlLeft(-t_speed);
            controlRight(t_speed);
            wait_us(t_time);
            controlLeft(d_speed);
            controlRight(d_speed);
            wait_us(d_time);
        last_case = cases;
        break;
        case 'r':
            controlLeft(t_speed);            
            controlRight(-t_speed);
            wait_us(t_time);
            controlLeft(d_speed);
            controlRight(d_speed);
            wait_us(d_time);
        last_case = cases;
        break;
        case 'L':
            controlLeft(-T_speed);
            controlRight(T_speed);
            wait_us(T_time);
            controlLeft(D_speed);
            controlRight(D_speed);
            wait_us(D_time);
            last_case = cases;
        break;
        case 'R':
            controlLeft(T_speed);
            controlRight(-T_speed);
            wait_us(T_time);
            controlLeft(D_speed);
            controlRight(D_speed);
            wait_us(D_time);
            last_case = cases;    
        break;
        case 'X':
            controlLeft(d_speed*-0.25);
            controlRight(d_speed*-0.25);
        break;
        }

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
        // Red
        if (red_ratio_R > threshold[1]) {
            // printf("Red is highest\n");
            red_R = true;}
        else {red_R = false;}
        // Green 
        if (green_ratio_R > threshold[2]) {
            // printf("Green is highest\n");
            green_R = true;}
        else {green_R = false;}
        // Blue
        if (blue_ratio_R > threshold[3]) {
            // printf("Blue is highest\n");
            blue_R = true;}
        else {blue_R = false;} 

    // Left Sensor
        // Red
        if (red_ratio_L > threshold[1]) {
            //printf("Red is highest\n");
            red_L = true;}
        else {red_L = false;}
        // Green
        if (green_ratio_L > threshold[2]) {
            //printf("Green is highest\n");
            green_L = true;}
        else {green_L = false;} 
        // Blue
        if (blue_ratio_L > threshold[3]) {
            //printf("Blue is highest\n");
            blue_L = true;} 
        else {blue_L = false;} 

    // if (red_L == true && red_R == true){
    //     disk = 1;
    //     }
    // if (blue_L == true && blue_R == true){
    //     disk = 1;
    //     }

        wait_us(2000); // Replace with PID controller and move wait to after - wait must be >20000us to allow for integration time
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

