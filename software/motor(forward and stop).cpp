#include "mbed.h"


// Define L298N motor control pins
PwmOut PwmR(PTA4);          // PWM for right side motors
DigitalOut Rforward(PTC3);  // Adjust these pin assignments as per your connection
DigitalOut Rbackward(PTC4);

PwmOut PwmL(PTA5);          // PWM for left side motors
DigitalOut Lforward(PTC5);
DigitalOut Lbackward(PTC6);

void control_R(float dutycycle, bool direction) {
    if (PWML != dutycycle;){
        PWML = dutycycle;
    }
    if (direction == 1){
        Rforward = 1;
        Rbackward =0;
    }
    else{
        Rforward = 0;
        Rbackward = 1;
    }
}
    
####    
PwmR ! = dutycycle;
    if (direction==1){
    Rforward = 1;
    Rbackward = 0;
    }
    else{
    Rforward = 0;
    Rbackward = 1;
    }
}
####

void stopMotor() {
    Rforward = 0;
    Rbackward = 0;
    Lforward = 0;
    Lbackward = 0;

}

void control_L(float dutycycle, bool direction) {
    if (PWMR != dutycycle){
        PWMR =dutycycle;
    }
    if (direction == 1){
        Lforward = 1;
        Lbackward = 0;
    }
    else{
        Lforward = 0;
        Lbackward = 1;
    }
    
    
    
    ####
    PwmL ! = dutycycle;
    if(direction==1){
    Lforward = 1;
    Lbackward = 0;
    }
    else{
    Lforward = 0;
    Lbackward = 1;
    }
    ####
}



int main() {
    while(1){
        for (float i = 0.0; i < 500; i++){ 
            control_R(i/500.0,1);
            control_L(i/500.0,1);
            wait_us(40000);
        }
        stopMotor();
        wait_us(5000000);//start speed 0.5

        control_R(0.1,1);
        control_L(0.1,1);
        wait_us(5000000);//start speed 0.5

        stopMotor();
        wait_us(5000000);//start speed 0.5

        control_R(1,1);
        control_L(1,1);
        wait_us(5000000);//start speed 0.5

        stopMotor();
         wait_us(1000000);//start speed 0.5

        control_R(0.1,0);
        control_L(0.1,0);
        wait_us(5000000);//start speed 0.5

        stopMotor();
        wait_us(1000000);//start speed 0.5

        control_R(1,0);
        control_L(1,0);
        wait_us(5000000);//start speed 0.5

        stopMotor();
        wait_us(1000000);//start speed 0.5 

        control_R(0.5,1);
        control_L(1,1);
        wait_us(5000000);//start speed 0.5

        stopMotor();
        wait_us(1000000);//start speed 0.5

        control_R(1,1);
        control_L(0.5,1);
        wait_us(5000000);//start speed 0.5  

        stopMotor();
    }


}
        if (rgb_readings[3] > rgb_readings[1] && rgb_readings[3] > rgb_readings[2]) {
            printf("Blue is highest\n");
        }
        wait_us(100000);
    }
}
