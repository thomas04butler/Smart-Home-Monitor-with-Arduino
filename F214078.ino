#include <EEPROM.h>
#include <Wire.h>  // Required for Adafruit_RGBLCDShield
#include <Adafruit_RGBLCDShield.h>
#define COUNTER_ADDRESS 0

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

struct Device {
  String id;
  String location;
  byte power;
  String type;
  String state;
  String change_state;
  byte change_power;
};

//creates UDCHAR for down arrow
byte downArrow[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
  B00000
};

//creates UDCHAR for up arrow
byte upArrow[8] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100,
  B00000
};


Device device;

#define MAX_LENGTH 23
#define MAX_LENGTH1 50
int state = 0;
int firstDevice = 0;
int firstDevice1 = 0;
int currentDevice = 0;
const int buttonPin = 2;
const int topButtonPin = 3;
int numDevices = EEPROM.read(0); 

void writeDeviceToEEPROM(Device &device, int value1) {
  int value2 = EEPROM.read(0); 
  int startAddress = 24 * value2 + 1;
  // write id
  for (int i = 0; i < 3; i++) {
    EEPROM.write(startAddress + i, device.id[i]);
  }
  // write location
  for (int i = 0; i < device.location.length(); i++) {
    if (i < 15) {
      EEPROM.write(startAddress + 4 + i, device.location[i]);
    } else {
      break;
    }
  }
  // pad location with null values
  for (int i = device.location.length(); i < 15; i++) {
    EEPROM.write(startAddress + 4 + i, 0);
  }
  // write type
  EEPROM.write(startAddress + 19, device.type[0]); // store only the first letter of the type
  // write state
  for (int i = 0; i < device.state.length(); i++) {
    if (i < 3) {
      EEPROM.write(startAddress + 20 + i, device.state[i]);
    } else {
      break;
    }
  }
  // pad state with null values
  for (int i = device.state.length(); i < 3; i++) {
    EEPROM.write(startAddress + 20 + i, 0);
  }
  // write power
  EEPROM.put(startAddress + 23, device.power);
}

void readDeviceFromEEPROM(Device &device, int val1) { 
  int startAddress = 24 * val1 + 1;
  // read id
  char buffer[4];
  for (int i = 0; i < 3; i++) {
    buffer[i] = EEPROM.read(startAddress + i);
  }
  buffer[3] = 0;
  device.id = String(buffer);
  // read location
  int i = 0;
  while (true) {
    char c = EEPROM.read(startAddress + 4 + i);
    if (c == 0 || i >= 15) {
      break;
    }
    buffer[i] = c;
    i++;
  }
  buffer[i] = 0;
  device.location = String(buffer);
  // read type
  char typeChar = EEPROM.read(startAddress + 19);
  device.type = String(typeChar);
  // read state
  i = 0;
  while (true) {
    char c = EEPROM.read(startAddress + 20 + i);
    if (c == 0 || i >= 3) {
      break;
    }
    buffer[i] = c;
    i++;
  }
  buffer[i] = 0;
  device.state = String(buffer);
  // read power
  EEPROM.get(startAddress + 23, device.power);
}

void seeAllfunctions(int numDevices) {
  for (int i1 = 0; i1 < numDevices+1; i1++) {
    int startAddress = 24 * i1 + 1;
    Device device;
    // read id
    char buffer[4];
    for (int i = 0; i < 3; i++) {
      buffer[i] = EEPROM.read(startAddress + i);
    }
    buffer[3] = 0;
    device.id = String(buffer);
    // read location
    int i = 0;
    while (true) {
      char c = EEPROM.read(startAddress + 4 + i);
      if (c == 0 || i >= 15) {
        break;
      }
      buffer[i] = c;
      i++;
    }
    buffer[i] = 0;
    device.location = String(buffer);
    // read type
    char typeChar = EEPROM.read(startAddress + 19);
    device.type = String(typeChar);
    // read state
    i = 0;
    while (true) {
      char c = EEPROM.read(startAddress + 20 + i);
      if (c == 0 || i >= 3) {
        break;
      }
      buffer[i] = c;
      i++;
    }
    buffer[i] = 0;
    device.state = String(buffer);
    // read power
    EEPROM.get(startAddress + 23, device.power);
    
    // print device info
    if(device.id != NULL || device.id != ""){
      Serial.print(" ID: ");
      Serial.print(device.id);
      Serial.print(", Location: ");
      Serial.print(device.location);
      Serial.print(", Type: ");
      Serial.print(device.type);
      Serial.print(", State: ");
      Serial.print(device.state);
      Serial.print(", Power: ");
      Serial.println(device.power);
    }
  } 
}

