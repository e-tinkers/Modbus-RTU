## SHT20-based Modbus Temperature/Humidity Sensor

![SHT20 Modbus Temperature Humidity Sensor](https://github.com/e-tinkers/Modbus-RTU/blob/master/examples/SHT20/SHT20_modbus.png "SHT20 Modbus Temperature Humidity Sensor")

> Most of the online stores on Aliexpress or alike refer to this [documenation](https://github.com/e-tinkers/Modbus-RTU/blob/master/examples/SHT20/Temperature-and-humidity-transmitter-SHT20-sensor-Modbus-RS485.pdf), the document however contains wrong information such as wrong CRC calculation and other errors. The following information are verified by me based on the sensor that I have.

### Communication Configuration

**Baud Rate:** 9600bps (Default), 14400bps, 19200bps

**Data Bits:** 8-bit

**Parity:** None

**Stop Bit:** 1 Stop bit

**Default Address:** 1

**Supported Modbus Registers:**

  03 - Reading Holding Register (Parameter Register)

  04 - Reading Input Register (Data Register)

  06 - Write Single Register

  10 - Write Multiple Registers

### Data Register List

| Register Address | Description        | Unit | Format  |
| ---------------- | ------------------ | ---- | ------- |
| 0x0001           | Temperature        | C    | Integer |
| 0x0002           | Humidity           | RH   | Integer |
| 0x0101           | Device Address     |      | Integer |
| 0x0102           | Baud Rate          |      | Integer |
| 0x0103           | Temperature offset |      | Integer |
| 0x0104           | Humidity offset    |      | Integer |

### Examples

#### Read Temperature Value

Request(HEX): 01 04 00 01 00 01 60 0A

| Data  | Description                             |
| ----- | --------------------------------------- |
| 01    | Device ID                               |
| 04    | Function code for Reading Data Register |
| 00 01 | Read data from register address 0x0001  |
| 00 01 | Read 1 words(2 bytes) of data           |
| 60 0A | CRC, with LSB first, MSB as second byte |

> There are two types of sensors where some come with the raw temperature and humidity reading as 10x of the actual value, some has a 100x factor.

Return(HEX): 01 04 02 01 31 79 74 (for those sensors with x10 factor)

Return(HEX): 01 04 02 0B EA 3F 8F (for those sensors with x100 factor)

| Data  | Description              |
| ----- | ------------------------ |
| 01    | Devive ID                |
| 04    | Return function code     |
| 02    | Number of bytes returned |
| 01 31 | Temperature Reading(x10) |
| 79 74 | CRC                      |

or

| Data  | Description              |
| ----- | ------------------------ |
| 01    | Devive ID                |
| 04    | Return function code     |
| 02    | Number of bytes returned |
| 0B EA | Temperature Reading(x100)|
| 79 74 | CRC                      |

Temperature value 0x131 = 305, 305/10 = 30.5 degree Celsius

Temperature value 0xBEA = 3050, 3050/100 = 30.5 degree Celsius

> The temperature value is in two-compliment format for negative number, 0xFF33 = -205 or -20.5 degree Celsius

#### Read Humidity Value

Request(HEX): 01 04 00 02 00 01 90 0A

| Data  | Description                             |
| ----- | --------------------------------------- |
| 01    | Device ID                               |
| 04    | Function code for Reading Data Register |
| 00 02 | Read data from register address 0x0002  |
| 00 01 | Read 1 words(2 bytes) of data           |
| 90 0A | CRC, with LSB first, MSB as second byte |

Return(HEX): 01 04 02 02 22 38 49 (for those sensors with x10 factor)

Return(HEX): 01 04 02 15 54 B6 5F (for those sensors with x100 factor)

| Data  | Description              |
| ----- | ------------------------ |
| 01    | Devive ID                |
| 04    | Return function code     |
| 02    | Number of bytes returned |
| 02 22 | Humidity Reading (10x)   |
| 38 49 | CRC                      |

or

| Data  | Description              |
| ----- | ------------------------ |
| 01    | Devive ID                |
| 04    | Return function code     |
| 02    | Number of bytes returned |
| 15 54 | Humidity Reading (100x)  |
| B6 5F | CRC                      |

Humidity value 0x222 = 546, 546/10 = 54.6 RH %

Humidity value 0x1554 = 5460, 5460/100 = 54.6 RH %

#### Read both Temperature and Humidity values

Temperature and Humidity values can be read with one single command

Request(HEX): 01 04 00 01 00 02 20 0B

Return(HEX):  01 04 04 01 31 02 22 2A CE

#### Read Meter Device ID

Request(HEX): 01 03 01 01 00 01 D4 36

| Data  | Description                             |
| ----- | --------------------------------------- |
| 01    | Device ID                               |
| 03    | Reading Holding Register                |
| 01 01 | Read data from register address 0x0101  |
| 00 01 | Read 1 word(2 bytes)                    |
| D4 36 | CRC, with LSB first, MSB as second byte |

Return(HEX): 01 03 02 00 01 79 84

| Data  | Description              |
| ----- | ------------------------ |
| 01    | Device ID                |
| 03    | Reading Holding Register |
| 02    | Return 2 bytes of data   |
| 00 01 | Device ID of 0x0001      |
| 79 84 | CRC                      |

#### Change Meter Device ID

Request(HEX): 01 06 01 01 00 08 D8 30

| Data  | Description                              |
| ----- | ---------------------------------------- |
| 01    | Device ID                                |
| 06    | Writing to Single Register               |
| 01 01 | Write data starting from Register 0x0101 |
| 00 08 | Modify device ID to 0x0008               |
| D8 30 | CRC                                      |

Return(HEX): 01 06 01 01 00 08 D8 30

Indicates that the device ID has been changed successfully.

#### Change Device ID and Communicaiton Baud Rate

Request(HEX): 01 10 01 01 00 02 04 00 20 25 80 25 09

| Data        | Description                               |
| ----------- | ----------------------------------------- |
| 01          | Device ID                                 |
| 10          | Function Code for Writing Single Register |
| 01 01       | Write data starting from Register 0x0101  |
| 00 02       | Write 2 registers data                    |
| 04          | 4 Bytes                                   |
| 00 20 25 80 | 0x0020 for deviceID, 0x2580 for baud rate |
| 25 09       | CRC                                       |

Return(HEX): 01 10 01 01 00 02 11 F4

Indicates that the setting has been changed successfully 
