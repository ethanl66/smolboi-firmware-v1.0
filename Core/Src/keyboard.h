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

	static uint8_t usb_keymap_layer1[NUM_ROWS][NUM_COLS] = {
			{ KEY_ESC, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_BACKSPACE },
			{ KEY_TAB, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_QUOTE },
			{ KEY_LEFT_SHIFT, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH, KEY_CAPS_LOCK },
			{ KEY_LEFT_CTRL, KEY_LEFT_GUI, KEY_LEFT_ALT, KEY_NONE, KEY_NONE, KEY_NONE, KEY_SPACE, KEY_NONE, KEY_NONE, KEY_LEFT, KEY_DELETE, KEY_ENTER }
			// 												fn_up, fn_dn						  fn_right
	};

	// matrix_scan_result[0][0] corresponds to usb_keymap_layer1[0][0], etc.
	void fillHidInputBuffer();
	void fillHidInputReport();
	void sendHidReport();

	void print_test_msg();

	// Scan matrix once and populate matrix_scan_result
	uint8_t scanMatrix();	// Returns 0 if no keys pressed and 1 if at least 1 key pressed
	uint8_t scanMatrixTest();	// Toggles col 0 1 second at a time and manually reads from rows

	uint8_t individualKeyTest();

#endif /* SRC_KEYBOARD_H_ */
