/*
Author: Devin Himmelheber
Date: 10/6/2021
Version: 1.0.1
Description: 
  Demo code to run on Sparkfun's Artemis ATP for modbus master/slave control over serial.
  System mode is determined by Global Control Variable "MASTER_SETUP". When the GCV is set to 0, 
  the system will be running in slave mode with ID set to 1 and a baud rate of 9600. When the GCV
  is set to 1, the system will be running in master mode with a baud rate of 9600 and is compatible
  with the MAX485 TTL to RS-485 Interface Module by using UART0 and pins 2 and 3.
*/

#include <main.h>


void setup()
{ 
  if (MASTER_SETUP == 0) { slaveSetup(); } // If system will be a slave
  else { masterSetup(); } // If system will be a master
}

void loop(){

  if(MASTER_SETUP == 0)
  {
    slave.run(); // Checks for a communication on the Serial line and decodes as necessary
  }
  else
  {
    //Example write
    uint16_t address = 0x0001;
    int value = 1;
    node.writeSingleRegister(address, value);
    readReply(readBuffer); // Could either read slave's message or just clear the response buffer with: node.clearResponseBuffer();

    //Example read
    uint16_t qty = 1;
    node.readHoldingRegisters(address, qty);
    readReply(readBuffer);
    
  }
  
  delay(1000);
}

void slaveSetup()
{
  // https://www.youtube.com/watch?v=wKPNcE51YUk
  registerBank.setId(DEVICE_ID); ///Set Slave ID

  for (int i = REGISTRY_START + 1; i < INTERNAL_REGISTERS; i++)
  {
    registerBank.add(i);
  }

  pinMode(GreenPin, OUTPUT);//Green light
  pinMode(YellowPin, OUTPUT);//Yellow Light
  pinMode(RedPin, OUTPUT);//Red Light

  slave._device = &registerBank; 
  slave.setBaud(BAUD_RATE); // Will want something higher for the real thing
}

void masterSetup()
{
  // https://circuitdigest.com/microcontroller-projects/rs-485-modbus-serial-communication-with-arduino-as-master
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);

  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  node.begin(BAUD_RATE); // Will want something higher for the real thing

  // https://protosupplies.com/product/max485-ttl-to-rs-485-interface-module/ 
  node.assignFunctions(&preTransmission, &postTransmission);
}

void readReply(uint16_t readBuffer[])
{
  for(int i = 0; i <= node._u8ResponseBufferLength; i++) // Unnecessary but the actual buffer is private. May change later.
    {
      readBuffer[i] = node.getResponseBuffer(i);
    }

    node.clearResponseBuffer();
}