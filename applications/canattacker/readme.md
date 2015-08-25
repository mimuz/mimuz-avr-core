# can attacker

MIDI信号を受けてソレノイドで缶を叩くやつです。
「アコースティックリズムボックス！」と名乗っていますが、実際は缶を叩くだけの単純なものです。

![can attacker](can.JPG)

# Version

v1.0 (2015.08.25)

# mi:muz:expr#1 Version

[Maker Faire Tokyo 2015](http://makezine.jp/event/makers2015/d_f_mac_at_triparts_music/)で展示したものは、[mi:muz:expr#1](https://github.com/tadfmac/mi-muz/tree/master/boards/expr1)で作ったものです。

mi:muz:expr#1は、LEDに利用しているPB3を除くと、PB1とPB4の2つしか空きポートがありません。
これら２つのポートにそれぞれMOSFET、ソレノイドを1つづつ接続して2つの缶を叩くデバイスを作りました。

## Schematics

![can attacker mi:muz:expr#1 version](canattacker.png)

## parts

- JP1: mi:muz:expr#1
- R1: 1kΩ
- R2: 470Ω
- R3: 1kΩ
- R4: 470KΩ
- Q1/Q2: [2SK4017](http://akizukidenshi.com/catalog/g/gI-07597/)
- D1/D2: 1N4007
- A1/A2: [5V Push Solenoid　ROB-11015](https://www.sparkfun.com/products/11015)

## Firmware

[Firmware](https://github.com/tadfmac/mi-muz/tree/master/applications/canattacker/can1_h/can1_h.ino)

# Licenses

<a rel="license" href="http://creativecommons.org/licenses/by/4.0/"><img alt="クリエイティブ・コモンズ・ライセンス" style="border-width:0" src="https://i.creativecommons.org/l/by/4.0/88x31.png" /></a><br />この 作品 は <a rel="license" href="http://creativecommons.org/licenses/by/4.0/">クリエイティブ・コモンズ 表示 4.0 国際 ライセンスの下に提供されています。</a>





