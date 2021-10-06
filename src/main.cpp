/*
Author: Devin Himmelheber
Date: 10/6/2021
Version: 1.0.0
Description: 
  Demo code to run on Sparkfun's Artemis ATP for modbus master/slave control over serial.
  System mode is determined by Global Control Variable "MASTER_SETUP". When the GCV is set to 0, 
  the system will be running in slave mode with ID set to 1 and a baud rate of 9600. When the GCV
  is set to 1, the system will be running in master mode with a baud rate of 9600 and is compatible
  with the MAX485 TTL to RS-485 Interface Module by using UART0 and pins 2 and 3.

To Do:
  - Test with Modbus slave/master program on the computer.
  - Check connection stability when run as an RTOS thread.
  - Increase baud rate to a higher value.
  - Test system with the compatible Interface Module.
  - Clean-up included libraries to increase efficiency (space & speed).
  - Work with MODBUS commands and make a sparsing algorithm.
  - Add the ability to switch between master and slave during runtime.

Strech Goals:
  - Test different communication protocols (I2C & SPI).
*/

#include <Arduino.h>
#include <modbusDevice.h>
#include <modbusRegBank.h>
#include <modbusSlave.h>
#include <ModbusMaster.h>

#include <Registry.h>

/* *** Global Control Variable *** */
#define MASTER_SETUP  0


modbusDevice regBank;
modbusSlave slave;
ModbusMaster node;

#define RedPin        12
#define YellowPin     11
#define GreenPin      10

#define MAX485_DE     3
#define MAX485_RE_NEG 2

#define MAXBUFFERSIZE 64

uint16_t readBuffer[MAXBUFFERSIZE] = {0};

using namespace Reg;

// Prototypes
void regUpdate();
void preTransmission();
void postTransmission();

void setup()
{ 
  if (MASTER_SETUP == 0) // If system will be a slave
  // https://www.youtube.com/watch?v=wKPNcE51YUk
  {
    regBank.setId(1); ///Set Slave ID

    regBank.add(COMMAND); // Command
    regBank.add(PARAMETER); // Parameter
    //regBank.add(3);//Green

    pinMode(RedPin,OUTPUT);//Red Light
    pinMode(YellowPin,OUTPUT);//Yellow Light
    pinMode(GreenPin,OUTPUT);//Green light
    
  
    slave._device = &regBank; 
    slave.setBaud(9600); // Will want something higher for the real thing
  }
  else // If system will be a master
  // https://circuitdigest.com/microcontroller-projects/rs-485-modbus-serial-communication-with-arduino-as-master
  {

    pinMode(MAX485_RE_NEG, OUTPUT);
    pinMode(MAX485_DE, OUTPUT);

    digitalWrite(MAX485_RE_NEG, 0);
    digitalWrite(MAX485_DE, 0);

    node.begin(9600); // Will want something higher for the real thing

    // https://protosupplies.com/product/max485-ttl-to-rs-485-interface-module/ 
    node.assignFunctions(&preTransmission, &postTransmission); 
    
  }
}

void loop(){

  if(MASTER_SETUP == 0)
  {
    Serial.println("Updating registry");
    regUpdate();

    slave.run();
  }
  else
  {
    //Example write
    uint16_t address = 0x0001;
    int value = 1;
    node.writeSingleRegister(address, value);
    // Could either read slave's message or clear the response buffer
    node.clearResponseBuffer();
    /*
    for(int i = 0; i <= node._u8ResponseBufferLength; i++) // Unnecessary but the actual buffer is private. May change later.
    {
      readBuffer[i] = node.getResponseBuffer(i);
      
    }
    */

    //Example read
    uint16_t qty = 1;
    node.readHoldingRegisters(address, qty);
    for(int i = 0; i <= node._u8ResponseBufferLength; i++) // Unnecessary but the actual buffer is private. May change later.
    {
      readBuffer[i] = node.getResponseBuffer(i);
      
    }
    
  }
  delay(1000);

}

void regUpdate(){
  for (int i = REGISTRY_START + 1; i < INTERNAL_REGISTERS; i++)
  {
    theRegistry[i] = regBank.get(i);
  }
  
  //Control LED Lights's 
  /*
    int command = regBank.get(1);
      if (RedLED <= 0 && digitalRead(RedPin) == HIGH)digitalWrite(RedPin,LOW);
      if (RedLED >= 1 && digitalRead(RedPin) == LOW)digitalWrite(RedPin,HIGH);
      
    int YellowLED = regBank.get(2);//Controls the 
      if (YellowLED <= 0 && digitalRead(YellowPin) == HIGH)digitalWrite(YellowPin,LOW);
      if (YellowLED >= 1 && digitalRead(YellowPin) == LOW)digitalWrite(YellowPin,HIGH);
      
    int GreenLED = regBank.get(3);//Controls the Green
      if (GreenLED <= 0 && digitalRead(GreenPin) == HIGH)digitalWrite(GreenPin,LOW);
      if (GreenLED >= 1 && digitalRead(GreenPin) == LOW)digitalWrite(GreenPin,HIGH);
      */
}

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
