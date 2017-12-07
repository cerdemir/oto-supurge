#ifndef DELAY_H_INCLUDED
#define DELAY_H_INCLUDED


void TimingDelay_Decrement(void);
void SysTick_Handler(void) ;
void Delay(__IO uint32_t nTime);

#endif /* DELAY_H_INCLUDED */
