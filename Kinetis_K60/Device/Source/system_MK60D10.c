/*
** ###################################################################
**     Processors:          MK60DN512VLL10
**                          MK60DX256VLL10
**                          MK60DN256VLL10
**                          MK60DN512VLQ10
**                          MK60DN256VLQ10
**                          MK60DX256VLQ10
**                          MK60DN512VMC10
**                          MK60DN256VMC10
**                          MK60DX256VMC10
**                          MK60DN512VMD10
**                          MK60DX256VMD10
**                          MK60DN256VMD10
**
**     Compilers:           ARM Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**
**     Reference manual:    K60P144M100SF2V2RM Rev. 2, Jun 2012
**     Version:             rev. 1.3, 2012-10-29
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright: 2012 Freescale, Inc. All Rights Reserved.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2012-01-03)
**         Initial version
**     - rev. 1.1 (2012-04-13)
**         Added new #define symbol MCU_MEM_MAP_VERSION_MINOR.
**         Added new #define symbols <peripheralType>_BASE_PTRS.
**     - rev. 1.2 (2012-07-09)
**         UART0 - Fixed register definition - CEA709.1-B (LON) registers added.
**     - rev. 1.3 (2012-10-29)
**         Registers updated according to the new reference manual revision - Rev. 2, Jun 2012
**
** ###################################################################
*/

/**
 * @file MK60D10
 * @version 1.3
 * @date 2012-10-29
 * @brief Device specific configuration file for MK60D10 (implementation file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#include <stdint.h>
#include "MK60D10.h"

#define DISABLE_WDOG    1

#define CLOCK_SETUP     2
/* Predefined clock setups (all use 50MHz external crystal, PEE mode)
   0 ... PLL Engaged External (PEE) mode - External crystal 50MHz
         Core clock = 52MHz, BusClock = 26MHz
   1 ... PLL Engaged External (PEE) mode - External crystal 50MHz
         Core clock = 75MHz, BusClock = 37.5MHz
   2 ... PLL Engaged External (PEE) mode - External crystal 50MHz (default)
         Core clock = 90MHz, BusClock = 60MHz
*/

/*----------------------------------------------------------------------------
  Define clock source values
 *----------------------------------------------------------------------------*/
#if (CLOCK_SETUP == 0)
    #define CPU_XTAL_CLK_HZ                 50000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_XTAL32k_CLK_HZ              32768u   /* Value of the external 32k crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u   /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             4000000u /* Value of the fast internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            52000000u /* Default System clock value */
#elif (CLOCK_SETUP == 1)
    #define CPU_XTAL_CLK_HZ                 50000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_XTAL32k_CLK_HZ              32768u   /* Value of the external 32k crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u   /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             4000000u /* Value of the fast internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            75000000u /* Default System clock value */
#elif (CLOCK_SETUP == 2)
    #define CPU_XTAL_CLK_HZ                 50000000u /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_XTAL32k_CLK_HZ              32768u   /* Value of the external 32k crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             32768u   /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             4000000u /* Value of the fast internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            90000000u /* Default System clock value */
#endif /* (CLOCK_SETUP == 2) */


/* ----------------------------------------------------------------------------
   -- Core clock
   ---------------------------------------------------------------------------- */

uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

/* ----------------------------------------------------------------------------
   -- SystemInit()
   ---------------------------------------------------------------------------- */

