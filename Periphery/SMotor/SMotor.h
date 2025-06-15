#ifndef __SMotor_H
#define __SMotor_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "common.h"
#include "misc.h"

/******************
°´¼ü
******************/
#define STEP1_RCC   RCC_APB2Periph_GPIOB
#define STEP1_GPIO  GPIOB
#define STEP1_PIN   GPIO_Pin_12
#define STEP1       GPIO_ReadInputDataBit(STEP_GPIO,STEP_PIN)
#define STEP1_ON    GPIO_WriteBit(STEP1_GPIO,STEP1_PIN,Bit_SET);
#define STEP1_OFF   GPIO_WriteBit(STEP1_GPIO,STEP1_PIN,Bit_RESET);

#define STEP2_RCC   RCC_APB2Periph_GPIOB
#define STEP2_GPIO  GPIOB
#define STEP2_PIN   GPIO_Pin_13
#define STEP2       GPIO_ReadInputDataBit(STEP2_GPIO,KEY2_PIN)
#define STEP2_ON    GPIO_WriteBit(STEP2_GPIO,STEP2_PIN,Bit_SET);
#define STEP2_OFF   GPIO_WriteBit(STEP2_GPIO,STEP2_PIN,Bit_RESET);

#define STEP3_RCC   RCC_APB2Periph_GPIOB
#define STEP3_GPIO  GPIOB
#define STEP3_PIN   GPIO_Pin_14
#define STEP3       GPIO_ReadInputDataBit(STEP3_GPIO,STEP3_PIN)
#define STEP3_ON    GPIO_WriteBit(STEP3_GPIO,STEP3_PIN,Bit_SET);
#define STEP3_OFF   GPIO_WriteBit(STEP3_GPIO,STEP3_PIN,Bit_RESET);

#define STEP4_RCC   RCC_APB2Periph_GPIOB
#define STEP4_GPIO  GPIOB
#define STEP4_PIN   GPIO_Pin_15
#define STEP4       GPIO_ReadInputDataBit(STEP4_GPIO,STEP4_PIN)
#define STEP4_ON    GPIO_WriteBit(STEP4_GPIO,STEP4_PIN,Bit_SET);
#define STEP4_OFF   GPIO_WriteBit(STEP4_GPIO,STEP4_PIN,Bit_RESET);

void  SMotor_Init(void);
void  SMotor_FFW(u8 direction);

#endif  