void viewAllDevices(int currentDevice) {
  // read device information from EEPROM
  Device device;
  int startAddress = 24 * currentDevice + 1;
  // read id
  char buffer[4];
  for (int j = 0; j < 3; j++) {
    buffer[j] = EEPROM.read(startAddress + j);
  }
  buffer[3] = 0;
  device.id = String(buffer);
  // read location
  int j = 0;
  while (true) {
    char c = EEPROM.read(startAddress + 4 + j);
    if (c == 0 || j >= 15) {
      break;
    }
    buffer[j] = c;
    j++;
  }
  buffer[j] = 0;
  device.location = String(buffer);
  // read type
  char typeChar = EEPROM.read(startAddress + 19);
  device.type = String(typeChar);
  // read state
  j = 0;
  while (true) {
    char c = EEPROM.read(startAddress + 20 + j);
    if (c == 0 || j >= 3) {
      break;
    }
    buffer[j] = c;
    j++;
  }
  buffer[j] = 0;
  device.state = String(buffer);
  // read power
  EEPROM.get(startAddress + 23, device.power);
  if (device.state == "OFF"){
    lcd.setBacklight(3);
  }  
  else if (device.state == "ON"){
    lcd.setBacklight(2);
  }

  if(device.id != NULL || device.id != ""){
    lcd.clear();
    if (currentDevice<numDevices){ 
      lcd.write(byte(1));
    }
    else{
      lcd.print(" ");
    }
    lcd.print(device.id);
    lcd.print(" ");
    lcd.print(device.location);
    lcd.setCursor(0, 1);
    if (currentDevice == 1){
      lcd.print(" ");
    }
    else{
      lcd.write(byte(0));
    }
    lcd.print(device.type);
    lcd.print(" ");
    lcd.print(device.state);
    lcd.print(" ");
    if (device.type == "S" || device.type == "L" || device.type == "T"){
      lcd.print(String(device.power));
      if (device.type == "S" || device.type == "L"){
        lcd.print("%");
      }
      else{
        lcd.print(char(0xDF));
        lcd.print("C");
      }
    }
  }
}

void viewAllDevices2(int currentDevice){
  // read device information from EEPROM
  Device device;
  int startAddress = 24 * currentDevice + 1;
  // read id
  char buffer[4];
  for (int j = 0; j < 3; j++) {
    buffer[j] = EEPROM.read(startAddress + j);
  }
  buffer[3] = 0;
  device.id = String(buffer);
  // read location
  int j = 0;
  while (true) {
    char c = EEPROM.read(startAddress + 4 + j);
    if (c == 0 || j >= 15) {
      break;
    }
    buffer[j] = c;
    j++;
  }
  buffer[j] = 0;
  device.location = String(buffer);
  // read type
  char typeChar = EEPROM.read(startAddress + 19);
  device.type = String(typeChar);
  // read state
  j = 0;
  while (true) {
    char c = EEPROM.read(startAddress + 20 + j);
    if (c == 0 || j >= 3) {
      break;
    }
    buffer[j] = c;
    j++;
  }
  buffer[j] = 0;
  device.state = String(buffer);
  // read power
  EEPROM.get(startAddress + 23, device.power);
  if(device.state == "ON"){
    lcd.setBacklight(2);
  }

  if(device.id != NULL || device.id != ""){
    if(device.state == "ON"){
      lcd.clear();
      lcd.write(byte(1));
      lcd.print(device.id);
      lcd.print(" ");
      lcd.print(device.location);
      lcd.setCursor(0, 1);
      if (currentDevice == 1){
        lcd.print(" ");
      }
      else{
        lcd.write(byte(0));
      }
      lcd.print(device.type);
      lcd.print(" ");
      lcd.print(device.state);
      lcd.print(" ");
      if (device.type == "S" || device.type == "L" || device.type == "T"){
        lcd.print(String(device.power));
        if (device.type == "S" || device.type == "L"){
          lcd.print("%");
        }
        else{
          lcd.print(char(0xDF));
          lcd.print("C");
        }
      }
    }
  }
  /*else{
    viewAllDevices(firstDevice + 1);
    firstDevice = firstDevice + 1;
  }*/
}

