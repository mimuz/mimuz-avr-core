![mimuz-logo](./mimuz-logo.png)

# mi:muz

`mi:muz`はArduinoとATtinyを使ってMIDIで遊ぶプロジェクト。

ATtinyでMIDIを扱うためのライブラリと、実験用ボードを開発しています。
全てオープンソースです。
自己責任でご自由にご利用ください。

## Arduino Software

ATtinyにMIDI機能を追加するライブラリとArduino IDE 1.6.3向けライブラリです。

- [VUSB-MIDI-ATtiny](https://github.com/tadfmac/mi-muz/tree/master/arduino/libraries/VUSBMidiATtiny)（ATtinyでUSB-MIDIデバイスを作るためのArduinoライブラリ）
- [ArduinoIDE 1.6.3向け設定ファイル](https://github.com/tadfmac/mi-muz/tree/master/arduino/hardware)（Arduino IDEにmi:muzボード設定を追加するファイル）
- [Hybrid MIDI for mi:muz:expr](https://github.com/tadfmac/mi-muz/tree/master/arduino/libraries/HybridMidiAttiny) (mi:muz:exprボード用のArduinoライブラリ。V-USBとI2C Slaveの両方に対応したMIDIデバイスが作れます)

## boards

実験用ボードの回路図を公開しています。

### mi:muz ATtiny Boards (protシリーズ)

ATtinyを搭載しVUSB MIDIを使ったMIDIデバイスを簡単に作るための実験用ボード。
V-USB単体で使う場合はこちら。

- [mi:muz:prot#1](https://github.com/tadfmac/mi-muz/tree/master/boards/prot1)（ATtiny45/85用V-USB-MIDI実験用ボード）
- [mi:muz:prot#2](https://github.com/tadfmac/mi-muz/tree/master/boards/prot2)（ATtiny44/84/841用V-USB-MIDI実験用ボード）
- [mi:muz:prot#3](https://github.com/tadfmac/mi-muz/tree/master/boards/prot3)（ATtiny45/85用V-USB-MIDI実験用ボード。I2Cマスタ用回路入り）

### mi:muz ATtiny Boards (exprシリーズ)

ATtinyを搭載しHybrid MIDIに対応したMIDIデバイスを簡単に作るための実験用ボード。
Arduinoと複数のexprシリーズボードを組み合わせた実験が行いたいときはこちら。

- [mi:muz:expr#1](https://github.com/tadfmac/mi-muz/tree/master/boards/expr1)（ATtiny85用Hybrid-MIDI実験用ボード）
- [mi:muz:expr#2](https://github.com/tadfmac/mi-muz/tree/master/boards/expr2)（ATtiny841用Hybrid-MIDI実験用ボード）

### Arduinoシールド

mi:muzを有効活用するためのArduino UNO用シールド。

- [mi:muz:wrtr](https://github.com/tadfmac/mi-muz/tree/master/boards/wrtr)（8/14ピンATtiny向けの小さなISPシールド）
- mi:muz:mstr-uno (comming soon)

### mi:muz ATtinyボード用アタッチメント

mi:muz ATtinyボードの機能を拡張するためのアタッチメント。

- [mi:muz:sndr#1](https://github.com/tadfmac/mi-muz/tree/master/boards/sndr1)（mi:muz用オーディオアダプタ）
- mi:muz:ctlr (comming soon)

## Applications

`mi:muz`を使ったアプリケーションを紹介します。

- [フィジカルWebアプリ「ち〜ん」](http://qiita.com/tadfmac/items/702e74efad1dd606166a)（MIDIデバイス化した呼び鈴と連携するWebアプリ）
- 12ノブコントローラー（ATtiny44を使った小さなフィジカルコントローラー）
- 缶インスタントバージョン（[mi:muz:prot#2](https://github.com/tadfmac/mi-muz/tree/master/boards/prot2)を使った缶型コントローラー）
- シンセモドキ1号（mi:muz:prot#1にオーディオ出力回路を付けただけのシンプルなMIDI音源）
- シンセモドキ2号（mi:muz:prot#3とI2C接続するATtiny45に音源を内蔵したMIDI音源。1号より安定動作）
- シンセモドキ3号（mi:muz:prot#3とI2C接続するATtiny45×2に音源を内蔵したMIDI音源）


