## Single-phase Energy Meter DDS238-1

![DDS238-1 Single-phase Energy Meter](https://github.com/e-tinkers/Modbus-RTU/blob/master/examples/DDS238/DDS238-1.png "DDS238 Single-phase Energy Meter")

### Communication Configuration

**Baud Rate:** 1200bps, 2400bps, 4800bps and 9600bps (Default).

**Data Bits:** 8-bit

**Parity:** None

**Stop Bit:** 1 Stop bit

**Default Address:** 1

**Supported Modbus Registers:**

    03 - Reading Holding Register (Parameter Register)

    04 - Reading Input Register (Data Register)

    10 - Write Multiple Registers

### Data Register List

| Register Address | Description           | Unit     | Format         |
| ---------------- | --------------------- | -------- | -------------- |
| 0x0000           | Total Energy          | kWh x 100 | INT32(DWORD)  |
| 0x0008           | Output Energy         | kWh x 100 | INT32(DWORD)  |
| 0x000A           | Input Energy          | Kwh x 100 | INT32(DWORD)  |
| 0x000C           | Voltage               | Volt x 10 | INT16(WORD)   |
| 0x000D           | Current               | Amp x 100 | INT16(WORD)   |
| 0x000E           | Active Power          | Watt      | INT16(WORD)   |
| 0x000F           | Reactive Power        | Var       | INT16(WORD)   |
| 0x0010           | Power Factor          | x1000     | INT16(WORD)   |
| 0x0011           | Frequency             | Hz x 100  | INT16(WORD)   |
| 0x0015           | Parameter Settings    |           | INT16(WORD)   |

### Examples

#### Read Parameter Settings

Request(HEX): 01 03 00 15 00 01 95 CE

| Data  | Description                                  |
| ----- | -------------------------------------------- |
| 01    | Device ID                                    |
| 03    | Function code for Reading Parameter Register |
| 00 15 | Read data from register address 0x0015       |
| 00 01 | Read 1 register data(1 word i.e. 2 bytes)    |
| 95 CE | CRC, with LSB first, MSB as second byte      |

Return(HEX): 01 03 02 01 01 78 14

| Data  | Description                                  |
| ----- | -------------------------------------------- |
| 01    | Device ID                                    |
| 03    | Function code for Reading Parameter Register |
| 02    | Return 2 bytes of data                       |
| 01 01 | high byte = device id, low byte = baud rate  |
| 78 14 | CRC                                          |

Device ID = high byte of 0x0101, i.e. 0x01
Baud Rate = low byte of 0x0101, i.e. 0x01 (default)
Baud Rate: 0x01 = 9600bps, 0x02 = 4800bps, 0x03 = 2400, 0x04 = 1200

#### Change Meter Device ID and Baud Rate

Request(HEX): 01 10 00 15 00 01 02 20 01 7C 95

| Data     | Description                                  |
| -------- | -------------------------------------------- |
| 01       | Device ID                                    |
| 10       | Function Code for Writing Parameter Register |
| 00 15    | Write data starting from Register 0x0015     |
| 00 01    | 1 word (2 bytes) data to be written          |
| 20 01    | Change ID(0x20), baud rate(0x01)             |
| 7C 95    | CRC                                          |

ID to be changed to 0x20 and baud rate to be changed to 0x01(i.e. 9600)

#### Read Voltage Value

Request(HEX): 01 03 00 0C 00 01 44 09

| Data  | Description                              |
| ----- | ---------------------------------------- |
| 01    | Device ID                                |
| 03    | Function code for Reading input Register |
| 00 0C | Read data from register address 0x000C   |
| 00 01 | Read 1 words(2 bytes) of data            |
| 44 09 | CRC, with LSB first, MSB as second byte  |

Return(HEX): 01 03 02 09 53 FE 3D

| Data  | Description                |
| ----- | -------------------------- |
| 01    | Devive ID                  |
| 03    | Return function code       |
| 02    | Number of bytes returned   |
| 09 63 | 2-byte value 0x0963 = 2403 |
| FE 3D | CRC                        |

Voltage = 0x0963 = 2403, 2403/10 = 240.4 Volt.

#### Read Curent Value

Request(HEX): 01 03 00 0D 00 01 15 C9

Return(HEX): 01 03 02 00 09 78 42

Current = 0x0009 = 9, 9/100 = 0.09 Amperes.

#### Read Active Power

Request(HEX): 01 03 00 0E 00 01 E5 C9

Return(HEX): 01 03 02 00 0E 39 80

Acitve Power 0x000E = 14 Watts

#### Read Total Energy

Request(HEX): 01 03 00 00 00 02 C4 0B

| Data  | Description                              |
| ----- | ---------------------------------------- |
| 01    | Device ID                                |
| 03    | Function code for Reading input Register |
| 00 00 | Read data from register address 0x0000   |
| 00 02 | Read 2 words(4 bytes) of data            |
| C4 0B | CRC, with LSB first, MSB as second byte  |

Return(HEX): 01 03 04 00 00 06 B2 79 E6

| Data        | Description                |
| ----------- | -------------------------- |
| 01          | Devive ID                  |
| 03          | Return function code       |
| 04          | Number of bytes returned   |
| 00 00 06 B2 | 2-byte value 0x0963 = 2403 |
| 79 E6       | CRC                        |

Total Energy = 0x000006B2 = 1714, 1714/100 = 17.14kWh

#### Read Power Factor
Request(HEX): 01 03 00 10 00 01 85 CF

| Data  | Description                              |
| ----- | ---------------------------------------- |
| 01    | Device ID                                |
| 03    | Function code for Reading input Register |
| 00 10 | Read data from register address 0x0010   |
| 00 01 | Read 1 words(2 bytes) of data            |
| 85 CF | CRC, with LSB first, MSB as second byte  |

Return(HEX): 01 03 02 03 E8 B8 FA

| Data  | Description                |
| ----- | -------------------------- |
| 01    | Devive ID                  |
| 03    | Return function code       |
| 02    | Number of bytes returned   |
| 03 E8 | 2-byte value 0x03E8 = 1000 |
| B8 FA | CRC                        |

Power Factor 0x03E8 = 1000, 1000/1000 = 1

#### Read Frequency

Request(HEX): 01 03 00 11 00 01 D4 0F

| Data  | Description                              |
| ----- | ---------------------------------------- |
| 01    | Device ID                                |
| 03    | Function code for Reading input Register |
| 00 11 | Read data from register address 0x0011   |
| 00 01 | Read 1 words(2 bytes) of data            |
| D4 0F | CRC, with LSB first, MSB as second byte  |

Return(HEX): 01 03 02 13 88 B5 12

| Data  | Description                |
| ----- | -------------------------- |
| 01    | Devive ID                  |
| 03    | Return function code       |
| 02    | Number of bytes returned   |
| 13 88 | 2-byte value 0x1388 = 5000 |
| B5 12 | CRC                        |

Frequency = 0x1388 = 5000, 5000/100 = 50Hz