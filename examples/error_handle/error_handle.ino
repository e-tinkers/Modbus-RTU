
#define SHOW_RAW_DATA  // This will let the library to print out raw data to be sent and the received data

#include <Arduino.h>
#include <RS485.h>
#include <Modbus-RTU.h>

//SHT20 Modbus
#define SHT20_ID                     1
// SHT20 Input Registers
#define SHT20_TEMPERATURE            0x0001
#define SHT20_HUMIDITY               0x0002
// SHT20 Holding Registers
#define SHT20_ADDRESS                0x0101
#define SHT20_BAUD_RATE              0x0102
#define SHT20_TEMPERATURE_OFFSET     0x0103
#define SHT20_HUMIDITY_OFFSET        0x0104

RS485 rs485(Serial2, 16, 17, NOT_A_PIN, NOT_A_PIN);
Modbus mb(&rs485);

void setup() {

    rs485.begin(9600, SERIAL_8N1);

    Serial.begin(115200);
    Serial.println();

    // Raw data will be print out for the send request as well as the received response
    // This is very useful during development for debugging purpose
    Serial.print("Read SHT20 Address:      ");
    mb.readHoldingRegister(SHT20_ID, SHT20_ADDRESS, 1);

    Serial.print("Read SHT20 Baud Rate:    ");
    mb.readHoldingRegister(SHT20_ID, SHT20_BAUD_RATE, 1);

    Serial.print("Read Temperature Offset: ");
    mb.readHoldingRegister(SHT20_ID, SHT20_TEMPERATURE_OFFSET, 1);

    Serial.print("Read Humidity Offset:    ");
    mb.readHoldingRegister(SHT20_ID, SHT20_HUMIDITY_OFFSET, 1);

    Serial.println("=========================");

}

void loop() {

    int ret = 0;
    
    // All Modbus-RTU read or write register(s) methods return either received length or 0 (when error occured)
    ret = mb.readInputRegister(SHT20_ID, SHT20_TEMPERATURE, 1);
    if (ret == 0) {
        md.printModbusExcpetion(); // find out the cause of the exception
    }
    else {
        float temperature = mb.getInt()/100.0;
        Serial.print("Temperature: ");
        Serial.println(temperature);
    }

    delay(100);

    ret = mb.readInputRegister(SHT20_ID, SHT20_HUMIDITY, 1);
    if (ret == 0) {
        md.printModbusExcpetion();
    }
    else {
        float humidity = mb.getInt()/100.0;
        Serial.print("Humidity: ");
        Serial.println(humidity);
    }

    delay(10000);

}
