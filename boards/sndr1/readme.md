# mi:muz:sndr#1 Audio Adaptor for mi:muz

mi:muzで手軽に音の実験を楽しむためのアダプターです。
音量調節できればいいやー、というシンプルなものですので、音質は悪いです。
下記ボードに対応しています。

- mi:muz:expr#1 v0.1 # 2osc対応
- mi:muz:expr#2 v0.1,v0.2,v0.21 # 2osc対応
- mi:muz:prot#3 v1.0 # 1osc対応

# Version

v0.3 (2015.06.25)

# Schematics

![mimuz-sndr1_v03.png](mimuz-sndr1_v03.png)

## parts

コンデンサ容量や抵抗値などは参考値です。下記ですと、かなりローパス強くかかり音がコモりますので、適宜自由に変えてください。

- JP1:ピンヘッダ (5PIN)
- C1:470uF
- C4:0.047uF
- ID1:25mH
- R1:470Ω
- R2:470Ω
- R3:470Ω
- R4:100KΩ
- 10KΩ可変抵抗
- 3.5mmステレオジャック

# example

- [sounder:シンセモドキ1号](https://github.com/tadfmac/mi-muz/tree/master/arduino/libraries/VUSBMidiATtiny/examples/sounder)
- [sounder_h:シンセモドキ1号(Hybrid MIDI Version)](https://github.com/tadfmac/mi-muz/tree/master/arduino/libraries/HybridMidiAttiny/examples/sounder_h)

# Licenses

<a rel="license" href="http://creativecommons.org/licenses/by/4.0/"><img alt="クリエイティブ・コモンズ・ライセンス" style="border-width:0" src="https://i.creativecommons.org/l/by/4.0/88x31.png" /></a><br />この 作品 は <a rel="license" href="http://creativecommons.org/licenses/by/4.0/">クリエイティブ・コモンズ 表示 4.0 国際 ライセンスの下に提供されています。</a>






