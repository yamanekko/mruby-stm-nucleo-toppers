# STMicroelectronics NucleoシリーズでTOPPERSを動かすためのmrbgem

## TOPPERS
* ASP3

## 対応ボード
* Nucleo F767zi

## 備考

Taskクラスのdly_tskなどTOPPERS APIを呼び出すメソッドの時間指定はミリ秒。
Clockクラスはマイクロミリ秒が返ってくるので注意してください。

mrubyでマイクロミリ秒単位でのタスクの操作は期待できないため、数字を大きくしないためにもミリ秒指定としている。
Clockクラスは時間の計測に使用することが想定されるため、マイクロミリ秒とした。
ただし、mrubyのオーバーヘッドがあることは注意してください。
