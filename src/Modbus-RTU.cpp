#include <RS485.h>
#include <Modbus-RTU.h>

// Based on MODBUS Application Protocol Specification V1.1b3 page 48
// In modbus protocol, error 0, 12 and 13 are undefined, added by this library
const char* modbusExceptions[] = {
  "No Error",
  "Illegal Function",
  "Illegal Data Address",
  "Illegal Data Value",
  "Server Device Failure",
  "Acknowledge",
  "Server Device Busy",
  "Undefined",
  "Memory Parity Error",
  "Undefined",
  "Gateway Path Unavailable",
  "Gateway Target Device Failed to Response",
  "Network Timeout",  // errNo = 12
  "CRC Error"         // errNo = 13
};

Modbus::Modbus(RS485* p_rs485) : _rs485(p_rs485) {};

// crc calculation by Jaime García (https://github.com/peninquen/Modbus-Energy-Monitor-Arduino/)
uint16_t Modbus::_calculateCRC(uint8_t* array, uint8_t len) {

  uint16_t _crc, _flag;
  _crc = 0xFFFF;

  for (uint8_t i = 0; i < len; i++) {
    _crc ^= (uint16_t)array[i];
    for (uint8_t j = 8; j; j--) {
      _flag = _crc & 0x0001;
      _crc >>= 1;
      if (_flag)
        _crc ^= 0xA001;
    }
  }

  return _crc;

}

void Modbus::_sendRequest(uint8_t* packet, uint8_t size) {

    uint16_t crc = _calculateCRC(packet, size-2);
    packet[size-2] = lowByte(crc);
    packet[size-1] = highByte(crc);

    _rs485->beginTransmission();
    _rs485->write(packet, size);
    _rs485->endTransmission();

}

int8_t Modbus::_getResponse(uint8_t func, uint16_t nw) {

    int8_t _responseLength = 0;

    switch (func) {
        case READ_COIL_REGISTERS:
        case READ_DISCRETE_INPUT_REGISTERS:
            // id + func + count + ( (nw -1) >> 3) + 1 ) + crc(2)
            // the nw passed-in has been converted to bytes neeed in
            // readDiscreteInputRegister() or readCoilRegister()
            _responseLength = 5 + nw;
            break;
        case READ_HOLDING_REGISTERS:
        case READ_INPUT_REGISTERS:
            // id + func + count + nw * 2 + crc(2 bytes)
            _responseLength = 5 + (nw * 2);
            break;
        case WRITE_SINGLE_COIL_REGISTERS:
            // id + func + reg(2) + coil_value(0xFF00 or 0x0000) + crc(2)
            _responseLength = 7;
            break;
        case WRITE_SINGLE_REGISTER:
        case WRITE_MULTIPLE_REGISTERS:
            // For WRITE_SINGLE:   id + func + reg(2) + value(2) + crc(2)
            // For WRITE_MUTLIPLE: id + func + reg(2) + qty(2) + crc(2)
            _responseLength = 8;
            break;
        default:
            break;
    }

    memset(response, 0, (size_t) _responseLength);

    unsigned long respStart = millis();
    int8_t i = 0;
    _errNo = 0;
    while (true) {
        if (_rs485->available())
            response[i++] = _rs485->read();

        // error response is 6 bytes long, and func byte MSB set to 1
        if (i == 6 && response[1] >= 0x80) {
            _errNo = response[2];
            return -1;
        }

        //timeout
        if (millis() - respStart > MODBUS_RESPONSE_TIMEOUT) {
          _errNo = 12;
          return -1;
        }

        // all bytes received
        if (i == _responseLength) {
            uint8_t temp[20] = {0};
            memcpy(temp, response, i-2);
            int16_t crc = _calculateCRC(temp, i-2);
            if ( highByte(crc) != response[i-1] || lowByte(crc) != response[i-2])
               _errNo = 13;    //crc error
            break;
        }

        yield();
    }
    return _responseLength;

}

// debugging function
void Modbus::debugPrint(uint8_t * array, size_t len) {
    for (int i=0; i<len; i++) {
        if (array[i] < 0x10) Serial.print('0');
        Serial.print(array[i], HEX);
        Serial.print(' ');
    }
    Serial.println();
}

// Refer to https://babbage.cs.qc.cuny.edu/IEEE-754.old/32bit.html for HEX/float conversion
float Modbus::getFloat() {

    float fv = 0.0;

    ((uint8_t*)&fv)[3]= response[3];
    ((uint8_t*)&fv)[2]= response[4];
    ((uint8_t*)&fv)[1]= response[5];
    ((uint8_t*)&fv)[0]= response[6];

    return fv;
}

int16_t Modbus::getInt() {

    return (int16_t) response[3] << 8 | response[4];

}


int32_t Modbus::getInt32() {

    return (int32_t) response[3] << 24 | response[4] << 16 | response[5] << 8 | response[6];

}

