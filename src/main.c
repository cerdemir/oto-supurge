/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f10x_conf.h"
#include "main.h"
#define NO_BARRIER 0
#define FRONT_BARRIER 1
#define LEFT_BARRIER  2
#define RIGHT_BARRIER  3

void InitGPIO();
void InitTimer();
void InitPWM();
void MotorSur(int,int);
void Beep();
void MotorToogle();
void InitADC();
int IsBarrier();
int main(void)
{
    //int status =0;
    int barrierStatus = 0;
    InitGPIO();
    InitTimer();
    InitPWM();
    //InitADC();


    if (SysTick_Config(SystemCoreClock / 1000)){
        /* Capture error */
        while (1);
    }
    girgirON();
    vakumON();
    MotorSur(100,100);

    while(1)
    {

printf("aaaaa\n");
        Beep();
        /*
        Delay(100);
        GPIOC->ODR ^= GPIO_Pin_13;
        girgirON();
        Delay(100);
        girgirOFF();
        Delay(100);
        vakumON();
        Delay(100);
        vakumOFF();
        Delay(100);
        MotorToogle();

        Delay(100);
        MotorSur(100,100);
        Delay(100);
        MotorSur(-100,-100);
        Delay(100);
        MotorSur(0,0);
        Delay(100);

        if (status ==1)
        {
           InitPWM(3000);
            status = 0;
            Delay(1000);
        }
        else
        {
            InitPWM(2000);
            status = 1;
            Delay(1000);
        }
        */
        barrierStatus= IsBarrier();
        if(barrierStatus==FRONT_BARRIER)
        {
            MotorSur(-100,100);
            Delay(2000);
        }
        if(barrierStatus==LEFT_BARRIER)
        {
            MotorSur(100,-100);
            Delay(700);
        }
        if(barrierStatus==RIGHT_BARRIER)
        {
            MotorSur(-100,100);
            Delay(700);
        }

        else
        {
            MotorSur(100,100);
        }

        Delay(100);
    }
}
int IsBarrier()
{
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)==0)
    {
        return LEFT_BARRIER;
    }
    else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)==0)
    {
        return RIGHT_BARRIER;
    }
    else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)==0)
    {
        return FRONT_BARRIER;
    }
    else
    {
        return NO_BARRIER;
    }
/*
    int frontSencor = ADC_GetConversionValue(ADC1);
    if (frontSencor >3000)
        return 1;
    else
        return 0;
*/
}
void InitADC(){
    RCC_ADCCLKConfig (RCC_PCLK2_Div2);//ADC freq=PCLK/2 Maximum 12 Mhz olabilir.
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ADC nin Clock u açýk


    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE; //Scan mod çalýþmayacak
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; // Devamlý mod çalýþacak
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // Sonuç Saða Yanaþýk olacak
    ADC_InitStructure.ADC_NbrOfChannel = 2;

    ADC_RegularChannelConfig(ADC1,ADC_Channel_2, 1,ADC_SampleTime_28Cycles5); //
    ADC_Init (ADC1, &ADC_InitStructure); //set config of ADC1

    ADC_Cmd (ADC1,ENABLE); //enable ADC1

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}
void InitTimer(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef timerInitStructure;
    timerInitStructure.TIM_Prescaler =  0;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 3599;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timerInitStructure);


}

void InitPWM(){

TIM_OCInitTypeDef  TIM_OCInitStructure;

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;


    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);


    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);


      TIM_ARRPreloadConfig(TIM2, ENABLE);

    TIM_Cmd(TIM2, ENABLE);
}

void InitGPIO(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;


    //BUZZER
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    //onboard blue led
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);

    //girgir motor
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    //vakum motor
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    //MOTOR SOL 1
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    //MOTOR SOL 2
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    //MOTOR SAG 1
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    //MOTOR SAG 2
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    //MOTOR PWM 1 SOL
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //MOTOR PWM 2 Sag
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //ön sensör
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    //sol sensörler
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //sağ sensörler
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}
void MotorSur(int sol,int sag)
{
    if (sol > 0) //sol ileri
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_0);
        GPIO_ResetBits(GPIOB, GPIO_Pin_1);
    }
    else if(sol< 0) // sol geri
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_1);
        GPIO_ResetBits(GPIOB, GPIO_Pin_0);
        sol=sol*-1;
    }
    else // sol dur
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_0);
        GPIO_ResetBits(GPIOB, GPIO_Pin_1);
    }

    if (sag > 0) //sað ileri
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_11);
        GPIO_SetBits(GPIOB, GPIO_Pin_10);
    }
    else if(sag< 0) // sað geri
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_10);
        GPIO_SetBits(GPIOB, GPIO_Pin_11);
        sag=sag*-1;
    }
    else // sað dur
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_10);
        GPIO_ResetBits(GPIOB, GPIO_Pin_11);
    }



    TIM2->CCR1=sol*3599/100;
    TIM2->CCR2=sag*3599/100;
}
void Beep()
{
    //GPIOA->ODR ^= GPIO_Pin_9;
    GPIO_SetBits(GPIOB, GPIO_Pin_14);
    Delay(100);
    GPIO_ResetBits(GPIOB, GPIO_Pin_14);

}

void MotorToogle()
{
    GPIOB->ODR ^= GPIO_Pin_0;
    GPIOB->ODR ^= GPIO_Pin_1;
    GPIOB->ODR ^= GPIO_Pin_10;
    GPIOB->ODR ^= GPIO_Pin_11;
}

