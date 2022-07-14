## Single-phase Energy Meter DDM18SD

![DDM18SD Single-phase Energy Meter](/Users/henrycheung1/Documents/Arduino/Modbus-RTU/examples/DDM18SD/DDM18SD.png "DDM18SD Single-phase Energy Meter")



### Communication Configuration

**Baud Rate:** 1200bps, 2400bps, 4800bps and 9600bps (Default).

**Data Bits:** 8-bit

**Parity:** Even

**Stop Bit:** 1 Stop bit

**Default Address:** 1

**Supported Modbus Registers:**

    03 - Reading Holding Register (Parameter Register)

    04 - Reading Input Register (Data Register)

    10 - Write Multiple Registers

### Data Register List

| Register Address | Description           | Unit  | Format         |
| ---------------- | --------------------- | ----- | -------------- |
| 0x0000           | Voltage               | V     | Floating Point |
| 0x0008           | Current               | A     | Floating Point |
| 0x0012           | Active Power          | Kw    | Floating Point |
| 0x001A           | Reactive Power        | Var   | Floating Point |
| 0x002A           | Power Factor          | Cos   | Floating Point |
| 0x0036           | Frequency             | Hz    | Floating Point |
| 0x0100           | Total Active Energy   | Kwh   | Floating Point |
| 0x0400           | Total Reactive Energy | Kvarh | Floating Point |

### Parameter Register List

| Register Address | Description                        | Format         | Default |
| ---------------- | ---------------------------------- | -------------- | ------- |
| 0x0000           | Baud Rate (1200, 2400, 4800, 9600) | Floating Point | 9600    |
| 0x0002           | Parity (0: Even, 1: Odd, 2: None)  | Floating Point | Even    |
| 0x0008           | Device Address (1 - 247)           | Floating Point | 1       |

**Note:** The data read from the meter confirms to the IEEE-754 32-bit single-precision floating-point data format. 

### Examples

#### Read Voltage Value

Request(HEX): 01 04 00 00 00 02 71 CB

| Data  | Description                             |
| ----- | --------------------------------------- |
| 01    | Device ID                               |
| 04    | Function code for Reading Data Register |
| 00 00 | Read data from register address 0x0000  |
| 00 02 | Read 2 words(4 bytes) of data           |
| 71 CB | CRC, with LSB first, MSB as second byte |

Return(HEX): 01 04 04 43 6B 58 0E 25 D8

| Data        | Description                           |
| ----------- | ------------------------------------- |
| 01          | Devive ID                             |
| 04          | Return function code                  |
| 04          | Number of bytes returned              |
| 43 6B 58 0E | 4-byte floating point value for 235.3 |
| 25 D8       | CRC                                   |

#### Read Meter Device ID

Request(HEX): 01 03 00 08 00 02 45 C9

| Data  | Description                                  |
| ----- | -------------------------------------------- |
| 01    | Device ID                                    |
| 03    | Function code for Reading Parameter Register |
| 00 08 | Read data from register address 0x0008       |
| 00 02 | Read 2 registers data(4 bytes)               |
| 71 CB | CRC, with LSB first, MSB as second byte      |

Return(HEX): 01 03 04 3F 80 00 00 EF F3

| Data        | Description                                  |
| ----------- | -------------------------------------------- |
| 01          | Device ID                                    |
| 03          | Function code for Reading Parameter Register |
| 04          | Return 4 bytes of data                       |
| 3F 80 00 00 | 4-byte floating point for device ID of 1     |
| EF F3       | CRC                                          |

#### Change Meter Device ID

Request(HEX): 01 10 00 08 00 02 04 40 00 00 00 E7 C9

| Data        | Description                                  |
| ----------- | -------------------------------------------- |
| 01          | Device ID                                    |
| 10          | Function Code for Writing Parameter Register |
| 00 08       | Write data starting from Register 0x0008     |
| 00 02       | Write 2 registers data                       |
| 04          | 4 Bytes                                      |
| 40 00 00 00 | 4-byte floating poitn value for 2            |
| E7 C9       | CRC                                          |

Return(HEX): 01 10 00 08 00 02 C0 0A

Indicates that the device ID has been changed successfully.

#### Change Communicaiton Baud Rate

Request(HEX): 01 10 00 00 00 02 04 44 96 00 00 25 7B

| Data        | Description                                  |
| ----------- | -------------------------------------------- |
| 01          | Device ID                                    |
| 10          | Function Code for Writing Parameter Register |
| 00 00       | Write data starting from Register 0x0000     |
| 00 02       | Write 2 registers data                       |
| 04          | 4 Bytes                                      |
| 44 96 00 00 | 4-byte floating point value for 1200         |
| 25 7B       | CRC                                          |

Return(HEX): 01 10 00 00 00 02 41 C8

Indicates that the setting has been changed successfully 

### Resource

[Hex to IEEE 754 floating point converter](Use https://babbage.cs.qc.cuny.edu/IEEE-754.old/32bit.html)
