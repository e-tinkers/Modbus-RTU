#include <Arduino.h>
#include <RS485.h>
#include <Modbus-RTU.h>

// DDS238
#define DDS238_ID                1
// DDS238 Input Registers
#define DDS238_TOTAL_ENERGY      0x0000  // int32(DWord), divide by 100 for kWh
#define DDS238_OUTPUT_ENERGY     0x0008  // int32(DWord), divide by 100 for kWh
#define DDS238_INPUT_ENERGY      0x000A  // int32(DWord), divide by 100 for kWh
#define DDS238_VOLTAGE           0x000C  // int, divide by 10 for volts
#define DDS238_CURRENT           0x000D  // int, divide by 100 for ampere
#define DDS238_ACTIVE_POWER      0x000E  // int, W
#define DDS238_REACTIVE_POWER    0x000F  // int, Var
#define DDS238_POWER_FACTOR      0x0010  // divide by 1000
#define DDS238_FREQUENCY         0x0011  // divide by 100
// DDS238 Holding registers
#define DDS238_COMMUNICATION     0x0015  // Higher byte = address (0-247) default 1
                                         // Lower byte = baud rate, 01=9600(default)
                                         //              02=4800, 03=2400, 04=1200

const char* bStr[5] = {"", "9600", "4800", "2400", "1200"};

RS485 rs485(Serial2, 16, 17, NOT_A_PIN, NOT_A_PIN);
Modbus mb(&rs485);

void setup() {

    Serial.begin(115200);
    Serial.println();

    rs485.begin(9600, SERIAL_8N1);

    // Reading the Device Settings
    mb.readHoldingRegister(DDS238_ID, DDS238_COMMUNICATION, 1);
    uint8_t id = mb.getHighByte();
    uint8_t baud_rate = mb.getLowByte();

    Serial.print("ID = ");
    Serial.print(id);
    Serial.print(", Baud rate = ");
    Serial.println( bStr[baud_rate] );
    Serial.println("========================");

}

void loop() {

    // Reading Power Meter
    mb.readHoldingRegister(DDS238_ID, DDS238_VOLTAGE, 1);
    int16_t voltage = mb.getInt();

    mb.readHoldingRegister(DDS238_ID, DDS238_CURRENT, 1);
    int16_t current = mb.getInt();

    mb.readHoldingRegister(DDS238_ID, DDS238_ACTIVE_POWER, 1);
    int16_t power = mb.getInt();

    mb.readHoldingRegister(DDS238_ID, DDS238_INPUT_ENERGY, 2);
    int32_t input_energy = mb.getInt32();

    mb.readHoldingRegister(DDS238_ID, DDS238_OUTPUT_ENERGY, 2);
    int32_t output_energy = mb.getInt32();

    mb.readHoldingRegister(DDS238_ID, DDS238_TOTAL_ENERGY, 2);
    int32_t total_energy = mb.getInt32();

    mb.readHoldingRegister(DDS238_ID, DDS238_POWER_FACTOR, 1);
    int16_t pf = mb.getInt();

    mb.readHoldingRegister(DDS238_ID, DDS238_FREQUENCY, 1);
    int16_t freq = mb.getInt();

    Serial.print("\nVoltage(V): ");        Serial.print(voltage/10.0);
    Serial.print("\tCurrent(A): ");        Serial.print(current/100.0);
    Serial.print("\tActive Power(W): ");          Serial.println(power);
    Serial.print("Energy In(kWh): ");      Serial.print(input_energy/100.0);
    Serial.print("\tEnergy Out(kWh): ");   Serial.print(output_energy,100.0);
    Serial.print("\tTotal Energy(kWh): "); Serial.println(total_energy/100.0);
    Serial.print("Power Factor: ");        Serial.print(pf/1000.0);
    Serial.print("\tFequency(Hz): ");      Serial.println(freq/100);

    delay(10000);

}
