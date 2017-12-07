#include "stm32f10x_conf.h"

static __IO uint32_t TimingDelay;

void Delay(__IO uint32_t nTime)
{
    int othertime;
    othertime=0;
    TimingDelay = nTime;
    while(TimingDelay != 0);

}
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00){
        TimingDelay--;
    }
}

void SysTick_Handler(void) {
	TimingDelay_Decrement();
}
