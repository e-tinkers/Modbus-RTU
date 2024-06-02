#ifndef MODBUS_RTU__H
#define MODBUS_RTU__H
#include <RS485.h>

#define MODBUS_RESPONSE_TIMEOUT 1000

#define READ_COIL_REGISTERS           0x01
#define READ_DISCRETE_INPUT_REGISTERS 0x02
#define READ_HOLDING_REGISTERS        0x03
#define READ_INPUT_REGISTERS          0x04
#define WRITE_SINGLE_COIL_REGISTERS   0x05
#define WRITE_SINGLE_REGISTER         0x06
#define WRITE_MULTIPLE_REGISTERS      0x10

class Modbus
{
private:
    void _sendRequest(uint8_t* packet, uint8_t size);
    uint16_t _calculateCRC(uint8_t* array, uint8_t len);
    int8_t _getResponse(uint8_t func, uint16_t nw);
    int8_t _errNo;

    RS485* _rs485;

public:
    Modbus(RS485* p_rs485);
    void debugPrint(uint8_t * array, size_t len);
    void printModbusExcpetion(void);
    float getFloat();
    int16_t getInt();
    int32_t getInt32();
    uint8_t getHighByte();
    uint8_t getLowByte();
    const char* errorMsg();
    int8_t readCoilRegister(uint8_t id, uint16_t reg, uint16_t nw);
    int8_t readDiscreteInputRegister(uint8_t id, uint16_t reg, uint16_t nw);
    int8_t readHoldingRegister(uint8_t id, uint16_t reg, uint16_t nw);
    int8_t readInputRegister(uint8_t id, uint16_t reg, uint16_t nw);
    int8_t writeSingleRegister(uint8_t id, uint16_t reg, uint16_t data);
    int8_t writeMultipleRegisters(uint8_t id, uint16_t reg, float payload);
    int8_t writeMultipleRegisters(uint8_t id, uint16_t reg, uint16_t payload);

    uint8_t response[255];

};

#endif
