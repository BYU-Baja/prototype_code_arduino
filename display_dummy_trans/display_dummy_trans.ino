#include <Wire.h>

#define SLAVE_ADDR 0x16

#define SPEED_ID 0x04
#define RPM_ID 0x02
#define FUEL_LEVEL_ID 0x03

#define SPEED_COUNTER_MAX 100

void sendRPMData();
void sendFuelLevelData();
void sendSpeedData();

uint8_t speed = 15;
uint16_t rpm = 1000;
float fuel_level = 100.0;

uint32_t speed_counter = 0;

typedef union
{
  float f;
  uint8_t array[4];
} FLOATUNION_t;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
}

void loop()
{
  sendRPMData();
  sendFuelLevelData();
  sendSpeedData();
//  if (speed_counter >= SPEED_COUNTER_MAX) {
//    sendSpeedData();
//    speed_counter = 0;
//  } else 
//    speed_counter++;

  delay(100);
}

void sendRPMData() {
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(RPM_ID);
  Wire.write(rpm & 0xFF);
  Wire.write(rpm >> 8);
  Wire.endTransmission();
  Serial.print("RPM:");
  Serial.println(rpm);

  if (rpm > 9000) {
    rpm = 1000;
  } else {
    rpm++; // We can just roll over rpm
  }
}

void sendFuelLevelData() {
  FLOATUNION_t converter;
  converter.f = fuel_level;
  
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(FUEL_LEVEL_ID);
  Wire.write(converter.array[0] & 0xFF);
  Wire.write(converter.array[1] >> 8);
  Wire.write(converter.array[2] >> 16);
  Wire.write(converter.array[3] >> 24);
  Wire.endTransmission();
  Serial.print("FUEL:");
  Serial.println(converter.array[0]);
  Serial.println(converter.array[1]);
  Serial.println(converter.array[2]);
  Serial.println(converter.array[3]);
  

  if (fuel_level > 0) {
    fuel_level = fuel_level - 0.1; 
  } else {
    fuel_level = 100.0;
  }
}

void sendSpeedData() {
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(SPEED_ID);
  Wire.write(speed);
  Wire.endTransmission();
  Serial.print("Speed:");
  Serial.println(speed);
  
    if (speed > 30) {
      speed = 0;
    } else {
      speed++;
    }
}
