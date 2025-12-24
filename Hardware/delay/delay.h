#ifndef _DELAY_H
#define _DELAY_H

#include "main.h"

//-----------------------------------------------------------------
// ����
//-----------------------------------------------------------------
#define SystemClock				168				// ϵͳʱ�ӣ���λ��MHz��
#define FAC_us				SystemClock/4	// ��ʱ����
//-----------------------------------------------------------------
// ��������
//-----------------------------------------------------------------
extern void delay_20ns(uint32_t nns);
extern void delay_ms(uint16_t nms);
extern void delay_us(uint32_t nus);
extern void SysTick_clkconfig(uint8_t SYSCLK);
extern void SysTick_delay_us(uint32_t nus);
extern void SysTick_delay_ms(uint16_t nms);
extern void delay_ns(uint32_t nns);
void bsp_DelayUS(uint32_t n);
#endif
//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