void SystemInit (void) {
#if (DISABLE_WDOG)
  /* Disable the WDOG module */
  /* WDOG_UNLOCK: WDOGUNLOCK=0xC520 */
  WDOG->UNLOCK = (uint16_t)0xC520u;     /* Key 1 */
  /* WDOG_UNLOCK : WDOGUNLOCK=0xD928 */
  WDOG->UNLOCK  = (uint16_t)0xD928u;    /* Key 2 */
  /* WDOG_STCTRLH: ??=0,DISTESTWDOG=0,BYTESEL=0,TESTSEL=0,TESTWDOG=0,??=0,STNDBYEN=1,WAITEN=1,STOPEN=1,DBGEN=0,ALLOWUPDATE=1,WINEN=0,IRQRSTEN=0,CLKSRC=1,WDOGEN=0 */
  WDOG->STCTRLH = (uint16_t)0x01D2u;
#endif /* (DISABLE_WDOG) */

  /*
   * PEE mode setup - all three setups use 50MHz external crystal
   * Clock path: FEI (default) -> FBE -> PBE -> PEE
   *
   * Clock setup 0: MCGOUT=100MHz, Core=50MHz, Bus=25MHz (PRDIV=24, VDIV=26)
   * Clock setup 1: MCGOUT=150MHz, Core=75MHz, Bus=37.5MHz (PRDIV=14, VDIV=21)
   * Clock setup 2: MCGOUT=180MHz, Core=90MHz, Bus=60MHz (PRDIV=14, VDIV=30) [default]
   */

  /* Set all dividers to maximum temporarily to avoid overclocking during transition */
  SIM->CLKDIV1 = (uint32_t)0xFFFFFFFFu;

  /* Configure external oscillator - 50MHz high-frequency range */
  MCG->C2 &= ~MCG_C2_LP_MASK;
  MCG->C2 = (uint8_t)0x10u;            /* RANGE0=2 (high freq), EREFS=0 (external clock input) */

  /* Enter FBE mode: external clock -> MCGOUT */
  MCG->C1 = (uint8_t)0xBAu;            /* CLKS=10 (external), FRDIV=23 (/1024), IREFS=0 (external ref) */
  MCG->C4 &= (uint8_t)~(uint8_t)0xE0u; /* Clear FLL bits */
  MCG->C6 &= (uint8_t)~MCG_C6_PLLS_MASK; /* Disable PLL, select FLL */
  while((MCG->S & MCG_S_IREFST_MASK) != 0u) { /* Wait for FLL to switch to external reference */
  }
  while((MCG->S & MCG_S_CLKST_MASK) != 0x08u) { /* Wait for MCGOUT = external clock */
  }

#if (CLOCK_SETUP == 0)
  /* Clock setup 0: 100MHz MCGOUT, 50MHz Core, 25MHz Bus */
  /* PRDIV=24 (divisor=25), VDIV=26 (multiplier=50), MCGOUT = 50*50/25 = 100MHz */
  MCG->C5 = (uint8_t)0x18u;            /* PRDIV0 = 24 */
  MCG->C6 = (uint8_t)0x1Au;           /* VDIV0 = 26, PLLS = 1 */
  /* OUTDIV1=1 (core=MCGOUT/2), OUTDIV2=3 (bus=MCGOUT/4), OUTDIV3=3, OUTDIV4=5 */
  SIM->CLKDIV1 = (uint32_t)0x01130000u;
#elif (CLOCK_SETUP == 1)
  /* Clock setup 1: 150MHz MCGOUT, 75MHz Core, 37.5MHz Bus */
  /* PRDIV=14 (divisor=15), VDIV=21 (multiplier=45), MCGOUT = 50*45/15 = 150MHz */
  MCG->C5 = (uint8_t)0x0Eu;            /* PRDIV0 = 14 */
  MCG->C6 = (uint8_t)0x55u;           /* VDIV0 = 21, PLLS = 1 */
  /* OUTDIV1=1 (core=75MHz), OUTDIV2=3 (bus=37.5MHz), OUTDIV3=4, OUTDIV4=4 */
  SIM->CLKDIV1 = (uint32_t)0x01240000u;
#elif (CLOCK_SETUP == 2)
  /* Clock setup 2: 180MHz MCGOUT, 90MHz Core, 60MHz Bus [default] */
  /* PRDIV=14 (divisor=15), VDIV=30 (multiplier=54), MCGOUT = 50*54/15 = 180MHz */
  MCG->C5 = (uint8_t)0x0Eu;            /* PRDIV0 = 14 */
  MCG->C6 = (uint8_t)0x5Eu;           /* VDIV0 = 30, PLLS = 1 */
  /* OUTDIV1=1 (core=90MHz), OUTDIV2=2 (bus=60MHz), OUTDIV3=2, OUTDIV4=5 */
  SIM->CLKDIV1 = (uint32_t)0x01250000u;
#endif

  /* Enter PBE mode: external clock -> PLL -> MCGOUT */
  while((MCG->S & MCG_S_PLLST_MASK) == 0u) { /* Wait for PLLST = 1 */
  }
  while((MCG->S & MCG_S_LOCK0_MASK) == 0u) { /* Wait for LOCK0 = 1 */
  }

  /* Enter PEE mode: MCGOUT = PLL output */
  MCG->C1 = (uint8_t)0x1Au;           /* CLKS=11 (PLL) */
  while((MCG->S & 0x0Cu) != 0x0Cu) {   /* Wait for CLKST = 11b (PEE mode) */
  }
  while((MCG->S & MCG_S_LOCK0_MASK) == 0u) { /* Wait for LOCK0 = 1 */
  }
}

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

