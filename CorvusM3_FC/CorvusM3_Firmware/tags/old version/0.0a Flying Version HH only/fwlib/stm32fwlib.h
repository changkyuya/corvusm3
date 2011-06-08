#ifndef __STM32FWLIB_H
#define __STM32FWLIB_H

#if __cplusplus
extern "C"
{
#endif

#include "stm32f10x_lib.h"

#define BB_BIT(REGISTER, BIT_NUMBER, BASE, BB_BASE) \
	((volatile s32*) ((BB_BASE) + (((u32) &(REGISTER)) - (BASE)) * 32 + (BIT_NUMBER) * 4))

#define PERIPHERAL_BIT(REGISTER, BIT_NUMBER) \
	BB_BIT(REGISTER, BIT_NUMBER, PERIPH_BASE, PERIPH_BB_BASE)

#define SRAM_BIT(REGISTER, BIT_NUMBER) \
	BB_BIT(REGISTER, BIT_NUMBER, SRAM_BASE, SRAM_BB_BASE)

#if __cplusplus
}
#endif

#endif /* __STM32FWLIB_H */
