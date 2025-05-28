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

uint8_t isModKey(uint8_t scan_result[NUM_ROWS][NUM_COLS]) {

	uint8_t mod_key_byte = 0;

	if (scan_result[2][11]) {
		mod_key_byte |= KEY_LEFT_SHIFT;
	}
	if (scan_result[3][0]) {
		// Left ctrl
		mod_key_byte |= KEY_LEFT_CTRL;
	}
	if (scan_result[3][1]) {
		// Left win
		mod_key_byte |= KEY_LEFT_GUI;
	}
	if (scan_result[3][2]) {
		// Left alt
		mod_key_byte |= KEY_LEFT_ALT;
	}
	if (scan_result[2][0]) {
		// Left shift
		mod_key_byte |= KEY_LEFT_SHIFT;
	}

	return mod_key_byte;
}

uint8_t isFnKey(uint8_t scan_result[NUM_ROWS][NUM_COLS]) {
	if (scan_result[3][3] == 1 || scan_result[3][7] == 1) {
			return 2;	// fn up
		} else if (scan_result[3][4] == 1) {
			return 1;	// fn down
		} else if (scan_result[3][10] == 1) {
			return 3;	// fn
		}
		return 0;
}

// Evaluate on every keypress
uint8_t capsWord(uint8_t scan_result[NUM_ROWS][NUM_COLS], uint8_t *caps_word, uint8_t fn) {

	// Caps word pressed, toggle state
	if (scan_result[2][11] == 1 && fn == 0) {
		*caps_word ^= 1;	// Toggle caps word on state
	}

	int i;
	// If normal keys not in the call list, set state to 0
	for (i = 2; i < 8; i++) {
		if (	(*caps_word == 1) &&
				((HID_input_buffer[i] < 40) || 	// A-Z, 0-9, KEY_NONE
				(HID_input_buffer[i] == KEY_MINUS) || 
				(HID_input_buffer[i] == KEY_BACKSPACE) || 
				(HID_input_buffer[i] == KEY_DELETE))
		) {
			*caps_word = 0;	
		}
	}	

	// Set shift if caps word state is on (after toggling and checking for call list keys)
	if (*caps_word == 1) {
		HID_input_buffer[0] |= KEY_LEFT_SHIFT;
	}


	return 0;
}

// Evaluate on every keypress
uint8_t keyLock(uint8_t scan_result[NUM_ROWS][NUM_COLS], uint8_t *key_locked, uint8_t *locked_key) {

	if (scan_result[2][11] == 1) {
		*key_locked ^= 1;	// Toggle key lock state

		// Listen for input
		if (*key_locked == 1) {
			while (scanMatrix() == 0) {
				LL_mDelay(20);	// Delay 20 ms
			}
			fillHidInputBuffer();
			for (int i = 2; i < 8; i++) {
				if (HID_input_buffer[i] != 0) {
					*locked_key = HID_input_buffer[i];
					break;
				}
			}
		}
	}

	// Append locked key to buffer
	if (*key_locked == 1) {
		for (int i = 2; i < 8; i++) {
			if (HID_input_buffer[i] == *locked_key) {
				break;
			}
			if (HID_input_buffer[i] != 0) {
				HID_input_buffer[i] = *locked_key;
				break;
			}
		}
	}
	return 0;
}

void fillHidInputBuffer() {
	int i, j;
	uint8_t fn;
	int count = 0;
	uint8_t current_key, mod_key_pressed_at_all = 0, norm_key_pressed_at_all = 0;

	// Check fn layer keys to switch layers
	fn = isFnKey(matrix_scan_result);	// 0 if no fn key, 1 if fn up, 2 if fn down, 3 if fn
	int row_offset = fn * 4;

	// Check mod keys
/*
	==========================================================================================================
	========== NOTE: CHECKING MOD KEYS IN THIS METHOD ONLY WORKS IF THEY ARE PRESENT IN EVERY LAYER ==========
								Otherwise, we need to check using the current_key
	==========================================================================================================
*/
	uint8_t mod_key_byte = isModKey(matrix_scan_result);	// Mod key byte
	if (mod_key_byte) {
		mod_key_pressed_at_all = 1;		// Mark that a mod key was pressed
		HID_input_buffer[0] = mod_key_byte;
	} 

	// Caps word: If caps lock is on, set shift 


	// Check all other keys: Traverse matrix_scan_result
	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++) {

			current_key = usb_keymap[row_offset + i][j];		// Current key to be checked
			
			// Check buffer overflow
			if (count >= 6) {					
				HID_input_buffer[2] = KEY_ERR_OVF;
				HID_input_buffer[3] = KEY_ERR_OVF;
				HID_input_buffer[4] = KEY_ERR_OVF;
				HID_input_buffer[5] = KEY_ERR_OVF;
				HID_input_buffer[6] = KEY_ERR_OVF;
				HID_input_buffer[7] = KEY_ERR_OVF;
				return;
			}

			// Check all other keys (non-mod/fn keys)
			if (
				(matrix_scan_result[i][j] == 1) &&
				(current_key != KEY_LEFT_CTRL) &&
				//(current_key != KEY_LEFT_GUI) &&
				//(current_key != KEY_LEFT_ALT) &&
				(current_key != KEY_LEFT_SHIFT) &&
				(current_key != KEY_NONE)
			) {
				// E and A conflict with KEY_LEFT_GUI and KEY_LEFT_ALT and must be checked manually
				// Also every other modifier bit:
				/*	#define KEY_LEFT_ALT    0x04
					#define KEY_LEFT_GUI    0x08
					#define KEY_RIGHT_CTRL  0x10
					#define KEY_RIGHT_SHIFT 0x20
					#define KEY_RIGHT_ALT   0x40
					#define KEY_RIGHT_GUI   0x80
				*/
				// Check scan_result[][] directly
				if (	current_key == 4 ||
						current_key == 8 ||
						current_key == 0x10 ||
						current_key == 0x20 ||
						current_key == 0x40 ||
						current_key == 0x80
					) {
					if (matrix_scan_result[0][3] == 1) {
						norm_key_pressed_at_all = 1;
						HID_input_buffer[2 + count] = current_key;	// E or 3
						count++;
					}
					if (matrix_scan_result[1][1] == 1) {
						norm_key_pressed_at_all = 1;
						HID_input_buffer[2 + count] = 4;	// A
						count++;
					}
					if (matrix_scan_result[2][7] == 1) {
						norm_key_pressed_at_all = 1;
						HID_input_buffer[2 + count] = KEY_M;	// M
						count++;
					}
					if (matrix_scan_result[0][7] == 1) {
						norm_key_pressed_at_all = 1;
						HID_input_buffer[2 + count] = 0x40;	// F7
						count++;
					}
					if (matrix_scan_result[3][9] == 1) {
						norm_key_pressed_at_all = 1;
						HID_input_buffer[2 + count] = 0x80;	// KEY_VOL_UP
						count++;
					}
				} else {
					// Not E or A
					norm_key_pressed_at_all = 1;		// Mark that a normal key was pressed
					HID_input_buffer[2 + count] = current_key;
					count++;
				}
			}
		}
	}

	// No keys pressed
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

	// If SHIFT is tapped, send a QUOTE key
	if (mod_key_byte == KEY_LEFT_SHIFT && !norm_key_pressed_at_all) {
		HID_input_buffer[2] = KEY_QUOTE;
		//HID_input_buffer[0] = 0;	// We want double quotes
	}

	// Caps word
	capsWord(matrix_scan_result, &caps_word, fn);
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
