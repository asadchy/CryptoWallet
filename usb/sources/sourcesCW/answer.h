#ifndef SOURCES_SOURCESCW_ANSWER_H_
#define SOURCES_SOURCESCW_ANSWER_H_

#include <stdint.h>
#include "data.hpp"

typedef unsigned char BYTE;
//typedef unsigned int uint32_t;

int dataToBuffer(uint8_t *dataIn, uint32_t *lenIn, uint8_t *buffer, uint32_t *lenBuf, uint32_t *send, int *pinInit, uint32_t *pinDef);
void answerCom(uint8_t *dataIn, uint32_t* lenIn, uint8_t *dataOut, uint32_t* lenOut, int *pinInit, uint32_t *pinDef, uint32_t *send);

void sign (int id, int amount, BYTE *mess, BYTE *valueTr, BYTE *addr, BYTE *out, uint32_t *lenOut, int *pinInit, uint32_t *pinDef);

int dataInPN(uint8_t *dataIN, uint8_t *buffer, uint32_t *lenBuf, uint32_t *send, int *pinInit, uint32_t *pinDef);
#endif
