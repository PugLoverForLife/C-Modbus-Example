#include <Arduino.h>
#include <modbusDevice.h>
#include <modbusRegBank.h>
#include <modbusSlave.h>
#include <ModbusMaster.h>

#include <Registry.h>

modbusDevice regBank;
modbusSlave slave;
ModbusMaster node;

#define RedPin        12
#define YellowPin     11
#define GreenPin      10

#define MAX485_DE     3
#define MAX485_RE_NEG 2

#define MASTER_SETUP  0
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

    regBank.add(1); // Command
    //regBank.add(2);//Yellow
    //regBank.add(3);//Green

    pinMode(RedPin,OUTPUT);//Red Light
    pinMode(YellowPin,OUTPUT);//Yellow Light
    pinMode(GreenPin,OUTPUT);//Green light
    
  
    slave._device = &regBank; 
    slave.setBaud(9600);
  }
  else // If system will be a master
  // https://circuitdigest.com/microcontroller-projects/rs-485-modbus-serial-communication-with-arduino-as-master
  {

    pinMode(MAX485_RE_NEG, OUTPUT);
    pinMode(MAX485_DE, OUTPUT);

    digitalWrite(MAX485_RE_NEG, 0);
    digitalWrite(MAX485_DE, 0);

    //Serial.begin(115200); // Serial should already be going
    node.begin(9600);

    //node.preTransmission(); Need to add something because library does not ahave this for some reason. Line 730 modbusmaster.cpp
    //node.postTransmission();
    
    //Example write
    uint16_t address = 0x0001;
    int value = 1;
    node.writeSingleRegister(address, value);

    //Example read
    
    uint16_t qty = 1;
    node.readHoldingRegisters(address, qty);
    for(int i = 0; i <= node._u8ResponseBufferLength; i++) // Unnecessary but the actual buffer is private. May change later.
    {
      readBuffer[i] = node.getResponseBuffer(i);
    }
    
  }
}

void loop(){

Serial.println("Updating registry");
regUpdate();

delay(1000);

slave.run(); 
}

void regUpdate(){
  for (int i = REGISTRY_START; i < INTERNAL_REGISTERS; i++)
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

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}
void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}
