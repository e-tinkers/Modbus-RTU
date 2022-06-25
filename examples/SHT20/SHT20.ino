
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

int16_t temp_offset = 0;
int16_t humidity_offset = 0;

RS485 rs485(Serial2, 16, 17, NOT_A_PIN, NOT_A_PIN);
Modbus mb(&rs485);

void setup() {

    rs485.begin(9600, SERIAL_8N1);

    Serial.begin(115200);
    Serial.println();

    mb.readHoldingRegister(SHT20_ID, SHT20_ADDRESS, 1);
    int16_t id = mb.getInt();
    Serial.print("Device Address: ");
    Serial.println(id);

    mb.readHoldingRegister(SHT20_ID, SHT20_BAUD_RATE, 1);
    int16_t br = mb.getInt();
    Serial.print("Baud Rate: ");
    Serial.println(br);

    mb.readHoldingRegister(SHT20_ID, SHT20_TEMPERATURE_OFFSET, 1);
    temp_offset = mb.getInt();
    Serial.print("Temperature Offset: ");
    Serial.println(temp_offset);

    mb.readHoldingRegister(SHT20_ID, SHT20_HUMIDITY_OFFSET, 1);
    humidity_offset = mb.getInt();
    Serial.print("Humidity Offset: ");
    Serial.println(humidity_offset);

    Serial.println("======================");

    rs485.end();
}

void loop() {

    rs485.begin(9600, SERIAL_8N1);
    delay(50);

    mb.readInputRegister(SHT20_ID, SHT20_TEMPERATURE, 1);
    float temperature = mb.getInt()/100.0;
    Serial.print("Temperature: ");
    Serial.println(temperature);
    // This particular sht20 modbus device seems require 50ms delay after each
    // sensor reading, otherwise both values are not correct
    delay(50);

    mb.readInputRegister(SHT20_ID, SHT20_HUMIDITY, 1);
    float humidity = mb.getInt()/100.0;
    Serial.print("Humidity: ");
    Serial.println(humidity);

    rs485.end();

    delay(10000);

}
