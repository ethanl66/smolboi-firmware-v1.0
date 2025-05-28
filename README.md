# smolboi-firmware-v1.0

Firmware for smolboi V1.0 PCB made using STM32CubeMX.

## Introduction
This repository contains the firmware for the smolboi V1.0 PCB with an STM32F072CBT6 MCU. The MCU is flashable through DFU on the USB-C port, for example through STM32CubeProgrammer. The binary file to be flashed can be found at `/Debug/smolboi-firmware-v1.0.elf`

## Description

Smol Boi is a custom personal 40% keyboard PCB designed to provide a compact and efficient layout. This repository contains the firmware required for the keyboard to function.
PCB files can be found [here](https://github.com/ethanl66/smol-boi).

## Features

- Matrix scanning to detect key presses and key releases
- Maps keypresses to a custom 4-layer keymap
- Generates HID Input Report packets and sends through USB to computer
- Primitive debouncing

Currently in development:
- Multilayered keymaps
- More advanced debouncing algorithm
- Explicit auto-repeat mechanism
- Cool custom key functions e.g. caps word, tap `SHIFT` for `"`, key lock

## Acknowledgments

- Made possible by the plentiful STM32 resources available
- Datasheets! Reference manuals!
