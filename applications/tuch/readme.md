# mi:muz:tuch Hybrid MIDI touch sensor board

![mi:muz:tuch](device.png)

タッチセンサーをUSB or I2Cを介したMIDI Note ONに変換するボードです。
PCのUSB端子あるいは、mi:muz:mstrのI2C端子に接続できます。

専用の[Configurator](http://mz4u.net/tuch/)によりNote Numberを変更することができます。

# Usage

- USB1.1互換端子に接続することで、USB機器として認識されます。USBにはV-USBを用いているので、Low Speed Deviceとして動作します。USB2.0対応ハブ経由ですと正しく動作しませんのでご注意ください。
- タッチセンサー端子（大きな穴）に、ワニ口クリップなどを使って缶や野菜を繋ぎます。その後、真ん中のタクトスイッチを押すとキャリブレーションします。
- 繋いだ缶や野菜に触るとMIDI Note ONがPCに送られます！
- Let's Enjoy :D

# Version

v0.11 (2016.07.31)

# Schematic

![Schematic](tuch-schematic.png)

# Top View

![TopView](tuch-top.png)

# Bottom View

![BottomView](tuch-bottom.png)

## parts

- [ATtiny44A-SSU](http://www.atmel.com/ja/jp/devices/ATTINY44A.aspx) * 1
- [ATmega8A-AU](http://www.atmel.com/ja/jp/devices/ATMEGA8A.aspx) * 1
- ツェナーダイオード(SOD323) 3.6V * 2
- クリスタル 16Mhz (5032 2Pad) * 1
- [フェライトビーズ](http://akizukidenshi.com/catalog/g/gP-04442/) * 2 [ID1,ID2]
- 100Ω(0603) * 2 (For USB D+, D- Line)
- 470Ω(0603) * 1 (For ATtiny44 LED)
- 1kΩ(0603) * 6 (For ATmega8A touch line LED)
- 4.7kΩ(0603) * 1 (D+ Line Pull up)
- 10kΩ(0603) * 1 (Tactical switch pull down)
- 1MΩ(2012) * 6 (For Touch Sensor)
- 3mLED 色はお好みで * 7
- 22pF(0603) * 2
- 0.1uF(0603) * 5
- 22uF(0805) * 1
- [タクトスイッチ](http://akizukidenshi.com/catalog/g/gP-03651/) 色はお好みで * 1
- 2.54mm ピンヘッダ 2×3 * 2 (v0.1基盤はATmega8A側は要ジャンパー) ※プログラム書き換えなければ不要

# sketches

- [Touch Sensor (for ATmega8A)](https://github.com/tadfmac/mi-muz/tree/master/applications/tuch/sketch/mega8_touchSensor/)
- [Touch Sensor to Hybrid MIDI bridge (for ATtiny44A)](https://github.com/tadfmac/mi-muz/tree/master/applications/tuch/sketch/mimuz-tuch_v01/)

# Configurator

mi:muz:tuch専用のConfiguratorです。

- [Sample](http://mz4u.net/tuch/)
- [Source](https://github.com/tadfmac/mi-muz/tree/master/applications/tuch/configurator/)

# Known issues

- [#4 bug: mi:muz:tuch ch#5 を触るとUSB通信が停止する問題](https://github.com/tadfmac/mi-muz/issues/4)

# Licenses

## Hardware (Schematics), Configurator

[Creative Commons Attribution 4.0 International (CC BY 4.0)](http://creativecommons.org/licenses/by/4.0/)

## Software (Sketches/Libraries)

[GNU General Public License, version 2 (GPL-2.0)](http://opensource.org/licenses/gpl-2.0.php).

See lso [HybridMidiAttiny](https://github.com/tadfmac/mi-muz/tree/master/arduino/libraries/HybridMidiAttiny).




