#ifndef REGISTRY_H
#define REGISTRY_H

namespace Reg {
  extern int theRegistry[];
}

enum NDX {
    REGISTRY_START,     // *** BEGIN MODBUS RECIEVE DATA ***
    COMMAND,
    INTERNAL_REGISTERS,                    // *** BEGIN INTERNAL REGISTERS ***
    EXAMPLE_REGISTER,
    REGISTRY_END
};

#endif