void changeStatus(int numDevices) {
  int value2 = EEPROM.read(0); 
  //loops through all devices in EEPROM to find the device that has teh ID you are looking for 
  for (int i1 = 0; i1 < value2+1; i1++) {
    int startAddress = 24 * i1 + 1;
    char a = EEPROM.read(startAddress);
    char b = EEPROM.read(startAddress + 1);
    char c = EEPROM.read(startAddress + 2);
    // this creates a string of the variable of the device ID in the EEPROM
    String tempid = String(a) + String(b) + String(c);
    //this compares whether the device IDs matc 
    if(tempid == device.id){
      String change = device.change_state;
      if(change == "ON"){
        char data[] = "ON";
        EEPROM.put(startAddress + 20, data);
        EEPROM.update(startAddress + 22, NULL);
        Serial.println("Status Changed");
      }
      else if(change == "OFF"){
        char data[] = "OFF";
        EEPROM.put(startAddress + 20, data);
        Serial.println("Status Shanged");
      }

    }
  }
}

void changePower(int numDevices) {
  int value2 = EEPROM.read(0); 
  for (int i1 = 0; i1 < value2+1; i1++) { // loop through all the stored devices
    int startAddress = 24 * i1 + 1; // calculate the starting address of the current device
    char a = EEPROM.read(startAddress);
    char b = EEPROM.read(startAddress + 1); 
    char c = EEPROM.read(startAddress + 2); 
    char d = EEPROM.read(startAddress + 3); 
    String tempid = String(a) + String(b) + String(c); // combine the first three characters to create the device ID
    if(tempid == device.id){ // check if the current device ID matches the ID of intended device
      if (d == "L" || "S"){ // if the device is a light or a socket
        if (device.change_power >=0 && device.change_power<101){ // check if the new power value is within the range of 0-100
          int change = device.change_power; 
          EEPROM.update(startAddress + 23, change); // update the power value in EEPROM
          Serial.println("Power Changed"); 
        }
        else{
          Serial.println("number not compatible"); 
        }
      }
      else if (d == "T"){ // if the device is a thermostat
        if (device.change_power >=9 && device.change_power<33){ // check if the new power value is within the range of 9-32
          int change = device.change_power; 
          EEPROM.update(startAddress + 23, change); // update the power value in EEPROM
          Serial.println("Power Changed"); 
        }
        else{
          Serial.println("number not compatible"); 
        }
      } 
    }
  }
}


void removeDevice(int numDevices) {
  // Read the value stored in the first address of EEPROM memory.
  int value2 = EEPROM.read(0);
  // Loop through all devices stored in EEPROM memory.
  for (int i1 = 0; i1 < value2+1; i1++) {
    // Calculate the starting address of the current device.
    int startAddress = 24 * i1 + 1;
    // Read the id of the current device from EEPROM memory.
    char a = EEPROM.read(startAddress);
    char b = EEPROM.read(startAddress + 1);
    char c = EEPROM.read(startAddress + 2);
    String tempid = String(a) + String(b) + String(c);
    // Check if the id of the current device matches the id of the device to be removed.
    if(tempid == device.id){
      // If the id matches, remove the device by overwriting its memory with NULL values.
      for(int i2 = 0; i2 < 24; i2++){
        EEPROM.update(startAddress + i2, NULL);
        Serial.println("Device removed");
      }
    }
  }
}

