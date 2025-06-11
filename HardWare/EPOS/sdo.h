#ifndef __SDO_H
#define __SDO_H

#include "main.h"
#include "epos.h"


void SDO_Write(Epos* epos1,Uint16 Index,Uint8 SubIndex,Uint32 param);
//void SDO_Read(Epos* epos,Uint16 Index,Uint8 SubIndex);
Uint32 SDO_Read(Epos* epos,Uint16 Index,Uint8 SubIndex);

#endif

