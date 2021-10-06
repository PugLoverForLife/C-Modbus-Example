#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <modbusDevice.h>
#include <modbusRegBank.h>
#include <modbusSlave.h>
#include <ModbusMaster.h>

/* *** Global Control Variable *** */
#define MASTER_SETUP  1


modbusDevice registerBank;
modbusSlave slave;
ModbusMaster node;

// Pin defines
#define MAX485_RE_NEG 2
#define MAX485_DE     3

#define GreenPin      10
#define YellowPin     11
#define RedPin        12

// Adjustable program parameters
#define MAX_BUFFER_SIZE 64
#define BAUD_RATE 9600
#define DEVICE_ID 1

// Buffers
uint16_t readBuffer[MAX_BUFFER_SIZE] = {0};

// Prototypes
void slaveSetup();
void masterSetup();
void readReply(uint16_t readBuffer[]);

enum NDX {
    REGISTRY_START,     // *** BEGIN MODBUS RECIEVE DATA ***
    COMMAND,
    PARAMETER,
    INTERNAL_REGISTERS, // *** BEGIN INTERNAL REGISTERS ***
    EXAMPLE_REGISTER,
    REGISTRY_END
};

// Callback functions
// Message to go out
void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

// Message to come in
void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

#endif