void setup() {
  currentDevice = 0;
  Serial.begin(9600);
  Serial.println("Enter operation (A-, S-, P-, or R-):");
  lcd.begin(16, 2);
  int val1 = 0;
  EEPROM.update(0, val1);
  Serial.begin(9600);
  int value = EEPROM.read(0);
  int numDevices = value;
  int value1 = EEPROM.read(0);
  int num = EEPROM.read(0);
  lcd.createChar(0, downArrow);
  lcd.createChar(1, upArrow);
}

void loop() {
  switch(state){
    case 0:
      lcd.setBacklight(5);
      lcd.print("Q");
      if (Serial.available()) {
        String operation = Serial.readStringUntil('\n');
        operation.trim();
        if(operation == "X"){
          state =1;
          break;
        }
      } 
      delay(1000);
      break;
    case 1:
      lcd.setBacklight(255);
      lcd.clear();
      int numDevices1 = EEPROM.read(0);
      if(numDevices1 == 0){
        lcd.print("No Devices");
        lcd.setCursor(0, 1);
        lcd.print("Connected");
        delay(1000); 
      }
      uint8_t pressedButtons = lcd.readButtons();
      switch(pressedButtons) {
        case BUTTON_UP:
          firstDevice = firstDevice+1;
          if(firstDevice<=numDevices1){
            viewAllDevices(firstDevice);
            delay(1000);
            break;
          }
          else{
            firstDevice = -0;
          }
        break;
        case BUTTON_DOWN:
          firstDevice = firstDevice-1;
          if (firstDevice>0){
            Device device;
            int startAddress = 24 * firstDevice + 1;
            // read id
            char buffer[4];
            for (int j = 0; j < 3; j++) {
              buffer[j] = EEPROM.read(startAddress + j);
            }
            buffer[3] = 0;
            device.id = String(buffer);
            // read location
            int j = 0;
            while (true) {
              char c = EEPROM.read(startAddress + 4 + j);
              if (c == 0 || j >= 15) {
                break;
              }
              buffer[j] = c;
              j++;
            }
            buffer[j] = 0;
            device.location = String(buffer);
            // read type
            char typeChar = EEPROM.read(startAddress + 19);
            device.type = String(typeChar);
            // read state
            j = 0;
            while (true) {
              char c = EEPROM.read(startAddress + 20 + j);
              if (c == 0 || j >= 3) {
                break;
              }
              buffer[j] = c;
              j++;
            }
            buffer[j] = 0;
            device.state = String(buffer);
            // read power
            EEPROM.get(startAddress + 23, device.power);
            if (device.state == "OFF"){
              lcd.setBacklight(3);
            }  
            else if (device.state == "ON"){
              lcd.setBacklight(2);
            }

            if(device.id != NULL || device.id != ""){
              lcd.clear();
              lcd.write(byte(1));
              lcd.print(device.id);
              lcd.print(" ");
              lcd.print(device.location);
              lcd.setCursor(0, 1);
              if (firstDevice == 1){
                lcd.print(" ");
              }
              else{
                lcd.write(byte(0));
              }
              lcd.print(device.type);
              lcd.print(" ");
              lcd.print(device.state);
              lcd.print(" ");
              if (device.type == "S" || device.type == "L" || device.type == "T"){
                lcd.print(String(device.power));
                if (device.type == "S" || device.type == "L"){
                  lcd.print("%");
                }
                else{
                  lcd.print(char(0xDF));
                  lcd.print("C");
                }
              }
            }
            /*else{
              viewAllDevices(firstDevice + 1);
              firstDevice = firstDevice + 1;
            }*/
            delay(1000);
            break;
          }
          else{
            firstDevice = 0;
          }
        case BUTTON_RIGHT:
          lcd.setBacklight(255);
          firstDevice1 = firstDevice1+1;
          if(firstDevice1<=numDevices1){
            viewAllDevices2(firstDevice1);
            delay(1000);
            break;
          }
          else{
            firstDevice1 = 0;
          }
        break;
        case BUTTON_LEFT:
          lcd.print("LEFT");
          delay(300);
          break;
      } 
      char temp[24];

      if (Serial.available()) {

        String operation = Serial.readStringUntil('\n');
        // Read the incoming serial data until a new line character is found

        for (int i = 0; i < MAX_LENGTH; i++) {
          if (i < operation.length()) {
            temp[i] = operation.charAt(i);
          } else {
            temp[i] = '\0';
          }
        }
        // Copy the incoming data into a char array 'temp' of size MAX_LENGTH

        if (temp[0] == 'A' && temp[1] == '-' && temp[2] >= 'A' && temp[2] <= 'Z' && temp[3] >= 'A' && temp[3] <= 'Z' && temp[4] >= 'A' && temp[4] <= 'Z' && temp[5] <= '-' && temp[7] <= '-') {
          // Check if the incoming data has the correct format for adding a device
          if (temp[6] == 'S'){ 
            // Check if the device type is a speaker
            // Increase the number of devices stored in EEPROM and update the value
            int value = EEPROM.read(0);
            int numDevices = value;
            value = value + 1;
            EEPROM.update(0, value);

            Serial.println("Your adding speaker");
            // Get the device location from the incoming data
            String device_location = "";
            for (int i = 8; i < MAX_LENGTH; i++) {
              if (temp[i] == '\0') {
                break;
              }
              device_location += temp[i];
            }
            // Create a new device object with the extracted information and write it to EEPROM
            device.location = device_location;
            device.type = "S";
            device.id  = String(temp[2]) + String(temp[3]) + String(temp[4]);
            Serial.println(String(temp[2]+String(temp[3]) + String(temp[4])));
            device.state = "OFF";
            device.power = 0;
            int value3 = EEPROM.read(0);
            value3 = value3 + 1;
            EEPROM.update(0, value);
            writeDeviceToEEPROM(device, 0);
          }

          else if (temp[6] == 'O'){ 
            // Check if the device type is a socket
            // Increase the number of devices stored in EEPROM and update the value
            int value = EEPROM.read(0);
            int numDevices = value;
            value = value + 1;
            EEPROM.update(0, value);
            Serial.println("Your adding socket");
            String device_location = "";
            for (int i = 8; i < MAX_LENGTH; i++) {
              if (temp[i] == '\0') {
                break;
              }
              device_location += temp[i];
            }
            // Create a new device object with the extracted information and write it to EEPROM
            device.location = device_location;
            device.type = "O";
            device.id  = String(temp[2]) + String(temp[3]) + String(temp[4]);
            Serial.println(String(temp[2]+String(temp[3]) + String(temp[4])));
            device.state = "OFF";
            device.power = 0;
            int value3 = EEPROM.read(0);
            value3 = value3 + 1;
            EEPROM.update(0, value);
            writeDeviceToEEPROM(device, 0);
          }

          else if (temp[6] == 'L'){ 
            int value = EEPROM.read(0);
            int numDevices = value;
            value = value + 1;
            EEPROM.update(0, value);
            Serial.println("Your adding light");
            String device_location = "";
            for (int i = 8; i < MAX_LENGTH; i++) {
              if (temp[i] == '\0') {
                break;
              }
              device_location += temp[i];
            }
            device.location = device_location;
            device.type = "L";
            device.id  = String(temp[2]) + String(temp[3]) + String(temp[4]);
            Serial.println(String(temp[2]+String(temp[3]) + String(temp[4])));
            device.state = "OFF";
            device.power = 0;
            int value3 = EEPROM.read(0);
            value3 = value3 + 1;
            EEPROM.update(0, value);
            writeDeviceToEEPROM(device, 0);
          }

          else if (temp[6] == 'T'){
            int value = EEPROM.read(0);
            int numDevices = value;
            value = value + 1;
            EEPROM.update(0, value); 
            Serial.println("Your adding thermostat");
            String device_location = "";
            for (int i = 8; i < MAX_LENGTH; i++) {
              if (temp[i] == '\0') {
                break;
              }
              device_location += temp[i];
            }
            device.location = device_location;
            device.type = "T";
            device.id  = String(temp[2]) + String(temp[3]) + String(temp[4]);
            Serial.println(String(temp[2]+String(temp[3]) + String(temp[4])));
            device.state = "OFF";
            device.power = 0;
            int value3 = EEPROM.read(0);
            value3 = value3 + 1;
            EEPROM.update(0, value);
            writeDeviceToEEPROM(device, 0);
          }

          else if (temp[6] == 'C'){ 
            Serial.println("Your adding camera");
            String device_location = "";
            for (int i = 8; i < MAX_LENGTH; i++) {
              if (temp[i] == '\0') {
                break;
              }
              device_location += temp[i];
            }
            device.location = device_location;
            device.type = "C";
            device.id  = String(temp[2]) + String(temp[3]) + String(temp[4]);
            Serial.println(String(temp[2]+String(temp[3]) + String(temp[4])));
            device.state = "OFF";
            device.power = 0;
            int value = EEPROM.read(0);
            value = value + 1;
            EEPROM.update(0, value);
            writeDeviceToEEPROM(device, 0);
          }
        }

        if (temp[0] == 'S' && temp[1] == '-' && temp[2] >= 'A' && temp[2] <= 'Z' && temp[3] >= 'A' && temp[3] <= 'Z' && temp[4] >= 'A' && temp[4] <= 'Z' && temp[5] <= '-') {
          // If the message starts with "S-", followed by 3 capital letters, and then a hyphen, then it is a state change message for a device.
          
          // Extract the device ID from the message.
          String device_id = "";
          for (int i = 2; i < 5; i++) {
            if (temp[i] == '\0') {
              break;
            }
            device_id += temp[i];
          }
          device.id = device_id;

          // Extract the new state for the device from the message.
          String device_change_state = "";
          for (int i = 6; i < MAX_LENGTH1; i++) {
            if (temp[i] == '\0') {
              break;
            }
            device_change_state += temp[i];
          }
          device.change_state = device_change_state;

          // Call the function to change the status of the device.
          changeStatus(0);
        }

        if (temp[0] == 'P' && temp[1] == '-' && temp[2] >= 'A' && temp[2] <= 'Z' && temp[3] >= 'A' && temp[3] <= 'Z' && temp[4] >= 'A' && temp[4] <= 'Z' && temp[5] <= '-') {
          // If the message starts with "P-", followed by 3 capital letters, and then a hyphen, then it is a power change message for a device.
          
          // Extract the device ID from the message.
          String device_id = "";
          for (int i = 2; i < 5; i++) {
            if (temp[i] == '\0') {
              break;
            }
            device_id += temp[i];
          }
          device.id = device_id;

          // Extract the new power level for the device from the message.
          String device_change_power = "";
          for (int i = 6; i < 9; i++) {
            if (temp[i] == '\0') {
              break;
            }
            device_change_power += temp[i];
          }
          int num = device_change_power.toInt();
          device.change_power = num;

          // Call the function to change the power level of the device.
          changePower(0);
        }

        if (temp[0] == 'R' && temp[1] == '-' && temp[2] >= 'A' && temp[2] <= 'Z' && temp[3] >= 'A' && temp[3] <= 'Z' && temp[4] >= 'A' && temp[4] <= 'Z') {
          // If the message starts with "R-", followed by 3 capital letters, then it is a message to remove a device from the system.
          
          // Extract the device ID from the message.
          String device_id = "";
          for (int i = 2; i < 5; i++) {
            if (temp[i] == '\0') {
              break;
            }
            device_id += temp[i];
          }
          device.id = device_id;

          // Call the function to remove the device from the system.
          removeDevice(0);
        }
      }
    }
  }