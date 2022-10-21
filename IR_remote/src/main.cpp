#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial softSerial(10, 11); // RX, TX


const uint8_t button_1[] = {0x01, 0x11, 0x11, 0x11, 0x12, 0x22, 0x22, 0x22, 0x21, 0x12, 0x21, 0x11, 0x12, 0x21, 0x12, 0x22, 0x23, 0x45};
const uint8_t button_2[] = {0x01, 0x11, 0x11, 0x11, 0x12, 0x22, 0x22, 0x22, 0x21, 0x11, 0x22, 0x11, 0x12, 0x22, 0x11, 0x22, 0x23, 0x45};
const uint8_t button_3[] = {0x01, 0x11, 0x11, 0x11, 0x12, 0x22, 0x22, 0x22, 0x21, 0x22, 0x22, 0x12, 0x12, 0x11, 0x11, 0x21, 0x23, 0x45};
const uint8_t button_4[] = {0x01, 0x11, 0x11, 0x11, 0x12, 0x22, 0x22, 0x22, 0x21, 0x11, 0x21, 0x11, 0x12, 0x22, 0x12, 0x22, 0x23, 0x45};
const uint8_t button_logo[] = {0x01, 0x11, 0x11, 0x11, 0x12, 0x22, 0x22, 0x22, 0x22, 0x12, 0x12, 0x11, 0x11, 0x21, 0x21, 0x22, 0x23, 0x45};

uint8_t channel = 0x01;
const uint8_t IR_05T_learn_init[] = {0xFD, 0xFD, 0xF1, 0xF2, 0xDF};
uint8_t read_buffer[256] = {0x00};
//////////////IR_03T
uint8_t IR_03T_learn_init[] = {0xFA, 0xFD, 0x01, 0x01, 0xDF}; //->A1, success A2;
uint8_t IR_03T_read_data[] = {0xFA, 0xFD, 0x06, 0x01, 0xDF};
//////////////
bool available_learn_flag1 = true;
bool available_learn_flag2 = true;
bool available_learn_flag3 = true;
bool available_learn_flag4 = true;

String ans = "";

void find_command(uint8_t * data){
  if (data[40] == 0x01) {
    
    if (memcmp(&data[40], button_1, 18) == 0)
      ans = "button_1";
    else if (memcmp(&data[40], button_2, 18) == 0)
      ans = "button_2";
    else if (memcmp(&data[40], button_3, 18) == 0)
      ans = "button_3";
    else if (memcmp(&data[40], button_4, 18) == 0)
      ans = "button_4";
    else if (memcmp(&data[40], button_logo, 18) == 0)
      ans = "button_logo";
    else
      ans = "invalid";
    Serial.print(ans+"\n");
  }
  else
    ans = "Error";
  
}

uint8_t* IR_05T_read_button(HardwareSerial &chip, bool &flag){
  if (chip.peek() == 0xFD) {
    flag = true;
    size_t size = chip.readBytes(read_buffer, 236);
    // for (size_t i = 0; i < size; i++) {
    //   Serial.print(read_buffer[i], HEX);
    // }
    // Serial.println();
    return read_buffer;
  }
  return nullptr;
}

void IR_05T_write_learn_state(HardwareSerial &chip, bool &flag){
  if (flag == true){
    chip.write(IR_05T_learn_init, 5);
    flag = false;
  }
}

uint8_t* IR_03T_read_button(HardwareSerial &chip, bool &flag){
  chip.flush();
  chip.write(IR_03T_read_data, 5);
  // Serial.println("================================================================");
  while (chip.available() <= 0){;}
  // Serial.println("=======================AVAILABLE=========================");
  if (chip.peek() == 0xFA){
    flag = true;
    size_t size = chip.readBytes(read_buffer, 236);
    // for (size_t i = 0; i < size; i++) {
    //   Serial.print(read_buffer[i], HEX);
    // }
    // Serial.println();
    return read_buffer;
  }
  return nullptr;
}

void IR_03T_write_learn_state(HardwareSerial &chip, bool &flag){
  if (flag == true){
    chip.write(IR_03T_learn_init, 5);
    // Serial.println("write");
    flag = false;
  }
}

uint8_t* soft_IR_05T_read_button(SoftwareSerial &chip, bool &flag){
  if (chip.peek() == 0xFD) {
    flag = true;
    size_t size = chip.readBytes(read_buffer, 236);
    // for (size_t i = 0; i < size; i++) {
    //   Serial.print(read_buffer[i], HEX);
    // }
    // Serial.println();
    return read_buffer;
  }
  return nullptr;
}

void soft_IR_05T_write_learn_state(SoftwareSerial &chip, bool &flag){
  if (flag == true){
    chip.write(IR_05T_learn_init, 5);
    flag = false;
  }
}

uint8_t* soft_IR_03T_read_button(SoftwareSerial &chip, bool &flag){
  chip.flush();
  chip.write(IR_03T_read_data, 5);
  // Serial.println("================================================================");
  while (chip.available() <= 0){;}
  // Serial.println("=======================AVAILABLE=========================");
  // Serial.print(chip.peek(), HEX);
  // if (chip.peek() == 0xFA){
    flag = true;
    size_t size = chip.readBytes(read_buffer, 236);
    // for (size_t i = 0; i < size; i++) {
    //   Serial.print(read_buffer[i], HEX);
    // }
    // Serial.println(size);
    return read_buffer;
  // }
  return nullptr;
}

void soft_IR_03T_write_learn_state(SoftwareSerial &chip, bool &flag){
  if (flag == true){
    chip.write(IR_03T_learn_init, 5);
    // Serial.println("write");
    flag = false;
  }
}

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  while (!Serial){;}
  Serial1.end();
  Serial2.end();
  Serial3.end();
  softSerial.end();
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  softSerial.begin(9600);
  Serial.print("hello\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  IR_05T_write_learn_state(Serial1, available_learn_flag1);
  IR_05T_write_learn_state(Serial2, available_learn_flag2);
  IR_03T_write_learn_state(Serial3, available_learn_flag3);
  soft_IR_03T_write_learn_state(softSerial, available_learn_flag4);

  if (Serial1.available() > 0) {
    find_command(IR_05T_read_button(Serial1, available_learn_flag1));
    // if (ans != "invalid")
      // Serial.println("Serial1: "+ ans);
        // Serial.println(ans);
  }
  if (Serial2.available() > 0) {
    find_command(IR_05T_read_button(Serial2, available_learn_flag2));
    // if (ans != "invalid")
      // Serial.println("Serial2: "+ ans);
        // Serial.println(ans);
  }
  
  if (Serial3.available() > 0){
    // Serial.println("====================================================in=====");
    if (Serial3.read() == 0xA2) {
      // Serial.println("check");
      delay(55);     //delay required
      find_command(IR_03T_read_button(Serial3, available_learn_flag3));
      // if (ans != "invalid")
        // Serial.println("Serial3: "+ ans);
        // Serial.println(ans);
    }
  }
  if (softSerial.available() > 0){
    // Serial.println("====================================================in=====");
    if (softSerial.read() == 0xA2) {
      // Serial.println("check");
      delay(100);     //delay required
      find_command(soft_IR_03T_read_button(softSerial, available_learn_flag4));
      // if (ans != "invalid")
        // Serial.println("softSerial: "+ ans);
        
    }
  }
  // IR_03T_read_button(Serial3, available_learn_flag3);
}