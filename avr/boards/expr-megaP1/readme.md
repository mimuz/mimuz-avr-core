# mi:muz:expr-mega#P1 ATmega328 MIDI Test Board 

mi:muzは本来ATtiny用のプロジェクトですが、ATmegaでも動かせないか？と聞かれたのでATmega328P-PUを使ってユニバーサル基板で作ってみました。

現時点ではVUSBMidiATtinyにだけ対応しています。

# Version

v0.01 (2016.03.26)

# Schematics

![mimuz-expr-mega-P1.png](mimuz-expr-mega-p1.png)

## parts

- U1:ATmega328P-PU
- ピンヘッダ (2×3PIN)
- ID1:Chip Indactor (フェライトビーズ)
- ID2:Chip Indactor (フェライトビーズ)
- ZD1:ツェナーダイオード3.6V ※3.3VだとI2Cが動かないので注意
- ZD2:ツェナーダイオード3.6V ※3.3VだとI2Cが動かないので注意
- C1:0.1uF
- C2:22pF
- C3:22pF
- C4:22uF〜
- R1:100Ω
- R2:100Ω
- R3:4.7KΩ（回路図は1.5KΩとなっていますが4.7KΩの方が安定します）
- R4,R5:470-1KΩ
- R6,R8:10KΩ
- R7:CdS
- LED1,LED2:LED
- Q1:16Mhz
- S1:タクトスイッチ

# example

- [blink:MIDI-INを受けてLEDチカするスケッチ](https://github.com/tadfmac/mi-muz/tree/master/arduino/libraries/VUSBMidiATtiny/examples/blink)
- [controller:ボリュームを回すとMIDIコントロール送信](https://github.com/tadfmac/mi-muz/tree/master/arduino/libraries/VUSBMidiATtiny/examples/controller)

# Licenses

<a rel="license" href="http://creativecommons.org/licenses/by/4.0/"><img alt="クリエイティブ・コモンズ・ライセンス" style="border-width:0" src="https://i.creativecommons.org/l/by/4.0/88x31.png" /></a><br />この 作品 は <a rel="license" href="http://creativecommons.org/licenses/by/4.0/">クリエイティブ・コモンズ 表示 4.0 国際 ライセンスの下に提供されています。</a>






