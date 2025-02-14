# smolboi-firmware-v1.0

Firmware for smolboi V1.0 PCB made using STM32CubeMX.

## Introduction
This repository contains the firmware for the smolboi V1.0 PCB with an STM32F072CBT6 MCU. The MCU is flashable through DFU on the USB-C port, for example through STM32CubeProgrammer. The binary file to be flashed can be found at `/Debug/smolboi-firmware-v1.0.elf`

## Description

Smol Boi is a custom personal 40% keyboard PCB designed to provide a compact and efficient layout. This repository contains the firmware required for the keyboard to function.
PCB files can be found [here](https://github.com/ethanl66/smol-boi)

## Features

- Matrix scanning to detect key presses and key releases
- Generates HID Input Report packets and sends through USB to computer
Currently in development:
- Multilayered keymaps
- Debouncing algorithm
- Auto-repeat mechanism

## Acknowledgments

- Made possible by the plentiful STM32 resource available
- Datasheets! Reference manuals!
