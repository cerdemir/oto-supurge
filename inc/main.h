#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "delay.h"

#define girgirON() (GPIO_SetBits(GPIOA, GPIO_Pin_9))
#define girgirOFF() (GPIO_ResetBits(GPIOA, GPIO_Pin_9))

#define vakumON() (GPIO_SetBits(GPIOA, GPIO_Pin_8))
#define vakumOFF() (GPIO_ResetBits(GPIOA, GPIO_Pin_8))

#endif /* MAIN_H_INCLUDED */
