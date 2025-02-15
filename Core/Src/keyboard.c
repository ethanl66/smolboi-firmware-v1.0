/*
 * keyboard.c
 *
 *  Created on: Feb 12, 2025
 *      Author: ethan
 */

#ifndef SRC_KEYBOARD_C_
#define SRC_KEYBOARD_C_

#include "keyboard.h"

void print_test_msg() {
	  // Press 'I'
	  HID_input_report[0] = 0x02;	// L-shift
	  HID_input_report[2] = KEY_I;	// 'I'
	  USBD_HID_SendReport(&hUsbDeviceFS, HID_input_report, 8);	// 02 00 14 00 00 00 00 00
	  HAL_Delay(15);		// USB processing time to finish

	  // Release
	  HID_input_report[0] = 0x00;
	  HID_input_report[2] = 0x00;
	  USBD_HID_SendReport(&hUsbDeviceFS, HID_input_report, 8);
	  HAL_Delay(15);

	  // Press '\'m'
	  HID_input_report[2] = KEY_QUOTE;	// '\''
	  HID_input_report[3] = KEY_M;		// 'm'
	  USBD_HID_SendReport(&hUsbDeviceFS, HID_input_report, 8);	// 02 00 14 00 00 00 00 00
	  HAL_Delay(15);

	  // Release
	  HID_input_report[0] = 0x00;
	  HID_input_report[2] = 0x00;
	  USBD_HID_SendReport(&hUsbDeviceFS, HID_input_report, 8);
	  HAL_Delay(15);

	  // Press ' key.'
	  HID_input_report[2] = KEY_SPACE;
	  HID_input_report[3] = KEY_K;
	  HID_input_report[4] = KEY_E;
	  HID_input_report[5] = KEY_Y;
	  HID_input_report[6] = KEY_PERIOD;
	  USBD_HID_SendReport(&hUsbDeviceFS, HID_input_report, 8);
	  HAL_Delay(15);

	  // Release
	  HID_input_report[2] = 0x00;
	  HID_input_report[3] = 0x00;
	  HID_input_report[4] = 0x00;
	  HID_input_report[5] = 0x00;
	  HID_input_report[6] = 0x00;
	  USBD_HID_SendReport(&hUsbDeviceFS, HID_input_report, 8);
	  HAL_Delay(15);

	  //HAL_Delay(1000);
  }

uint8_t scanMatrix() {

	uint8_t col;
	uint8_t row;
	uint8_t key_pressed = 0;	// 0: no key pressed at all. 1: any key pressed

	// For scanning
	uint32_t current_row_state;

	// Columns should've been initialized low

	// Scanning loop
	for (col = 0; col < NUM_COLS; col++) {

		// Set a column to high
		LL_GPIO_SetOutputPin(col_pins[col].port, col_pins[col].pin);

		// Read rows.
		// If any row is high, that means key at key[row][col] was pressed.
		for (row = 0; row < NUM_ROWS; row++) {
			current_row_state = LL_GPIO_IsInputPinSet(row_pins[row].port, row_pins[row].pin);	// State of current row?
			if (current_row_state == 1) {
				// Key was pressed! Or maybe... (debouncing will figure out the truth)
				// For now (before debouncing implementation), assume pressed
				matrix_scan_result[row][col] = 1;	// Mark key (row state) as pressed
				key_pressed = 1;
			} else if (current_row_state == 0) {
				matrix_scan_result[row][col] = 0;	// Mark key (row state) as not pressed
			}
			// No delay needed between columns
		}

		// Return column to low
		LL_GPIO_ResetOutputPin(col_pins[col].port, col_pins[col].pin);
	}

	return key_pressed;
}

uint8_t scanMatrixTest() {

	uint8_t key_pressed = 0;

	// For scanning
	uint32_t row_0_state;
	uint32_t row_1_state;
	uint32_t row_2_state;
	uint32_t row_3_state;

	// Set col 0 low
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4);

	LL_mDelay(1000);	// Delay 1s


	// Scanning loop

		// Set col 0 to high
		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_4);

		LL_mDelay(1000);	// Delay 1s

		// Read rows.
		// If any row is low, that means key at key[row][col] was pressed.

		row_0_state = LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_15);	// row 0
		row_1_state = LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_3);	// row 1
		row_2_state = LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_14);	// row 2
		row_3_state = LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_7);	// row 3

		// High means pressed
		if (row_0_state + row_1_state + row_2_state + row_3_state > 0) {
			key_pressed = 1;
		}

		// Return column 0 low
		LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4);

	return key_pressed;
}

uint8_t isModifierKey(uint8_t row, uint8_t col) {
	if (	(row == 3 && col == 0) ||
			(row == 3 && col == 1) ||
			(row == 3 && col == 2) ||
			(row == 2 && col == 0)	) {
		return 1;
	}
	return 0;
}
void fillHidInputBuffer() {
	int row, col;
	int count = 0;
	uint8_t current_key, mod_key_pressed_at_all = 0, norm_key_pressed_at_all = 0;

	for (row = 0; row < NUM_ROWS; row++) {
		for (col = 0; col < NUM_COLS; col++) {

			current_key = usb_keymap_layer1[row][col];	// Current key to be checked
			if (count >= 6) {							// If buffer overflow
				HID_input_buffer[2] = KEY_ERR_OVF;
				HID_input_buffer[3] = KEY_ERR_OVF;
				HID_input_buffer[4] = KEY_ERR_OVF;
				HID_input_buffer[5] = KEY_ERR_OVF;
				HID_input_buffer[6] = KEY_ERR_OVF;
				HID_input_buffer[7] = KEY_ERR_OVF;
				return;
			}
			if (matrix_scan_result[row][col] == 1 && count < 6) {	// If key is pressed and buffer not full

					// Current key down is modifier key
				if (isModifierKey(row, col)) {
					mod_key_pressed_at_all = 1;		// Mark that a mod key was pressed
					HID_input_buffer[0] = current_key;
				} else {
					// Current key down is normal key
					norm_key_pressed_at_all = 1;		// Mark that a mod key was pressed
					HID_input_buffer[2 + count] = current_key;
				}
				count++;

			}

			if (!mod_key_pressed_at_all) {
				HID_input_buffer[0] = 0;
			}
			if (!norm_key_pressed_at_all) {
				HID_input_buffer[2] = 0;
				HID_input_buffer[3] = 0;
				HID_input_buffer[4] = 0;
				HID_input_buffer[5] = 0;
				HID_input_buffer[6] = 0;
				HID_input_buffer[7] = 0;
			}
		}
	}
}

void fillHidInputReport() {
	int i;
	for (i = 0; i < 8; i++) {
		HID_input_report[i] = HID_input_buffer[i];
	}
}

void sendHidReport() {
	USBD_HID_SendReport(&hUsbDeviceFS, HID_input_report, 8);
	HAL_Delay(15);		// USB processing time to finish
}

#endif /* SRC_KEYBOARD_C_ */
