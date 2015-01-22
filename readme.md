# UsbMidiAttiny45in3

This is a simple Arduino [V-USB](http://www.obdev.at/products/vusb/index.html) MIDI library for ATTiny45/85/44/84.

Arduino環境から、ATTiny45/85/44/84を使ったUSB-MIDIデバイスを作るためのシンプルなライブラリです。
[V-USB](http://www.obdev.at/products/vusb/index.html)をベースにしています。

今のところ、下記機能の送受信にのみ対応しています。

- MIDI NOTE ON
- MIDI NOTE OFF
- MIDI Control Change

## 使い方

### 準備

ArduinoでATTiny45/85を開発するために、[kosakalabさんのhardware.zip](http://make.kosakalab.com/arduino/use/source/hardware.zip)を使わせていただいてます。
あわせて、ATTiny45/85をArduino ISPで焼き込む環境が必要です。
詳細は、[kosakalabさん「Arduino IDEでATtiny他の開発（Arduino-ISP編）」](http://make.kosakalab.com/make/electronic-work/arduino-ide-arduinoisp/)を参照して、ATTiny45/85にファームウェアを書き込み環境を準備してください。

### boards.txtへの追加

ATTiny45/85は16.5Mhzで使います。
このため、例えば、ATTiny45の場合、boards.txtに下記のように定義を追加してください。

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

ATTiny44/84は16Mhzの外部クリスタルを接続してください。
ATTiny44の場合のboards.txtの定義例です。

```boards.txt
attiny44-16.name=ATtiny44 (external 16 MHz clock)
attiny44-16.bootloader.low_fuses=0xfe
attiny44-16.bootloader.high_fuses=0xdf
attiny44-16.bootloader.extended_fuses=0xff
attiny44-16.upload.maximum_size=4096
attiny44-16.build.mcu=attiny44
attiny44-16.build.f_cpu=16000000L
attiny44-16.build.core=arduino
attiny44-16.build.variant=tiny14
```

## 配線

### ATTiny45/85

ユニバーサル基板で工作がしやすい標準サイズのUSB-Bコネクタと実体配線のしやすさの方を優先して、D+はINT0(D2)の代わりにPCINT1(D1)に繋いでいます。なので、シビアな使い方には向いてないかもしれません。

![schematic](schema.png)

- USB電源で動きますが、ATTiny45には5Vを供給してください。
- USBのD-を7番PIN、D+を6番PINに接続します。ツェナーダイオードで3.3Vに降圧させます。
- Arduino ISPを想定してますので、リセットPINは空けといてください。

配線を変える場合、配線に合わせて`usbconfig.h`の`USB_CFG_DPLUS_BIT`などの設定値を変更してください。

### ATTiny44/84

USBのD+は12番PIN、D-は13番PINに接続してください。
あとはATTiny45/85の場合と同様です。

### Libraryフォルダへインストール

UsbMidiAttiny45in3フォルダをそのままArduonoのlibrariesフォルダに入れて、Arduino IDEを再起動すれば使えるようになります。

### examples

####  LED Blink (For ATTiny45/85/44/84)

`/examples/ATTiny_VUSBMIDI_BlinkTest/ATTiny_VUSBMIDI_BlinkTest.ino`

Note ONを受け取ったらLEDを光らせるだけのデモです。
導通テストなどにご利用ください。

####  シンセモドキ (For ATTiny45/85)

`/examples/ATTiny45_Sounder3/ATTiny45_Sounder3.ino`

これは、MIDIキーボードでポルタメントっぽい演奏ができるシンセモドキです。
オシレータのSine Waveの部分は、[Arduino DDS Sinewave Generator](http://interface.khm.de/index.php/lab/experiments/arduino-dds-sinewave-generator/)を参考にしましたが、他の機能を入れたり、ATTiny45に併せてROM削減したので、もとよりかなり荒くなっています。

配線は、[上のMIDI部分の配線図](schema.png)に加え、D4（3番PIN）へオーディオ出力を追加しています。
オーディオ出力の先は、ローパスフィルタや、オペアンプ、スピーカーなどを追加すると良いと思います。

ROM容量はパツパツですが、一応ATTiny45にも収まるように作っています。

#### 「ちーん」 (For ATTiny45/85)

`/examples/ATTiny45_CheenSounder02/ATTiny45_CheenSounder02.ino`

一応、MIDI入出力の例として、最近よく持ち歩いてる「ち〜ん」と鳴るアレのファームを公開しておきます。

[makerhub](https://makershub.jp/make/453)
[youtube](https://www.youtube.com/watch?v=1L06lI9XeK0)

回路図は下記の通りです。
![回路図](cheen-schematic.png)


## ベースソフトウエアとライセンス

V-USBをはじめ、下記ソースをベースにしています。
なので、ライセンスは下記コードのライセンス（非商用に限りGPLv2）に準じます。

- V-USB by Objective Development Software GmbH
	http://www.obdev.at/products/vusb/index.html

- vusb-for-arduino 
  https://code.google.com/p/vusb-for-arduino/downloads/list

- Atmel Attiny45 / Attiny85 based USB MIDI controller by Thorsten
  http://electronicsodyssey.blogspot.jp/2011/10/atmel-attiny45-attiny85-based-usb-midi.html




