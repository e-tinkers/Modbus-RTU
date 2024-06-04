/* This sketch enable the debug print by passing in DEBUG_ON flag into Modbus::begin().
 * This will print out the raw dat transmitted and received and is useful for debugging Modbus communicaiton.
 * Debug Print Out example: 
 *    TX ==> [ 08 04 00 02 00 01 90 93 ]
 *    RX <== [ 08 04 02 1C D2 ED AC ]
 * The example also shows the use of error handling API for showing the potential Modubus Excption, network
 * timeout and crc errors if such error occured.
**/
#include <Arduino.h>
#include <RS485.h>
#include <Modbus-RTU.h>

#define RXD 16
#define TXD 17
#define EN  5

// Enable Debug Print will print out raw Modbus TX and RX packets
#define DEBUG_ON                     1

//SHT20 Modbus
#define SHT20_ID                     8
// SHT20 Input Registers
#define SHT20_TEMPERATURE            0x0001
#define SHT20_HUMIDITY               0x0002
// SHT20 Holding Registers
#define SHT20_ADDRESS                0x0101
#define SHT20_BAUD_RATE              0x0102
#define SHT20_TEMPERATURE_OFFSET     0x0103
#define SHT20_HUMIDITY_OFFSET        0x0104

RS485 rs485(Serial2, 16, 17, EN, EN);  // Refer to RS485 Library README for configuration
Modbus mb(&rs485);

void setup() {

    rs485.begin(9600, SERIAL_8N1);
    mb.begin(SHT20_ID, DEBUG_ON);  // pass-in DEBUG_ON flag

    Serial.begin(115200);
    Serial.println();

    // this help to identify the network error and modbus exception
    if (!mb.readHoldingRegister(SHT20_ADDRESS, 1)) {
        Serial.println(mb.errorMsg());
    }
    else {
        int16_t id = mb.getInt();
        Serial.print("Device Address: ");
        Serial.println(id);
    }

    if (!mb.readHoldingRegister(SHT20_BAUD_RATE, 1)) {
        Serial.println(mb.errorMsg());       
    }
    else {
        int16_t br = mb.getInt();
        Serial.print("Baud Rate: ");
        Serial.println(br);
    }

    Serial.println("======================");

}

void loop() {

    if (!mb.readInputRegister(SHT20_TEMPERATURE, 1)) {
        Serial.println(mb.errorMsg());         
    }
    else {
        float temperature = mb.getInt()/100.0;
        Serial.print("Temperature: ");
        Serial.println(temperature);
    }

    // This particular sht20 modbus device seems require 100ms delay after each
    // sensor reading, otherwise both values are not correct
    delay(100);

    if (!mb.readInputRegister(SHT20_HUMIDITY, 1)) {
        Serial.println(mb.errorMsg());         
    }
    else {
        float humidity = mb.getInt()/100.0;
        Serial.print("Humidity: ");
        Serial.println(humidity);
    }

    delay(10000);

}