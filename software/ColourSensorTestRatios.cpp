#include "TCS3472_I2C.h"
#include "mbed.h"

// https://os.mbed.com/users/Allar/code/TCS3472_I2C/
// ensure library has char cast to char fixed

// Pin initialisation
TCS3472_I2C rgb_sensor(PTC2, PTC1);
DigitalOut boardled(LED1);
// DigitalOut red(PTA5);
// DigitalOut green(PTA4);
// DigitalOut blue(PTA12);
// DigitalOut lineLED(PTA13);
AnalogOut led(PTE30);
// AnalogIn line_threshold(PTB0);

int main() {
  // Initialising the sensor values - sent through I2C to sensor peripheral
  rgb_sensor.enablePowerAndRGBC();
  rgb_sensor.setIntegrationTime(20);
  // Variable initiation
  float threshold[4] = {1250.0, 3.0, 3.0, 2.5};
  int rgb_readings[4];
  float line_constant;
  // Right Sensor
  float red_ratio_R;
  float green_ratio_R;
  float blue_ratio_R;
  // Left Sensor
  float red_ratio_L;
  float green_ratio_L;
  float blue_ratio_L;
  // Main Loop
  while (1) {
    rgb_sensor.getAllColors(rgb_readings);
    printf("clear: %d, red: %d, green: %d, blue: %d\n", rgb_readings[0],
           rgb_readings[1], rgb_readings[2], rgb_readings[3]);
    // Ratios of colours divided by other two colour values values
    // Right Sensor
    red_ratio_R = rgb_readings[1] / ((rgb_readings[2] + rgb_readings[3]));
    green_ratio_R = rgb_readings[2] / ((rgb_readings[1] + rgb_readings[3]));
    blue_ratio_R = rgb_readings[3] / ((rgb_readings[1] + rgb_readings[2]));
    // Left Sensor
    red_ratio_L = rgb_readings[1] / ((rgb_readings[2] + rgb_readings[3]));
    green_ratio_L = rgb_readings[2] / ((rgb_readings[1] + rgb_readings[3]));
    blue_ratio_L = rgb_readings[3] / ((rgb_readings[1] + rgb_readings[2]));
    // Setting logic statements to 0
    // EM Disk
    bool blue_pickup;
    bool red_pickup;
    // Right Sensor
    bool red_R = false;
    bool green_R = false;
    bool blue_R = false;
    bool on_line_R = false;
    // Left Sensor
    bool red_L = false;
    bool green_L = false;
    bool blue_L = false;
    bool on_line_L = false;

// If conditions for Red, Green, and Blue detection (respectively)
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
    //line_constant will be a value between 0.0-1.0, where [0.0 -> Hard LEFT] & [1.0 -> Hard RIGHT]
    //real values for this will vary between about 0.25-0.75 from testing
    line_constant = (rgb_readings['left clear']/(rgb_readings['right_clear']+rgb_readings['left clear']));
    

    wait_us(2000);
    //Turning
    if (on_line_R == true) {
      //Turn right using PID control
    } else if (on_line_L) {
      //Turn left using PID control
    }


    // Red Disk flags for pickup and drop off
    if (red_R == true && red_L == false) {
      // Electromagnet Trigger ON
      red_pickup = true;
    } else if (red_R == true && red_L == true && red_pickup == true) {
      // Electromagnet Trigger OFF
      red_pickup = false;
    }

    // Blue Disk flags for pickup and drop off
    if (blue_R == true && blue_L == false) {
      // Electromagnet Trigger ON
      blue_pickup = true;
    } else if (blue_R == true && blue_L == true && blue_pickup == true) {
      // Electromagnet Trigger OFF
      blue_pickup = false;
    }
  }
}