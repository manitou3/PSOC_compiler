#include <stdbool.h>

#include "core_cm3_psoc5.h"
#include "core_cm3.h"
#include "psoc5.h"
#include "libc.h"
#include "lcd.h"


extern uint32_t volatile msTicks;

void WaitForTick(void)
{
    uint32_t curTicks = msTicks; // Save Current SysTick Value
    while (msTicks == curTicks)
    {
        // Wait for next SysTick Interrupt
        __WFE(); // Power-Down until next Event/Interrupt
    }
}

void main(void)
{

#if 0
    // Config data in config.hex, implemented by app_config.c code

    Set_Pin_Drive_Mode(REG_PRT2_PC0, PRT_PC__DRIVE_MODE__STRONG);
    Set_Pin_Drive_Mode(REG_PRT2_PC1, PRT_PC__DRIVE_MODE__STRONG);
    Set_Pin_Drive_Mode(REG_PRT2_PC2, PRT_PC__DRIVE_MODE__STRONG);
    Set_Pin_Drive_Mode(REG_PRT2_PC3, PRT_PC__DRIVE_MODE__STRONG);
    Set_Pin_Drive_Mode(REG_PRT2_PC4, PRT_PC__DRIVE_MODE__STRONG);
    Set_Pin_Drive_Mode(REG_PRT2_PC5, PRT_PC__DRIVE_MODE__STRONG);
    Set_Pin_Drive_Mode(REG_PRT2_PC6, PRT_PC__DRIVE_MODE__STRONG);

    // LED: OUT: Port 6.0 Strong, initially OFF
    // DM 2,1,0: Strong: 1 1 0 (DATA = 1 ON, 0 OFF)
    Set_Pin_Drive_Mode(REG_PRT6_PC0, PRT_PC__DRIVE_MODE__STRONG);
    Clear_Pin(REG_PRT6_PC0);

    // LED: OUT: Port 6.6 Strong, initially OFF
    // DM 2,1,0: Strong: 1 1 0 (DATA = 1 ON, 0 OFF)
    Set_Pin_Drive_Mode(REG_PRT6_PC6, PRT_PC__DRIVE_MODE__STRONG);
    Clear_Pin(REG_PRT6_PC6);

    // Button: IN: Port 6.1 Pull Up
    // DM 2,1,0: Pullup: 0 1 0 (Data = 1 pullup 5K Vcc) [not 0 - hard 0V]
    Set_Pin_Drive_Mode(REG_PRT6_PC1, PRT_PC__DRIVE_MODE__RES_PULLUP);
    Set_Pin(REG_PRT6_PC1);
#endif

    LCD_init_4bit(2, LCD_FONT_5x10);
    LCD_moveto(0,0);
    LCD_write_string("Hello PSoC 3a");

    bool led2 = false;
    uint32_t count = 0;

    // Note: This is a busy wait loop - ok for a demo but not for product code.
    while(1)
    {
        if (Read_Pin_Raw(REG_PRT6_PC1))
            Clear_Pin(REG_PRT6_PC0);
        else
            Set_Pin(REG_PRT6_PC0);

#if 0
        // busy wait
        delay_ms(10);
        if (count++ % 100 == 0) // 10 * 100 = 1000 ms
            led2 = !led2;
#else
        // system sleeps between ticks
        WaitForTick(); // note that this reduces button responsiveness to once per tick (ms)
#endif
        count++;

        if (count % 1000 == 0)
        {
            led2 = !led2;
            LCD_moveto(0,1);
            LCD_printf("count %d", count/1000);
        }

        if (led2)
            Clear_Pin(REG_PRT6_PC6);
        else
            Set_Pin(REG_PRT6_PC6);
    };

    // don't return
}