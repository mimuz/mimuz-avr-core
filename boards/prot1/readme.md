# mi:muz:prot#1 ATtiny45/85 V-USB MIDI Test Board 

シンプルなATtiny45/84用のV-USB MIDI Test Boardです。
（一番最初につくったものですが、公開遅れました）

# Version

v1.0 (2015.05.23)

# Schematics

![mimuz-prot1.png](mimuz-prot1.png)

## parts

- U1:ATtiny45-20SU or ATtiny85-20SU
- JP1:ピンヘッダ (2×3PIN)
- JP2:ピンヘッダ (1×5PIN)
- ID1:Chip Indactor (フェライトビーズ)
- ID2:Chip Indactor (フェライトビーズ)
- ZD1:ツェナーダイオード3.3V
- ZD2:ツェナーダイオード3.3V
- C1:0.1uF
- C2:10uF〜
- R1:100Ω
- R2:100Ω
- R3:1.5KΩ
- R4:470Ω
- LED1:LED

# example

- [blink:MIDI-INを受けてLEDチカするスケッチ](https://github.com/tadfmac/mi-muz/tree/master/arduino/libraries/VUSBMidiATtiny/examples/blink)
- [sounder:シンセモドキ1号](https://github.com/tadfmac/mi-muz/tree/master/arduino/libraries/VUSBMidiATtiny/examples/sounder)
- [「ち〜ん」](https://github.com/tadfmac/mi-muz/tree/master/arduino/libraries/VUSBMidiATtiny/examples/ATTiny45_CheenSounder02)

# Licenses

<a rel="license" href="http://creativecommons.org/licenses/by/4.0/"><img alt="クリエイティブ・コモンズ・ライセンス" style="border-width:0" src="https://i.creativecommons.org/l/by/4.0/88x31.png" /></a><br />この 作品 は <a rel="license" href="http://creativecommons.org/licenses/by/4.0/">クリエイティブ・コモンズ 表示 4.0 国際 ライセンスの下に提供されています。</a>






