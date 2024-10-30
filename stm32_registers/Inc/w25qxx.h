#ifndef __W25QXX_H__
#define __W25QXX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "sys_clock.h"
#include "gpio.h"
#include "spi.h"

void W25Q_Reset (void);
void W25Q_ReadID (char *rData);


#ifdef __cplusplus
}
#endif

#endif