uint8_t Modbus::getHighByte() {

    return response[3];

}

uint8_t Modbus::getLowByte() {

    return response [4];

}

const char * Modbus::errorMsg() {
    return modbusExceptions[_errNo];
}

/* Modbus Function Code 0x02 for reading discrete input registers
 * request:  <id> <0x02> <addr_h> <addr_l> <qty_h> <qty_l>
 * response: <id> <0x02> <count> <in_1> .. <in_n> <crc_h> <crc_l>
 * e.g. request  - 10 02 00 c4 00 16 (read 0x16(20) registers starting from 00c4(196+1))
 *      response - 10 02 B10101100 B11011011 B00110100
 *                               |--------------------- register 197
 *                        |---------------------------- register 204
 *                                  |------------------ register 212
 *                                         |----------- register 205
 *                                                   |- register 213
 *                                              |------ register 218
 */
int8_t Modbus::readDiscreteInputRegister(uint8_t id, uint16_t reg, uint16_t nw) {

    uint16_t _nw = (uint16_t) (((nw - 1) >> 3) + 1);

    uint8_t packet[8] = {id, READ_DISCRETE_INPUT_REGISTERS,
                        highByte(reg), lowByte(reg), highByte(_nw), lowByte(_nw),
                        0, 0
                        };

    _sendRequest(packet, 8);

    return _getResponse(READ_DISCRETE_INPUT_REGISTERS, _nw);

}

/*
 * Modbus Function Code 0x04 for reading input registers
 * request: <id> <0x04> <add_h> <add_l> <nw_h> <nw_l> <crc_h> <crc_l>
 * response: <id> <0x04> <byte_count> <resp1_h> <resp1_l> [<payload2_h> <payload2_l>] <crc_h> <crc_l>
 */
int8_t Modbus::readInputRegister(uint8_t id, uint16_t reg, uint16_t nw) {

    uint8_t packet[8] = {id, READ_INPUT_REGISTERS, highByte(reg), lowByte(reg),
                         highByte(nw), lowByte(nw), 0, 0
                        };

    _sendRequest(packet, 8);

    return _getResponse(READ_INPUT_REGISTERS, nw);

}

/*
 * Modbus Function Code 0x03 for reading holding registers
 * request: <id> <0x03> <add_h> <add_l> <nw_h> <nw_l> <crc_h> <crc_l>
 * response: <id> <0x03> <byte_count> <resp1_h> <resp1_l> [<payload2_h> <payload2_l>] <crc_h> <crc_l>
 */
int8_t Modbus::readHoldingRegister(uint8_t id, uint16_t reg, uint16_t nw) {

    uint8_t packet[8] = {id, READ_HOLDING_REGISTERS, highByte(reg), lowByte(reg),
                         highByte(nw), lowByte(nw), 0, 0
                        };

    _sendRequest(packet, 8);

    return _getResponse(READ_HOLDING_REGISTERS, nw);

}

int8_t Modbus::writeSingleRegister(uint8_t id, uint16_t reg, uint16_t data) {

    uint8_t packet[8] = {id, WRITE_SINGLE_REGISTER,
        highByte(reg), lowByte(reg), highByte(data), lowByte(data), 0, 0};

        _sendRequest(packet, 8);

        return _getResponse(WRITE_SINGLE_REGISTER, 1);
}

/* Modbus Function Code 0x10 for writing multiple registers
 * respone echo back <id> <func> <reg_h> <reg_l> <N_h> <N_l> <crc_h> <crc_l>
 */
int8_t Modbus::writeMultipleRegisters(uint8_t id, uint16_t reg, float payload) {

    uint8_t packet[13] = {id, WRITE_MULTIPLE_REGISTERS, highByte(reg), lowByte(reg),
        0x00,
        0x02,                        // 2 registers (number of registers N)
        0x04,                        // 4 bytes(2*N) for float data
        ((uint8_t*)&payload)[3],     // convert from float little Endian to big Endian
        ((uint8_t*)&payload)[2],
        ((uint8_t*)&payload)[1],
        ((uint8_t*)&payload)[0],
        0, 0};

        _sendRequest(packet, 13);

        return _getResponse(WRITE_MULTIPLE_REGISTERS, 4);

}

int8_t Modbus::writeMultipleRegisters(uint8_t id, uint16_t reg, uint16_t payload) {

    uint8_t packet[11] = {id, WRITE_MULTIPLE_REGISTERS, highByte(reg), lowByte(reg),
        0x00,
        0x01,                        // 1 register (number of registers N)
        0x02,                        // 2 bytes(2*N)
        ((uint8_t*)&payload)[1],     // convert from uint16_t little Endian to big Endian
        ((uint8_t*)&payload)[0],
        0, 0};

        _sendRequest(packet, 11);

        return _getResponse(WRITE_MULTIPLE_REGISTERS, 2);

}
