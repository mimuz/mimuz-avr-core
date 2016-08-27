# mi:muz:tuch (MIDI Controller with flexible touch keys) AVR Version

![mi:muz:tuch](device.png)

mi:muz:tuch is a USB MIDI controller with capacitive touch sensor.
It has now two indivisual versions, AVR or STM32.

And also, Web preset configurator is available to [Configurator](http://mz4u.net/tuch/).
MIDI note numbers be able to change to You want.  

# AVR Version (ATtiny44 16Mhz with ATmega8A 8Mhz)

AVR version of mi:muz:tuch (v0.1x) is based mi:muz:expr#2 for 'Hybrid MIDI feature, It can be able to connect to PC's USB Port using V-USB or mi:muz:mstr (I2C master) devices's I2C port using I2C slave mode.

## Version

- v0.11 (2016.07.31)

## Known Issues

- v0.1x does not work properly when connected to a USB2.0 Hub. (V-USB's general issue... unfixable...)
- [v0.1x does not work USB Mode when ch#6 is enable.] (https://github.com/tadfmac/mi-muz/issues/4)

## Schematic

![Schematic](tuch-schematic-011.png)

## Top View

![TopView](tuch-top-011.png)

## Bottom View

![BottomView](tuch-bottom-011.png)

## Eagle Files

[eagle files (v0.11)](./eagle-files/v0.11)

## parts

- [ATtiny44A-SSU](http://www.atmel.com/ja/jp/devices/ATTINY44A.aspx) * 1
- [ATmega8A-AU](http://www.atmel.com/ja/jp/devices/ATMEGA8A.aspx) * 1
- Zener diode(SOD323) 3.6V * 2
- Crystal 16Mhz (5032 2Pad) * 1
- [Ferrite beads](http://akizukidenshi.com/catalog/g/gP-04054/) * 2 [ID1,ID2]
- 100Ω(0603) * 2 (For USB D+, D- Line)
- 470Ω(0603) * 1 (For ATtiny44 LED)
- 1kΩ(0603) * 6 (For ATmega8A touch line LED)
- 4.7kΩ(0603) * 1 (D- Line Pull up)
- 10kΩ(0603) * 1 (Tactical switch pull down)
- 1MΩ(2012) * 6 (For Touch Sensor)
- 3mLED * 7 
- 22pF(0603) * 2
- 0.1uF(0603) * 5
- 22uF(0805) * 1
- [Switch](http://akizukidenshi.com/catalog/g/gP-03651/) * 1
- Pin headers (2.54mm) 2×3 * 2
  WARNING: if you have v0.1 board, it has a bug that missing junction from ATmega8A to ISP.

## sketches

- [Touch Sensor (for ATmega8A)](https://github.com/tadfmac/mi-muz/tree/master/applications/tuch/sketch/mega8_touchSensor/)
- [Touch Sensor to Hybrid MIDI bridge (for ATtiny44A)](https://github.com/tadfmac/mi-muz/tree/master/applications/tuch/sketch/mimuz-tuch_v01/)

# Licenses

## Hardware (Schematics), Configurator

[Creative Commons Attribution 4.0 International (CC BY 4.0)](http://creativecommons.org/licenses/by/4.0/)

## Software (Sketches/Libraries)

[GNU General Public License, version 2 (GPL-2.0)](http://opensource.org/licenses/gpl-2.0.php).

See lso [HybridMidiAttiny](https://github.com/tadfmac/mi-muz/tree/master/arduino/libraries/HybridMidiAttiny).




