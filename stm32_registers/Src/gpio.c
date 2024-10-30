/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

void MX_GPIO_Init(void)
{
  //RCC_AHB1_Clock_Enable_Disable AHB1 Peripheral Clock Enable Disable
  RCC->AHB1ENR  |= 1UL;
  

  /*
   Init gpio output PA6 PA7
  */
  GPIOA->MODER  |= (1<<12);
  GPIOA->MODER  |= (1<<14);

  GPIOA->OTYPER &=~((1<<6)|(1<<7));

  GPIOA->OSPEEDR &=~((3<<14)|(3<<12));

  GPIOA->PUPDR  |=((1<<14)|(1<<12));

  GPIOA->ODR |=((1<<6)|(1<<7));

  /*
  USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
  */
  
  //USART1 clock enable
  RCC->APB2ENR  |=  (0x1UL << 4U);

  //Select Alternate function mode
  GPIOA->MODER  = ((GPIOA->MODER & ~(0x3UL << 18U)) | (0x2UL << 18U));
  GPIOA->MODER  = ((GPIOA->MODER & ~(0x3UL << 20U)) | (0x2UL << 20U));

  //configure the output type of the I/O port (Output push-pul)
  GPIOA->OTYPER &=~((1<<9)|(1<<10));

  //configure the I/O output speed (Very high speed)
  GPIOA->OSPEEDR = ((GPIOA->OSPEEDR & ~(0x3UL << 18U)) | (0x3UL << 18U));
  GPIOA->OSPEEDR = ((GPIOA->OSPEEDR & ~(0x3UL << 20U)) | (0x3UL << 20U));

  //configure the I/O pull-up or pull-down (No pull-up, pull-down)
  GPIOA->PUPDR = ((GPIOA->PUPDR & ~(0x3UL << 18U)) | (0x0UL << 18U));
  GPIOA->PUPDR = ((GPIOA->PUPDR & ~(0x3UL << 20U)) | (0x0UL << 20U));

  // Selecting an alternate function
  GPIOA->AFR[1] =
  GPIOA->AFR[1] = ((GPIOA->AFR[1] & ~(0xFUL << 4U)) | (0x7UL << 4U));
  GPIOA->AFR[1] = ((GPIOA->AFR[1] & ~(0xFUL << 8U)) | (0x7UL << 8U));
/*



*/
  //GPIOB clock enable
  RCC->AHB1ENR  |= (0x1UL << 1U);
  //SPI clock enable
  RCC->APB2ENR  |= (0x1UL << 12U);

  //Select Alternate function mode
  GPIOB->MODER  = ((GPIOB->MODER & ~(0x3UL << 6U)) | (0x2UL << 6U));
  GPIOB->MODER  = ((GPIOB->MODER & ~(0x3UL << 8U)) | (0x2UL << 8U));
  GPIOB->MODER  = ((GPIOB->MODER & ~(0x3UL << 10U)) | (0x2UL << 10U));

  //configure the output type of the I/O port (Output push-pul)
  GPIOA->OTYPER &=~((1<<3)|(1<<4)|(1<<5));

  //configure the I/O output speed (Very high speed)
  GPIOB->OSPEEDR = ((GPIOB->OSPEEDR & ~(0x3UL << 6U)) | (0x3UL << 6U));
  GPIOB->OSPEEDR = ((GPIOB->OSPEEDR & ~(0x3UL << 8U)) | (0x3UL << 8U));
  GPIOB->OSPEEDR = ((GPIOB->OSPEEDR & ~(0x3UL << 10U)) | (0x3UL << 10U));

  //configure the I/O pull-up or pull-down (No pull-up, pull-down)
  GPIOB->PUPDR = ((GPIOB->PUPDR & ~(0x3UL << 6U)) | (0x0UL << 6U));
  GPIOB->PUPDR = ((GPIOB->PUPDR & ~(0x3UL << 8U)) | (0x0UL << 8U));
  GPIOB->PUPDR = ((GPIOB->PUPDR & ~(0x3UL << 10U)) | (0x0UL << 10U));

  // Selecting an alternate function
  GPIOB->AFR[0] = ((GPIOB->AFR[0] & ~(0xFUL << 12U)) | (0x5UL << 12U));
  GPIOB->AFR[0] = ((GPIOB->AFR[0] & ~(0xFUL << 16U)) | (0x5UL << 16U));
  GPIOB->AFR[0] = ((GPIOB->AFR[0] & ~(0xFUL << 20U)) | (0x5UL << 20U));

    /*
   Init gpio output PB0
  */
  GPIOB->MODER  |= (0x1UL << 0U);
  GPIOB->OTYPER &=~(0x1UL << 0U);
  GPIOB->OSPEEDR = ((GPIOB->OSPEEDR & ~(0x3UL << 0U)) | (0x3UL << 0U));
  GPIOB->PUPDR = ((GPIOB->PUPDR & ~(0x3UL << 0U)) | (0x0UL << 0U));
  GPIOB->ODR |=(1<<0);

}
void gpio_tog(GPIO_TypeDef * _gpio, uint16_t Pin)
{
  uint32_t reg_b;
  reg_b = _gpio->ODR;
  _gpio->BSRR = ((reg_b & (0x1UL << Pin)) << 16U) | (~reg_b & (0x1UL << Pin));
}
void gpio_write(GPIO_TypeDef * _gpio, uint16_t Pin, uint8_t val)
{
  if(val == 0U)  {_gpio->ODR &= ~(0x1UL << Pin);}
  else           {_gpio->ODR |=  (0x1UL << Pin);}
}