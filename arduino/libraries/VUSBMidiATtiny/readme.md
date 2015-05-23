# VUSBMidiAttiny

A very simple Arduino [V-USB](http://www.obdev.at/products/vusb/index.html) MIDI library for ATtiny Microcontroller.

ATtiny向けのシンプルなArduino [V-USB](http://www.obdev.at/products/vusb/index.html) MIDIライブラリです。

今のところ下記MIDIメッセージの送受信にのみ対応しています。

- MIDI NOTE ON
- MIDI NOTE OFF
- MIDI Control Change

The following ATtiny AVR are supported:
下記ATtiny AVRをサポートしています。

- ATtiny45/85 : with Arduino IDE 1.0.x or 1.6.3
- ATtiny44/84 : with Arduino IDE 1.0.x or 1.6.3
- ATtiny841 (maybe can support 441 but i cannot test it yet.) : Only Support Arduino IDE 1.6.3

## 準備（開発環境）

ATtinyをArduino ISPで焼き込む環境が必要です。

### Arduino IDE 1.6.3

Arduino IDE ver.1.6.3用にボード設定を用意しました。

`./hardware/mimuz` フォルダを、`Arduino.app/Contents/Java/hardware/`直下にコピーして、Arduino IDEを再起動してください。

詳細は、`./hardware/mimuz/avr/board.txt`を参照ください。

### Arduino IDE 1.0.x

Arduino IDE 1.0.xをご利用の場合、[kosakalabさん「Arduino IDEでATtiny他の開発（Arduino-ISP編）」](http://make.kosakalab.com/make/electronic-work/arduino-ide-arduinoisp/)などを参照して、ATtinyにファームウェアを書き込む環境を準備してください。

ただし、Arduino IDE 1.0.xではATtiny441/841はサポートされておりません。
ATtiny441/841をご利用の際は、Arduino IDE 1.6.3をご利用ください。

Arduino IDEの1.0.xのboard.txt記述例です。

#### ATTiny45 (16.5Mhz Internal Oscillator) for Arduino IDE 1.0.x

```boards.txt
attiny45-16.name=ATtiny45 (internal 16.5 MHz clock)
attiny45-16.bootloader.low_fuses=0xe1
attiny45-16.bootloader.high_fuses=0xdf
attiny45-16.bootloader.extended_fuses=0xff
attiny45-16.upload.maximum_size=4096
attiny45-16.bootloader.path=dummy
attiny45-16.bootloader.file=dummy.hex
attiny45-16.build.mcu=attiny45
attiny45-16.build.f_cpu=16500000L
attiny45-16.build.core=arduino
attiny45-16.build.variant=tiny8
```

#### ATTiny44 (16.0Mhz External Oscillator) for Arduino IDE 1.0.x

```boards.txt
attiny44-16.name=ATtiny44 (external 16.0 MHz clock)
attiny44-16.bootloader.low_fuses=0xfe
attiny44-16.bootloader.high_fuses=0xdf
attiny44-16.bootloader.extended_fuses=0xff
attiny44-16.upload.maximum_size=4096
attiny44-16.build.mcu=attiny44
attiny44-16.build.f_cpu=16000000L
attiny44-16.build.core=arduino
attiny44-16.build.variant=tiny14
```

## インストール（ライブラリ）

`libraries/VUSBMidiATtiny`フォルダをそのままArduonoのlibrariesフォルダに入れて、Arduino IDEを再起動すれば使えるようになります。

## リファレンスボード (mi:muz)

下記3種類のボード設定に対応しています。
適宜 usbconfig.hの設定を変更することで、自由にピンアサインを変更可能です。

### [mi:muz:prot#1 (ATTiny45/85)](https://github.com/tadfmac/mi-muz/tree/master/boards/prot1)

- D+ PB1
- D- PB2

### [mi:muz:prot#2 (ATTiny44/84/841)](https://github.com/tadfmac/mi-muz/tree/master/boards/prot2)

- D+ PA1
- D- PA0

### [mi:muz:prot#3 (ATTiny45/85)](https://github.com/tadfmac/mi-muz/tree/master/boards/prot3)

- D+ PB1
- D- PB3

## examples

### blink (For ATTiny45/85/44/84/841)

`/examples/blink/blink.ino`

Note ONを受け取ったらLEDを光らせるだけのデモです。
導通テストなどにご利用ください。

### controller (For ATTiny45/85/44/84/841)

`/examples/controller/controller.ino`

アナログ入力１つのコントローラーのデモです。
アナログ入力に可変抵抗を繋いで回せば、コントトールチェンジを送信します。

### シンセモドキ (For mi:muz:prot1 ATTiny45/85)

`/examples/sounder/sounder.ino`

これは、MIDIキーボードでポルタメントっぽい演奏ができるシンセモドキです。
オシレータのSine Waveの部分は、[Arduino DDS Sinewave Generator](http://interface.khm.de/index.php/lab/experiments/arduino-dds-sinewave-generator/)を参考にしましたが、他の機能を入れたり、ATTiny45に併せてROM削減したので、もとよりかなり荒くなっています。

[mi:muz:prot#1 (ATTiny45/85)](https://github.com/tadfmac/mi-muz/tree/master/boards/prot1)をベースとしてD4（3番PIN）へオーディオ出力を追加してください。

ROM容量はパツパツですが、一応ATTiny45にも収まるように作っています。

## ベースソフトウエアとライセンス

V-USBをはじめ、下記ソースをベースにしています。
なので、ライセンスは下記コードのライセンス（非商用に限りGPLv2）に準じます。

- V-USB by Objective Development Software GmbH
	http://www.obdev.at/products/vusb/index.html

- vusb-for-arduino 
  https://code.google.com/p/vusb-for-arduino/downloads/list

- Atmel Attiny45 / Attiny85 based USB MIDI controller by Thorsten
  http://electronicsodyssey.blogspot.jp/2011/10/atmel-attiny45-attiny85-based-usb-midi.html




