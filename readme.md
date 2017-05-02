![mimuz-logo](./mimuz-logo.png)

# mi:muz avr-core

`mi:muz avr-core`はATtiny向けのArduino用MIDIライブラリです。

------

## Arduino Library

Arduinoを使ってMIDIで遊ぶためのATtinyにMIDI機能を追加するライブラリとArduino IDE 1.6.3向けの設定ファイル。

- [VUSB-MIDI-ATtiny](./arduino/libraries/VUSBMidiATtiny)（ATtinyでUSB-MIDIデバイスを作るためのArduinoライブラリ）
- [ArduinoIDE 1.6.3向け設定ファイル](./arduino/hardware)（Arduino IDEにmi:muzボード設定を追加するファイル）
- [Hybrid MIDI for mi:muz:expr](./arduino/libraries/HybridMidiAttiny) (mi:muz:exprボード用のArduinoライブラリ。V-USBとI2C Slaveの両方に対応したMIDIデバイスが作れます)

## boards

mi:muz avr-coreに対応したボードの回路図とサンプルコードです

### mi:muz ATtiny Boards (protシリーズ)

ATtinyを搭載しV-USB MIDIを使ったMIDIデバイスを簡単に作るための実験用ボード。

- [mi:muz:prot#1](./boards/prot1)（ATtiny45/85用V-USB-MIDI実験用ボード）
- [mi:muz:prot#2](./boards/prot2)（ATtiny44/84/841用V-USB-MIDI実験用ボード）
- [mi:muz:prot#3](./boards/prot3)（ATtiny45/85用V-USB-MIDI実験用ボード。I2Cマスタ用回路入り）

### mi:muz ATtiny Boards (exprシリーズ)

ATtinyを搭載しHybrid MIDIに対応したMIDIデバイスを簡単に作るための実験用ボード。
Arduinoと複数のexprシリーズボードを組み合わせた実験が行いたいときはこちら。

- [mi:muz:expr#1](./boards/expr1)（ATtiny85用Hybrid-MIDI実験用ボード）
- [mi:muz:expr#2](./boards/expr2)（ATtiny841用Hybrid-MIDI実験用ボード）

### Arduinoシールド

mi:muzを有効活用するためのArduino UNO用シールド。

- [mi:muz:wrtr](./boards/wrtr)（8/14ピンATtiny向けの小さなISPシールド）
- [mi:muz:mstr-uno](./boards/mstr-uno) (mi:muz:exprをArduino UNOに接続するためのシールド)

### mi:muz ATtinyボード用アタッチメント

mi:muz ATtinyボードを使って簡単にデバイスの実験を行うためのアタッチメント。

- [mi:muz:sndr#1](./boards/sndr1)（mi:muz:expr#1,#2用オーディオアダプタ）
- [mi:muz:ctlr#1](./boards/ctlr1)（mi:muz:expr#2用 8chコントローラーアダプタ）

# Applications

`mi:muz`プロジェクトで生まれたデバイスを紹介しています。

- [フィジカルWebアプリ「ち〜ん」](http://qiita.com/tadfmac/items/702e74efad1dd606166a)（MIDIデバイス化した呼び鈴と連携するWebアプリ）
- [mi:muz:can (旧名：can attacker (缶たたくやつ))](https://github.com/mimuz/mimuz-can)
- [mi:muz:tuch](https://github.com/mimuz/mimuz-tuch) MIDI Controller with flexible touch keys.

