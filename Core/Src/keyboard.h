/*
 * keyboard.h
 *
 *  Created on: Feb 12, 2025
 *      Author: ethan
 */

#ifndef SRC_KEYBOARD_H_
#define SRC_KEYBOARD_H_

//#include <stdint.h>
#include "usb_device.h"
#include "usbd_hid.h"	// USBD_HID_SendReport?
#include "gpio_define.h"
#include "usb_keycodes_define.h"

// Keyboard constants
#define NUM_ROWS 4
#define NUM_COLS 12
#define NUM_LAYERS 3
// static uint8_t num_rows = NUM_ROWS;
// static uint8_t num_cols = NUM_COLS;

// USB
extern USBD_HandleTypeDef hUsbDeviceFS;	// Declare handle
static uint8_t HID_input_report[8] = {0};		// Array to store 8 byte input report
static uint8_t HID_input_buffer[8] = {0};		// Buffer to store matrix scan results

// Matrix Scanning

	// Mapping
	static uint8_t matrix_scan_result[NUM_ROWS][NUM_COLS] = {0};
	/*
	 * { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 *   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 *   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 *   0, 0 ,0 ,0 ,0, 0, \, 0, \, 0, 0, 0
	 *   }
	 *
	 *   For empty keys, the row will simply never return a HIGH when that column is being scanned (is always 0).
	 */

	static uint8_t usb_keymap[NUM_ROWS * NUM_LAYERS][NUM_COLS] = {
		// Layer 1 (row 0-3)
			{ KEY_ESC, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_BACKSPACE },
			{ KEY_TAB, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_ENTER },
			{ KEY_LEFT_SHIFT, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH, KEY_NONE }, // caps_word
			{ KEY_LEFT_CTRL, KEY_LEFT_GUI, KEY_LEFT_ALT, KEY_NONE, KEY_NONE, KEY_NONE, KEY_SPACE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_DELETE, KEY_ENTER },
			// 												fn_up, fn_dn						  fn_up					fn_lock?
		// Layer 2 (up) (row 4-7)
			//				!	  @		 #		$	   %	  ^		 &		*	  (		 )
			{ KEY_TILDE, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_NONE },
			{ KEY_CAPS_LOCK, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_MINUS, KEY_EQUAL, KEY_LEFT_BRACE, KEY_RIGHT_BRACE, KEY_BACKSLASH },
			{ KEY_NONE, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE },
			{ KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_INSERT, KEY_PRINTSCREEN },
			//									 vol+	  vol-
		// Layer 3 (down) (row 8-11)
			{ KEY_NONE, KEY_NONE, KEY_UP, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_7, KEY_8, KEY_9, KEY_NONE, KEY_DELETE },
			{ KEY_NONE, KEY_LEFT, KEY_DOWN, KEY_RIGHT, KEY_NONE, KEY_NONE, KEY_NONE, KEY_4, KEY_5, KEY_6, KEY_NONE, KEY_ENTER },
			{ KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_1, KEY_2, KEY_3, KEY_NONE, KEY_NONE },
			{ KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_0, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE }
	};

	// matrix_scan_result[0][0] corresponds to usb_keymap_layer1[0][0], etc.
	void fillHidInputBuffer();
	void fillHidInputReport();
	void sendHidReport();

	void print_test_msg();

	// Scan matrix once and populate matrix_scan_result
	uint8_t scanMatrix();	// Returns 0 if no keys pressed, 1 if at least 1 key pressed
	uint8_t scanMatrixTest();	// Toggles col 0 1 second at a time and manually reads from rows

	// Helper functions
	uint8_t isModifierKey(uint8_t row, uint8_t col);
	uint8_t isFnKey(uint8_t scan_result[NUM_ROWS][NUM_COLS]);		// Returns 0 if not fn key, 1 if up, 2 if down

	uint8_t individualKeyTest();

#endif /* SRC_KEYBOARD_H_ */
