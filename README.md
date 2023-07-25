# SolenoidRetainerUnit

<img src="https://github.com/akita11/SolenoidRetainerUnit/blob/main/SolenoidRetainUnit.jpg" width="320px">


[タカハのスリムロック](https://www.takaha.co.jp/co/product-slim-lock/)等の負荷を、「最初短い時間通電したあとは、弱く通電する」という制御を行います。スリムロックでは、通電するとロックが解除されますが、通電を解除すると、再び押し込めばロックされます。「ロックが解除された状態を維持したいが、通電しつづけると負荷に負担がかかる（スリムロックは長時間通電すると、内部の部品が熱で変形して使えなくなります）」という場合に。PWM駆動で「弱く通電した状態」を維持します。（[スイッチサイエンスで委託販売中](https://www.switch-science.com/products/9147)です）

基板形状が「[M5Stack用VH3.96-4ピンユニット](https://www.switch-science.com/products/4055)」と同一のため、そのケースを使うことができます。


## 使い方

オレンジ色のコネクタ(VH3.96-4p)に、負荷用の電源と負荷を接続します。M5Stackの[BAVGソケット](https://www.switch-science.com/products/7234)を使うと、電源は2.1mmプラグ、負荷はスプリング端子で接続できて便利です。

Grove端子のピン配置は以下のとおりです。
- 1. GND
- 2. VDD(3.3〜5V)
- 3. 未使用（内部でVDDにプルアップ）
- 4. 制御入力（内部でVDDにプルアップ）

「4.制御入力」を"0"にすると、最初の200msだけ負荷をONにし、そのあとは、「4.制御入力」が"0"の間はデューティー比10%でPWM駆動します。「4.制御入力」を"1"（または開放）とすると、負荷をOFFにします。


## プログラムの書き換え

このボードは、回路図をみるとわかるように、マイコン(STC8G1K08A)と負荷駆動用のMOSFETからなります。つまりマイコンのプログラム次第で、負荷を自由に制御することができます。

初期状態で書き込んであるプログラムはフォルダFW以下にありますので、必要であればこれを元に改造してください。Grove端子の2本の信号線はマイコンのUART端子につなげてあります。

マイコンSTC8G1K08Aのプログラム開発・書き込みは[こちらの記事](https://note.com/akita11/n/n41316faaaf19)などを参考にしてください。


## Author

Junichi Akita (akita@ifdl.jp, @akita11)
