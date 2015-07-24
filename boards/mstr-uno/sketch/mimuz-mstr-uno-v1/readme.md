# mi:muz:mstr:uno sketch

Arduino UNO Rev.3をUSB MIDIとI2C MIDIのブリッジにするためのスケッチです。
mi:muz:mstr:uno v0.1に対応しています。

# dependencies

本スケッチは下記ソフトウエアに依存しています。併せて導入をお願いします。

- [Arduino IDE 1.6.3 or later](https://www.arduino.cc/en/main/software)
- [Arduino MIDI Library by @fortysevenfx](https://github.com/FortySevenEffects/arduino_midi_library/)
- [mocoLufa by @morecat_lab](https://github.com/kuwatay/mocolufa)

# Version

v0.1 (2015.07.24)

# Setup

## 1. mocoLUFAの導入

Arduino UNOのATmega16u2に[mocoLUFA](https://github.com/kuwatay/mocolufa)を導入します。

導入方法は、下記参考サイトを参照してください。

- [本家サイト](http://morecatlab.akiba.coocan.jp/morecat_lab/MocoLUFA.html)
- [Arduino MIDIハック](http://sssslide.com/www.slideshare.net/kuwatay/20131116-asbs13-2)
- [超簡単！Arduino UNOをMIDIコントローラーにしよう！ <Arduino MIDI Library 4.2対応版>](http://qiita.com/tadfmac/items/9136f47ae1eea99a4ef7)
- [Arduinoで作るMIDIコン](http://www.slideshare.net/tadfmac/arduinomidi)

## 2. Arduino MIDI Libraryの導入

[Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library/)をArduinoの`Libraries`フォルダにインストールします。

## 3. Arduino UNO Rev.3へのスケッチの書き込み

本スケッチをArduinoに書き込み、mi:muz:mstr:uno v0.1シールドをArduino UNO Rev.3
に装着することで、mi:muz:exprとの接続が可能になります。

# 機能

## 対応するMIDIメッセージ

本スケッチは現状の[HybridMidiATtiny](https://github.com/tadfmac/mi-muz/blob/master/arduino/libraries/HybridMidiAttiny/)の仕様にあわせ、下記MIDIメッセージにのみ対応しています。

- Note On
- Note Off
- Control Change

## 接続可能なデバイス

下記I2Cチャンネルを設定したmi:muz:exprに対応しています。

[HybridMidiATtiny.h](https://github.com/tadfmac/mi-muz/blob/master/arduino/libraries/HybridMidiAttiny/HybridMidiAttiny.h)の下記いずれかのチャンネルを設定してください。

```HybridMidiATtiny.h
#define HMMAddr_None	0x26
#define HMMAddr_0		0x30
#define HMMAddr_1		0x31
#define HMMAddr_2		0x32
#define HMMAddr_3		0x33
#define HMMAddr_4		0x34
#define HMMAddr_5		0x35
#define HMMAddr_6		0x36
#define HMMAddr_7		0x37
```

本スケッチでは、I2Cチャンネルに応じて下記のように異なる処理を行います。

### HMMAddr_None (0x26) : MIDI Sounder

mi:muz:expr側がMIDI受信のみに対応する場合に設定するチャンネルです。

本スケッチではUSBからの（厳密にはmocoLUFAによって一旦UARTに変換された後の）MIDIメッセージを受信した際に、I2Cバスに対してブロードキャストでMIDIメッセージを転送します。

MIDI受信のみに対応するデバイスは、対応可能なMIDIチャンネルやMIDIメッセージをフィルターして処理する必要があります。

### HMMAddr_0 (0x30) - HMMAddr_7 (0x37) : MIDI Controller

mi:muz:expr側がMIDI送信のみに対応する場合に設定するチャンネルです。

本スケッチでは定期的にI2Cチャンネル0x30〜0x37が設定されたmi:muz:exprに対してMIDI情報の問い合わせを行い、mi:muz:expr側からのMIDIメッセージを回収します。
回収したMIDIメッセージはmocoLUFA経由でUSBに送信します。

# Licenses

<a rel="license" href="http://creativecommons.org/licenses/by/4.0/"><img alt="クリエイティブ・コモンズ・ライセンス" style="border-width:0" src="https://i.creativecommons.org/l/by/4.0/88x31.png" /></a><br />この 作品 は <a rel="license" href="http://creativecommons.org/licenses/by/4.0/">クリエイティブ・コモンズ 表示 4.0 国際 ライセンスの下に提供されています。</a>






