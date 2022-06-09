#include <Arduino.h>
#include <RS485.h>
#include <Modbus-RTU.h>

// ================ DDM18SD ===============
// Meter ID
#define DDM_ID                3
// Meter Input Registers
#define DDM_VOLTAGE           0x0000
#define DDM_CURRENT           0x0008
#define DDM_ACTIVE_POWER      0x0012
#define DDM_REACTIVE_POWER    0x001A
#define DDM_POWER_FACTOR      0x002A
#define DDM_FREQUENCY         0x0036
#define DDM_ACTIVE_ENERGY     0x0100
#define DDM_REACTIVE_ENERGY   0x0400

// Meter Holding Registers
#define DDM_BAUD_RATE    0x0000
#define DDM_PARITY       0x0002
#define DDM_DEVICE_ID    0x0008
#define DDM_RELAY        0x0010

// Meter Parameters
#define DDM_BAUD_1200        1200.0
#define DDM_BAUD_2400        2400.0
#define DDM_BAUD_4800        4800.0
#define DDM_BAUD_9600        9600.0
#define DDM_PARITY_EVEN      0.0
#define DDM_PARITY_ODD       1.0
#define DDM_PARITY_NONE      2.0

const char* pStr[5] = {"Even", "Odd", "None"};

RS485 rs485(Serial2, 16, 17, NOT_A_PIN, NOT_A_PIN);
Modbus mb(&rs485);

void setup() {

    Serial.begin(115200);
    Serial.println();

    rs485.begin(9600, SERIAL_8E1);

    mb.readHoldingRegister(DDM_ID, DDM_DEVICE_ID, 2);
    float id = mb.getFloat();
    mb.readHoldingRegister(DDM_ID, DDM_BAUD_RATE, 2);
    float br = mb.getFloat();
    mb.readHoldingRegister(DDM_ID, DDM_PARITY, 2);
    float parity = mb.getFloat();

    Serial.print("ID: ");             Serial.print(id, 0);
    Serial.print(", Baud Rate: ");    Serial.print(br, 0);
    Serial.println(", Parity: ");     Serial.println(pStr[(uint8_t)parity]);
    Serial.println("====================================");
}

void loop() {

    // Reading Power Meter
    mb.readInputRegister(DDM_ID, DDM_VOLTAGE, 2);
    float volt = mb.getFloat();

    mb.readInputRegister(DDM_ID, DDM_CURRENT, 2);
    float amp = mb.getFloat();

    mb.readInputRegister(DDM_ID, DDM_ACTIVE_POWER, 2);
    float pwr = mb.getFloat();

    mb.readInputRegister(DDM_ID, DDM_REACTIVE_POWER, 2);
    float rpwr = mb.getFloat();

    mb.readInputRegister(DDM_ID, DDM_ACTIVE_ENERGY, 2);
    float energy = mb.getFloat();

    mb.readInputRegister(DDM_ID, DDM_REACTIVE_ENERGY, 2);
    float renergy = mb.getFloat();

    mb.readInputRegister(DDM_ID, DDM_POWER_FACTOR, 2);
    float pf = mb.getFloat();

    mb.readInputRegister(DDM_ID, DDM_FREQUENCY, 2);
    float freq = mb.getFloat();

    Serial.print("\nVoltage(V): ");               Serial.print(volt);
    Serial.print("\tCurrent(A): ");               Serial.print(amp);
    Serial.print("\tActive Power(W): ");          Serial.print(pwr);
    Serial.print("\tReactive Power(Var): ");      Serial.println(rpwr);
    Serial.print("Active Energy (kWh): ");        Serial.print(energy);
    Serial.print("\tReactive Energy (kVarh): ");  Serial.println(renergy);
    Serial.print("Power Factor: ");               Serial.print(pf);
    Serial.print("\tFequency(Hz): ");             Serial.println(freq);

    delay(10000);

}
