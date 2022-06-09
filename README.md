# Modbus-RTU library for Arduino

This Modbus-RTU library focus on supporting various RTU functions. The RS485 is supported via a separate [RS485 library](https://github.com/e-tinkers/RS485.git). This provide the flexibity of supporting various MCU's hardware configuration, it also allows various Modbus Devices to be built on top the Modbus RTU lirbary.


## Why another Modbus RTU library?
Many of the Modbus RTU libraries or open source projects that we seen are either written for one specific device in mind or for one specific MCU interface, as the result it is tightly integrated and lack of separation between communication layer, the modbus protocol stack and the device driver, so you often have to do some modification in order to support the MCU that you are using, or re-write many part of the code in order to support a different Modbus devive.

This library has the dependency of RS485 library for a good reason. The Modbus protocol layer is separated from lower layer communication protocol so that you could instantiate and configure your Serial Communication via the RS485 library, while this library focusing on providing well-tested and reliable Modbus RTU funcitonalities.

Many embiousious Modbus projects are trying have one single library to support both Modbus RTU and Modbus TCP, dispite the two have very different system architectures and network interfaces. As the result, the codes are bloated and complicate to extend or adding new supports for various Modbus functions. This library focus only on Modbus RTU, and do it right effectively and efficiently.

Modbus RTU is an extreamely flexible protocol, this is a good thing, however, none of the Modbus devices share the same implementation from each others. For this reason, it is almost certain that in order to support a Modbus device, your sketch is more or less a Device Driver for that particular Modbus device, in terms of how the Input and Holdings registers are used.

The library provides the Modbus RTU as a middleware, allows different Modbus Device Drivers (essentially your sketch) to be built on top the Modbus RTU protocol stack. This library provides a few Driver sketches as example, but we hope over the time, drivers for other devices could be added through Pull Request to enrich this library.

       |------------------------------------------|
       |  device  |  devive  | . . . . |  device  |
       | driver 1 | driver 2 | . . . . | driver n |
       |------------------------------------------|
       | Modbus-RTU Class supporting RTU funtions |
       |------------------------------------------|
       |   RS485 Library based on Stream Class    |
       |------------------------------------------|

## Supported Modbus RTU functions
The following Modbus functions are available so far (Pull Request for adding new functionality is welcome):

Discrete Coils/Flags

0x02 - Read Discrete Inputs

Registers

0x03 - Read Holding Registers
0x04 - Read Input Registers
0x06 - Write Single Register
0x10 - Write Multiple Registers