void SystemCoreClockUpdate (void) {
  uint32_t MCGOUTClock;                                                        /* Variable to store output clock frequency of the MCG module */
  uint8_t Divider;

  if ((MCG->C1 & MCG_C1_CLKS_MASK) == 0x0u) {
    /* Output of FLL or PLL is selected */
    if ((MCG->C6 & MCG_C6_PLLS_MASK) == 0x0u) {
      /* FLL is selected */
      if ((MCG->C1 & MCG_C1_IREFS_MASK) == 0x0u) {
        /* External reference clock is selected */
        if ((MCG->C7 & MCG_C7_OSCSEL_MASK) == 0x0u) {
          MCGOUTClock = CPU_XTAL_CLK_HZ;                                       /* System oscillator drives MCG clock */
        } else { /* (!((MCG->C7 & MCG_C7_OSCSEL_MASK) == 0x0u)) */
          MCGOUTClock = CPU_XTAL32k_CLK_HZ;                                    /* RTC 32 kHz oscillator drives MCG clock */
        } /* (!((MCG->C7 & MCG_C7_OSCSEL_MASK) == 0x0u)) */
        Divider = (uint8_t)(1u << ((MCG->C1 & MCG_C1_FRDIV_MASK) >> MCG_C1_FRDIV_SHIFT));
        MCGOUTClock = (MCGOUTClock / Divider);  /* Calculate the divided FLL reference clock */
        if ((MCG->C2 & MCG_C2_RANGE0_MASK) != 0x0u) {
          MCGOUTClock /= 32u;                                                  /* If high range is enabled, additional 32 divider is active */
        } /* ((MCG->C2 & MCG_C2_RANGE0_MASK) != 0x0u) */
      } else { /* (!((MCG->C1 & MCG_C1_IREFS_MASK) == 0x0u)) */
        MCGOUTClock = CPU_INT_SLOW_CLK_HZ;                                     /* The slow internal reference clock is selected */
      } /* (!((MCG->C1 & MCG_C1_IREFS_MASK) == 0x0u)) */
      /* Select correct multiplier to calculate the MCG output clock  */
      switch (MCG->C4 & (MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS_MASK)) {
        case 0x0u:
          MCGOUTClock *= 640u;
          break;
        case 0x20u:
          MCGOUTClock *= 1280u;
          break;
        case 0x40u:
          MCGOUTClock *= 1920u;
          break;
        case 0x60u:
          MCGOUTClock *= 2560u;
          break;
        case 0x80u:
          MCGOUTClock *= 732u;
          break;
        case 0xA0u:
          MCGOUTClock *= 1464u;
          break;
        case 0xC0u:
          MCGOUTClock *= 2197u;
          break;
        case 0xE0u:
          MCGOUTClock *= 2929u;
          break;
        default:
          break;
      }
    } else { /* (!((MCG->C6 & MCG_C6_PLLS_MASK) == 0x0u)) */
      /* PLL is selected */
      Divider = (1u + (MCG->C5 & MCG_C5_PRDIV0_MASK));
      MCGOUTClock = (uint32_t)(CPU_XTAL_CLK_HZ / Divider);                     /* Calculate the PLL reference clock */
      Divider = ((MCG->C6 & MCG_C6_VDIV0_MASK) + 24u);
      MCGOUTClock *= Divider;                       /* Calculate the MCG output clock */
    } /* (!((MCG->C6 & MCG_C6_PLLS_MASK) == 0x0u)) */
  } else if ((MCG->C1 & MCG_C1_CLKS_MASK) == 0x40u) {
    /* Internal reference clock is selected */
    if ((MCG->C2 & MCG_C2_IRCS_MASK) == 0x0u) {
      MCGOUTClock = CPU_INT_SLOW_CLK_HZ;                                       /* Slow internal reference clock selected */
    } else { /* (!((MCG->C2 & MCG_C2_IRCS_MASK) == 0x0u)) */
      MCGOUTClock = CPU_INT_FAST_CLK_HZ / (1 << ((MCG->SC & MCG_SC_FCRDIV_MASK) >> MCG_SC_FCRDIV_SHIFT));  /* Fast internal reference clock selected */
    } /* (!((MCG->C2 & MCG_C2_IRCS_MASK) == 0x0u)) */
  } else if ((MCG->C1 & MCG_C1_CLKS_MASK) == 0x80u) {
    /* External reference clock is selected */
    if ((MCG->C7 & MCG_C7_OSCSEL_MASK) == 0x0u) {
      MCGOUTClock = CPU_XTAL_CLK_HZ;                                           /* System oscillator drives MCG clock */
    } else { /* (!((MCG->C7 & MCG_C7_OSCSEL_MASK) == 0x0u)) */
      MCGOUTClock = CPU_XTAL32k_CLK_HZ;                                        /* RTC 32 kHz oscillator drives MCG clock */
    } /* (!((MCG->C7 & MCG_C7_OSCSEL_MASK) == 0x0u)) */
  } else { /* (!((MCG->C1 & MCG_C1_CLKS_MASK) == 0x80u)) */
    /* Reserved value */
    return;
  } /* (!((MCG->C1 & MCG_C1_CLKS_MASK) == 0x80u)) */
  SystemCoreClock = (MCGOUTClock / (1u + ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> SIM_CLKDIV1_OUTDIV1_SHIFT)));
}
