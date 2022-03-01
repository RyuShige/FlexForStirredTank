# FlexForStirredTank
NVIDIA FleXのdemoを拡張し、**物理エンジンを撹拌装置へ適応**しました。

回転数変更など、**設計条件の変更をリアルタイムに可視化**することができます。

https://user-images.githubusercontent.com/66407509/156123414-284c170e-5f6e-426b-86fd-e15a6e729177.mp4

# 実行環境
NVIDIA FleXのREADMEを参考にしてください。https://github.com/NVIDIAGameWorks/FleX

難しい場合は検索してみてください。

# 研究概要
## 目標
物理エンジンを撹拌装置へ適応し、**リアルタイムに流れを可視化する。

## 結果，わかったこと
リアルタイム性を考慮すると、システムの精度は十分有効であること。

g_params.vorticityConfinementという渦度に基づく力を流体に与えるパラメータを導入することで、回転翼の力を反応器全体に伝えることができること。

## 検証方法
-2次元での流速分布

-3次元での混合度

## Future Work
-g_params.vorticityConfinement と 回転数の関係 を明らかにする。

-ユーザが回転数を変更した際、自動でvorticityConfinementを調整する機能の実装。

-回転数（撹拌強度）以外の設計変更に対しても有効であることを示す。

# 思ったこと
この研究ではFleXのリアルタイム性に着眼して進めていきましたが、どうしても精度の検証に時間を取られました。

**"リアルタイムにシミュレーションができる"ことをもっと活かした研究**ができると思います。

# 発表資料など
[特別研究発表_要旨.pdf](https://github.com/RyuShige/FlexForStirredTank/files/8159621/_._._2.pdf)

[特別研究_論文.pdf](https://github.com/RyuShige/FlexForStirredTank/files/8159623/_._6.pdf)

[特別研究_パワーポイント.pdf](https://github.com/RyuShige/FlexForStirredTank/files/8159861/_.pdf)

# 参考文献
## 物理エンジンの応用
https://www.jstage.jst.go.jp/article/transjsme/84/859/84_17-00480/_pdf/-char/ja

https://www.researchgate.net/publication/338902736_Simulations_of_realistic_granular_soils_in_oedometer_tests_using_physics_engine

https://easychair.org/publications/open/GsgK  

https://www.jstage.jst.go.jp/article/sabo/69/4/69_16/_pdf/-char/ja

## FleXの基礎理論
https://www.researchgate.net/publication/274479082_Unified_Particle_Physics_for_Real-Time_Applications

https://www.bing.com/search?q=Position+based+fluids&cvid=f7427377a1b94d45b63df6a308cb5eb2&aqs=edge..69i57.385j0j4&FORM=ANAB01&PC=NMTS
