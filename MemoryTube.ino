#define MAX_VALUES_PER_TUBE 127
#define TUBE2_OFFSET (MAX_VALUES_PER_TUBE*(CHANNELS+1))

int addr1 = 0;
int colors1 = 50;
float value1Duration = 1;
float value1RDiff, value1GDiff, value1BDiff;

int addr2 = 0;
int colors2 = 50;
float value2Duration = 1;
float value2RDiff, value2GDiff, value2BDiff;

void memoryTube() {
  if (value1Duration <= 0) {
    value1RNow = EEPROM.read(addr1*(CHANNELS+1) + 0);
    value1GNow = EEPROM.read(addr1*(CHANNELS+1) + 1);
    value1BNow = EEPROM.read(addr1*(CHANNELS+1) + 2);

    value1Duration = EEPROM.read(addr1 * (CHANNELS + 1) + 3);

    addr1 ++;
    if (addr1 >= colors1) {
      addr1 = 0;
    }

    if (value1Duration > 0) {
      value1RDiff = EEPROM.read(addr1*(CHANNELS+1) + 0) - value1RNow;
      value1GDiff = EEPROM.read(addr1*(CHANNELS+1) + 1) - value1GNow;
      value1BDiff = EEPROM.read(addr1*(CHANNELS+1) + 2) - value1BNow;

      value1RDiff /= value1Duration;
      value1GDiff /= value1Duration;
      value1BDiff /= value1Duration;
    }

    /*Serial.print("Real value1R ");
    Serial.println(EEPROM.read(addr1*(CHANNELS+1) + 0));
    Serial.print("Real value1R Next ");
    Serial.println(EEPROM.read((addr1+1)*(CHANNELS+1) + 0));
    Serial.print("value1RNow ");
    Serial.println(value1RNow);
    Serial.print("value1RDiff ");
    Serial.println(value1RDiff);
    Serial.print("value1Duration ");
    Serial.println(value1Duration);*/
  }
  else {
    value1RNow += value1RDiff;
    value1GNow += value1GDiff;
    value1BNow += value1BDiff;
  }
  value1Duration -= 1.0f;

  if (value2Duration <= 0) {
    value2RNow = EEPROM.read(TUBE2_OFFSET + addr2*(CHANNELS+1) + 0);
    value2GNow = EEPROM.read(TUBE2_OFFSET + addr2*(CHANNELS+1) + 1);
    value2BNow = EEPROM.read(TUBE2_OFFSET + addr2*(CHANNELS+1) + 2);

    value2Duration = EEPROM.read(TUBE2_OFFSET + addr2*(CHANNELS+1) + 3);

    addr2 ++;
    if (addr2 >= colors2) {
      addr2 = 0;
    }

    if (value2Duration > 0) {
      value2RDiff = EEPROM.read(TUBE2_OFFSET + addr2*(CHANNELS+1) + 0) - value2RNow;
      value2GDiff = EEPROM.read(TUBE2_OFFSET + addr2*(CHANNELS+1) + 1) - value2GNow;
      value2BDiff = EEPROM.read(TUBE2_OFFSET + addr2*(CHANNELS+1) + 2) - value2BNow;

      value2RDiff /= value2Duration;
      value2GDiff /= value2Duration;
      value2BDiff /= value2Duration;
    }
  }
  else {
    value2RNow += value2RDiff;
    value2GNow += value2GDiff;
    value2BNow += value2BDiff;
  }
  value2Duration -= 1.0f;
}
void writeEEPROM() {
  for (int v = 0; v < colors1; v++) {
    byte valR = random(255);
    byte valG = random(255);
    byte valB = random(255);
    float mean = 0.33f * valR + 0.33f * valG + 0.33 * valB;
    valR = constrain(mean + (valR - mean) * 1.5, 0, 255);
    valG = constrain(mean + (valG - mean) * 1.5, 0, 255);
    valB = constrain(mean + (valB - mean) * 1.5, 0, 255);
    EEPROM.write(v * (CHANNELS + 1) + 0, valR);
    EEPROM.write(v * (CHANNELS + 1) + 1, valG);
    EEPROM.write(v * (CHANNELS + 1) + 2, valB);
    //int duration = random(20, 50);
    int duration = PerlinNoise(10, 100);
    EEPROM.write(v * (CHANNELS + 1) + 3, duration);
    delay(random(1,10));
  }

  for (int v = 0; v < colors2; v++) {
    byte valR = random(255);
    byte valG = random(255);
    byte valB = random(255);
    float mean = 0.33f * valR + 0.33f * valG + 0.33 * valB;
    valR = constrain(mean + (valR - mean) * 1.5, 0, 255);
    valG = constrain(mean + (valG - mean) * 1.5, 0, 255);
    valB = constrain(mean + (valB - mean) * 1.5, 0, 255);
    EEPROM.write(TUBE2_OFFSET + v * (CHANNELS + 1) + 0, valR);
    EEPROM.write(TUBE2_OFFSET + v * (CHANNELS + 1) + 1, valG);
    EEPROM.write(TUBE2_OFFSET + v * (CHANNELS + 1) + 2, valB);
    //int duration = random(20, 50);
    int duration = PerlinNoise(10, 100);
    EEPROM.write(TUBE2_OFFSET + v * (CHANNELS + 1) + 3, duration);
    delay(random(1,10));
  }
}
