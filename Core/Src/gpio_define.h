/*
 * gpio_define.h
 *
 *  Created on: Feb 12, 2025
 *      Author: ethan
 */

#ifndef SRC_GPIO_DEFINE_H_
#define SRC_GPIO_DEFINE_H_

#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_utils.h"

// Struct to hold GPIO ports and pins for LL API
typedef struct {
	GPIO_TypeDef* port;
	uint32_t pin;
} LL_GPIO_PinDef;

// Array of column GPIO pins in order ([0] == col 0, [1] == col 1, ...)
static const LL_GPIO_PinDef col_pins[] = {
		{GPIOA, LL_GPIO_PIN_4},   // C0 -> PA4
		{GPIOA, LL_GPIO_PIN_5},   // C1 -> PA5
		{GPIOA, LL_GPIO_PIN_6},   // C2 -> PA6
		{GPIOA, LL_GPIO_PIN_7},   // C3 -> PA7
		{GPIOB, LL_GPIO_PIN_0},   // C4 -> PB0
		{GPIOB, LL_GPIO_PIN_1},   // C5 -> PB1
		{GPIOB, LL_GPIO_PIN_2},   // C6 -> PB2
		{GPIOB, LL_GPIO_PIN_10},  // C7 -> PB10
		{GPIOB, LL_GPIO_PIN_11},  // C8 -> PB11
		{GPIOB, LL_GPIO_PIN_6},   // C9 -> PB6
		{GPIOB, LL_GPIO_PIN_5},   // C10 -> PB5
		{GPIOB, LL_GPIO_PIN_4}    // C11 -> PB4
};

// Array of row GPIO pins in order
static const LL_GPIO_PinDef row_pins[] = {
		{GPIOA, LL_GPIO_PIN_15},   // R0 -> PA15
		{GPIOB, LL_GPIO_PIN_3},   // R1 -> PB3
		{GPIOA, LL_GPIO_PIN_14},   // R2 -> PA14
		{GPIOB, LL_GPIO_PIN_7},   // R3 -> PB7
};

#endif /* SRC_GPIO_DEFINE_H_ */
