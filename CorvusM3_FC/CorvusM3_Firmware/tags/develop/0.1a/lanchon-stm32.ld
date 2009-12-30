/* Linker script for STM32 (by Lanchon)
 *
 * Configure target memory and included script
 * according to your application requirements. */

/* Define memory spaces. */
MEMORY
{
  ram (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
  rom (rx) : ORIGIN = 0x08000000, LENGTH = 512K
}

/* Include the appropriate script.
 * Options:
 *   lanchon-stm32-rom.ld
 *   lanchon-stm32-rom-hosted.ld
 *   lanchon-stm32-ram.ld
 *   lanchon-stm32-ram-hosted.ld
 */
INCLUDE "lanchon-stm32-rom.ld"
