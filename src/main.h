#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <modbusDevice.h>
#include <modbusRegBank.h>
#include <modbusSlave.h>
#include <ModbusMaster.h>

/* *** Global Control Variable *** */
#define MASTER_SETUP RUNTIME_DECISION

// GCV options
#define RUNTIME_DECISION 0
#define SLAVE_START_UP 1
#define MASTER_START_UP 2

// Object creation
modbusDevice registerBank;
modbusSlave slave;
ModbusMaster node;

// Pin defines
#define MAX485_RE_NEG 2
#define MAX485_DE     3

#define GreenPin      10
#define YellowPin     11
#define RedPin        12

#define Decision_EN   27
#define Decision_Pin  28

// Adjustable program parameters
#define MAX_BUFFER_SIZE 64
#define BAUD_RATE 9600
#define DEVICE_ID 1

// Buffers
uint16_t readBuffer[MAX_BUFFER_SIZE] = {0};

// Globals
int master_Setup_Override;

// Prototypes
void slaveSetup();
void masterSetup();
void runtime_decision();
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