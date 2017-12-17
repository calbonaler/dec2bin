# dec2bin
このプログラムには次の機能があります:
* 2進数値を十進数値に変換する
* 変換された2進数値と入力された2進数値との一致を調べる

# 使用法
1. プログラムを実行します。評価ループが開始されます。
2. 行いたいことにしたがってコマンドを入力します:
   * 変換を行いたい場合は、変換する十進数値を入力します: (例) `123.4567`
   * 一致を調べたい場合は、変換する十進数値と一致を調べる2進数値を単一のスペースで区切って入力します:
     (例) `123.4567 1111011.0111`
3. 終了する場合は、EOF（Linuxの場合はCtrl+D、Windowsの場合はCtrl+Zに続いてEnter）を入力します。

## 変換の出力
変換された2進数値に循環節が存在する場合、`(`, `)`で囲まれて出力されます。

## 一致判定の出力
次の条件をすべて満たした場合、一致と判定されます。
* 整数部: 変換された2進数値が入力された2進数値と**完全に等しい**
* 小数部: 変換された2進数値が入力された2進数値で**始まっている**

さらに、一致判定モードでは、入力された2進数値の小数部の桁数も表示されます。

# コンパイル方法
## VC++
```
; Visual Studio 開発者コマンドプロンプトより
> cl /EHsc src\dec2bin.cpp
```
## GCC/G++
```sh
$ g++ -std=c++11 src/dec2bin.cpp
```

# dec2bin
This program has following features:
* Converting a decimal value to a binary value.
* Matching a converted binary value with an inputed binary value.

# Usage
1. Run this program. The evaluation loop has been started.
2. Input the command based on what you want to do:
   * If you want to convert a decimal value to binary value, input a decimal
     value e.g. `123.4567`
   * If you want to match a converted binary value with an input binary value,
     input a decimal value to be converted and a binary value to be match with,
     separated with a single space e.g. `123.4567 1111011.0111`
3. If you want to quit, input EOF (Ctrl+D in Linux, Ctrl+Z followed by Enter in
   Windows).

## Conversion Output
If the converted binary value has a repating decimal section, the section is
surrounded with parentheses.

## Matching Output
This program consider two values as matched only if all the following conditions are satisfied.
* Integer part: converted binary value **exactly equals to** input binary value 
* Fraction part: converted binary value **starts with** input binary value

Also, in matching mode, this program shows the length of the fraction part of
the input binary value.

# How to Compile
## VC++
```
; In Visual Studio Developer Command Prompt
> cl /EHsc src\dec2bin.cpp
```
## GCC/G++
```sh
$ g++ -std=c++11 src/dec2bin.cpp
```