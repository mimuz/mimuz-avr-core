![mimuz-logo](./mimuz-logo.png)

# mi:muz

`mi:muz`はArduinoとATtinyを使ってMIDIで遊ぶプロジェクト。

ATtinyでMIDIを扱うためのライブラリと、実験用ボードを開発しています。
全てオープンソースです。
自己責任でご自由にご利用ください。

## Arduino Software

ATtinyにMIDI機能を追加するライブラリとArduino IDE 1.6.3向けライブラリです。

- [VUSB-MIDI-ATtiny](https://github.com/tadfmac/mi-muz/tree/master/arduino/libraries/VUSBMidiATtiny) ATtinyでUSB-MIDIデバイスを作るためのArduinoライブラリ
- [設定ファイル(Arduino IDE 1.6.3)](https://github.com/tadfmac/mi-muz/tree/master/arduino/hardware) Arduino IDEにmi:muzボード設定を追加するファイル
- Hybrid MIDI for ATtiny (Comming Soon)

## boards

実験用ボードの回路図を公開しています。

- [mi:muz:wrtr#1](https://github.com/tadfmac/mi-muz/tree/master/boards/wrtr) 8/14ピンATtiny向けの小さなISPシールド
- mi:muz:prot#1 ATtiny45/85用V-USB-MIDI実験用ボード
- [mi:muz:prot#2](https://github.com/tadfmac/mi-muz/tree/master/boards/prot2) ATtiny44/84/841用V-USB-MIDI実験用ボード
- [mi:muz:prot#3](https://github.com/tadfmac/mi-muz/tree/master/boards/prot3) ATtiny45/85用V-USB-MIDI実験用ボード。I2Cマスタ用回路入り
- mi:muz:expr#2 ATtiny841用Hybrid-MIDI実験用ボード) [Comming Soon]

