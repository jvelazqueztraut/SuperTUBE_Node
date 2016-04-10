#include <EEPROM.h>
#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>
RF24 radio(9, 10);               // nRF24L01(+) radio attached using Getting Started board
RF24Network network(radio);      // Network uses that radio
const uint16_t base_node = 00;   // Address of the main node in Octal format
const uint16_t this_node = 01;    // Address of our node in Octal format ( 04,031, etc)

struct payload_t {                  // Structure of our payload
  unsigned char totalPeopleInside;
  signed char deltaPeople;
};

#define TUBES 2
#define CHANNELS 3

#define TUBE1_R 2
#define TUBE1_G 3
#define TUBE1_B 4

#define TUBE2_R 5
#define TUBE2_G 6
#define TUBE2_B 7

#define REPETITIONS 3

typedef enum{
  FIXED=0,
  SINE,
  RANDOM,
  MEMORY,
  CONTROLLED
}TubeMode;

TubeMode mode=FIXED;

float value1RNow, value1GNow, value1BNow;
float value2RNow, value2GNow, value2BNow;

void setup() {
  Serial.begin(57600);
  Serial.println("SuperTUBE_Node");

  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);

  pinMode(TUBE1_R, OUTPUT);
  pinMode(TUBE1_G, OUTPUT);
  pinMode(TUBE1_B, OUTPUT);

  pinMode(TUBE2_R, OUTPUT);
  pinMode(TUBE2_G, OUTPUT);
  pinMode(TUBE2_B, OUTPUT);

  //writeEEPROM();
}

void loop() {
  network.update();                  // Check the network regularly
  while ( network.available() ) {     // Is there anything ready for us?
    RF24NetworkHeader header;        // If so, grab it and print it out
    payload_t payload;
    network.read(header, &payload, sizeof(payload));
    Serial.print("From node: ");
    Serial.print(header.from_node);
    Serial.print(", total people:");
    Serial.print(payload.totalPeopleInside);
    Serial.print(", delta people: ");
    Serial.println(payload.deltaPeople);
  }
  switch(mode){
    case FIXED:
      value1RNow=value2RNow=50;
      value1GNow=value2GNow=255;
      value1BNow=value2BNow=220;
      break;
    case SINE:
      sineTube();
      break;
    case RANDOM:
      randomTube();
      break;
    case MEMORY:
      memoryTube();
    case CONTROLLED:
      break;
    default:
      mode=MEMORY;
      break;  
  }
  
  lightShow(value1RNow,value1GNow,value1BNow,value2RNow,value2GNow,value2BNow);
}
void lightShow(float v1R, float v1G, float v1B, float v2R, float v2G, float v2B) {
  byte value1R = v1R;
  byte value1G = v1G;
  byte value1B = v1B;

  byte value2R = v2R;
  byte value2G = v2G;
  byte value2B = v2B;

  for (int r = 0; r < REPETITIONS; r++) {
    for (byte i = 0; i < 255; i++) {
      if (i < value1R)
        digitalWrite(TUBE1_R, HIGH);
      else
        digitalWrite(TUBE1_R, LOW);

      if (i < value1G)
        digitalWrite(TUBE1_G, HIGH);
      else
        digitalWrite(TUBE1_G, LOW);

      if (i < value1B)
        digitalWrite(TUBE1_B, HIGH);
      else
        digitalWrite(TUBE1_B, LOW);

      if (i < value2R)
        digitalWrite(TUBE2_R, HIGH);
      else
        digitalWrite(TUBE2_R, LOW);

      if (i < value2G)
        digitalWrite(TUBE2_G, HIGH);
      else
        digitalWrite(TUBE2_G, LOW);

      if (i < value2B)
        digitalWrite(TUBE2_B, HIGH);
      else
        digitalWrite(TUBE2_B, LOW);

      //delayMicroseconds(1);
    }
  }
}
void sineTube(){
  float seg = millis() / 1000.0f;
  value1RNow = 255.0f * 0.5f * (sin(seg * 1.5f + 1.0f) + 1.0f);
  value1GNow = 255.0f * 0.5f * (sin(seg * 1.0f + 0.0f) + 1.0f);
  value1BNow = 255.0f * 0.5f * (sin(seg * 0.8f + 2.0f) + 1.0f);
  
  value2RNow = 255.0f * 0.5f * (sin(seg * 1.2f + 2.0f) + 1.0f);
  value2GNow = 255.0f * 0.5f * (sin(seg * 0.5f + 1.0f) + 1.0f);
  value2BNow = 255.0f * 0.5f * (sin(seg * 1.0f + 0.0f) + 1.0f);
}

void randomTube(){
  value1RNow = random(10,255);
  value1GNow = random(10,255);
  value1BNow = random(10,255);

  value2RNow = random(10,255);
  value2GNow = random(10,255);
  value2BNow = random(10,255);
}

