# HybridMidiAttiny

HybridMidiAttiny is a Arduino MIDI library for mi:muz:expr, it was forked from [VUSBMidiATtiny](https://github.com/tadfmac/mi-muz/tree/master/arduino/libraries/VUSBMidiATtiny).

HybridMidiAttiny provides hybrid mode interface, V-USB or I2C slave (auto selectable) to the following boards.

- mi:muz:expr#1 (ATtiny85)
- mi:muz:expr#2 (ATtiny841)

I2C SlaveとV-USBの2モードに対応したArduino用のMIDIライブラリです。
I2CとUSBのピンを共用する設計になっているmi:muz:exprシリーズのボードでのみ使うことができます。

I2C Slave機能が追加されていることを除き、使い方や機能は[VUSBMidiATtiny](https://github.com/tadfmac/mi-muz/tree/master/arduino/libraries/VUSBMidiATtiny)に準じます。

## Usage

[T.B.D.]

See also [VUSBMidiATtiny](https://github.com/tadfmac/mi-muz/tree/master/arduino/libraries/VUSBMidiATtiny).

## License

HybridMidiATtiny is licenced under the terms of the GPLv2 Licence.
See also [GNU General Public License, version 2 (GPL-2.0)](http://opensource.org/licenses/gpl-2.0.php).

HybridMidiATtiny is based on the following projects.

[1] V-USB by Objective Development Software GmbH
  http://www.obdev.at/products/vusb/index.html

[2] vusb-for-arduino 
  https://code.google.com/p/vusb-for-arduino/downloads/list

[3] Atmel Attiny45 / Attiny85 based USB MIDI controller by Thorsten
  http://electronicsodyssey.blogspot.jp/2011/10/atmel-attiny45-attiny85-based-usb-midi.html

[4] TinyWireS
  http://playground.arduino.cc/Code/USIi2c
  https://github.com/rambo/TinyWire

[5] arduino-vusb-midi-attiny
  https://github.com/tadfmac/arduino-vusb-midi-attiny



