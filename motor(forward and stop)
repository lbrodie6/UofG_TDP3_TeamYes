#include "mbed.h"
#include "TCS3472_I2C.h"

// Initialize sensor and LED
TCS3472_I2C rgb_sensor(PTE0, PTE1);
DigitalOut red(LED1);

// Define L298N motor control pins
DigitalOut motorPinA1(PA_0);  // Adjust these pin assignments as per your connection
DigitalOut motorPinA2(PA_1);
DigitalOut motorPinB1(PA_2);
DigitalOut motorPinB2(PA_3);

void moveForward() {
    motorPinA1 = 1;
    motorPinA2 = 0;
    motorPinB1 = 1;
    motorPinB2 = 0;
}

void stopMotor() {
    motorPinA1 = 0;
    motorPinA2 = 0;
    motorPinB1 = 0;
    motorPinB2 = 0;
}

bool isBlack(int clearValue) {
    const int blackThreshold = 100;  // Adjust this threshold as needed based on testing
    return clearValue < blackThreshold;
}

int main() {
    rgb_sensor.enablePowerAndRGBC();
    rgb_sensor.setIntegrationTime(100);

    int rgb_readings[4];
    while(1) {
        rgb_sensor.getAllColors(rgb_readings);
        printf("clear: %d, red: %d, green: %d, blue: %d\n", rgb_readings[0], rgb_readings[1], rgb_readings[2], rgb_readings[3]);

        if (isBlack(rgb_readings[0])) {
            moveForward();
        } else {
            stopMotor();
        }

        // Rest of the color detection logic remains the same
        if (rgb_readings[1] > rgb_readings[2] && rgb_readings[1] > rgb_readings[3]) {
            printf("Red is highest\n");
        }
        if (rgb_readings[2] > rgb_readings[1] && rgb_readings[2] > rgb_readings[3]) {
            printf("Green is highest\n");
        }
        if (rgb_readings[3] > rgb_readings[1] && rgb_readings[3] > rgb_readings[2]) {
            printf("Blue is highest\n");
        }
        wait_us(100000);
    }
}
