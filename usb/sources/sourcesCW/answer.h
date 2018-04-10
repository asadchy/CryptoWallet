#include <stdint.h>
typedef unsigned char BYTE;
//typedef unsigned int uint32_t;

int dataToBuffer(uint8_t *dataIn, uint32_t *lenIn, uint8_t *buffer, uint32_t *lenBuf, uint32_t *send, int *pinInit, uint32_t *pinDef);
void answerCom(uint8_t *dataIn, uint32_t* lenIn, uint8_t *dataOut, uint32_t* lenOut, int *pinInit, uint32_t *pinDef);
void checkPin(int *pinInit, uint32_t *pinDef);
void sign (int id, int amount, BYTE *mess, BYTE *valueTr, BYTE *addr, BYTE *out, uint32_t *lenOut, int *pinInit, uint32_t *pinDef);
