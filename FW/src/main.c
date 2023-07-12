#include <stdint.h>
#include "fw_reg_stc8g.h"

// STC8G1K08A-36I-DFN8
// Grove: RXD(P3.0)-TXD(P3.1)-VDD-GND / RXD for control (1=on / 0=off)

// 起動時の通電→PWM駆動のパラメータ。時間の単位は[0.005ms]
#define T_ON   40000 // 起動時の通電時間 (0.005ms * 40000 = 200ms)
#define TON_0  1    // PWM駆動時の"1"の時間 (例：0.005ms * 1 = 0.005ms)
#define TOFF_0 9    // PWM駆動時の"0"の時間 (例：0.005ms * 9 = 0.045ms)
// 備考
// - PWM駆動の周期は "TON_0 + TOFF_0"で決まる（この例では0.05ms * (1+9) = 0.05ms、つまり周波数は20kHz）
// - PWM駆動のデューティー比は "TON_0 / (TON_0 + TOFF_0)"で決まる（この例では、1 / (1+9) = 10%）

// コンパイラのインストール
// - Mac・Linux: こちらの"sdcc"と"binutils"のインストール手順: https://github.com/creeper0723/Ksylitoli
// - Windows: http://sdcc.sourceforge.net/ からインストール（未検証）

//-----------------------
//以下は編集不要

#define pinLOAD P55 // P5.4 / CCP2_3
#define pinMD0  P32 // P3.2
#define pinMD1  P33 // P3.3
#define pinRXD  P30 // P3.0 / Grove-p1
#define pinTXD  P31 // P3.1 / Grove-p2


#define TON_1  2
#define TOFF_1 18
#define TON_2  3
#define TOFF_2 27
#define TON_3  4
#define TOFF_3 36

#define FOSC 11059200UL

volatile uint16_t cnt = 0;
volatile uint8_t f = 0;
uint8_t Ton, Toff;

// 200kHz (5us)
void Timer0_ISR(void) __interrupt 1 // Timer0 interrupt
//void Timer0_ISR(void) __interrupt // Timer0 interrupt
{
  f = 1;
}

// wait for [x 5us]
void delay_5us(uint16_t t)
{
  cnt = 0;
  TR0 = 1;
  while(t-- > 0){
    f = 0; while(f == 0);
  }
  TR0 = 0;
}

void main()
{
  uint32_t Ton, Toff;
  P3M1 = 0x00; P3M0 = 0x00; // set P3.0-3.3 as as quasi-bidirectional
  P5M1 = 0x00; P5M0 = 0x20; // set P5.5 as push-pull output

  TMOD = 0x02; // set Timer0 as 8bit timing mode (counting-down, TL0=counter, TH0=reload value)
  AUXR |= 0x80; // T0x12=1 / SYSclk/12 = 11.06MHz
  TH0 = 255 - 56; // Timer0 interrupt = 197.5kHz (11.06MHz / 56)
  TL0 = TH0;
  EA = 1;
  ET0 = 1;
  TR0 = 1;
  Ton = TON_0;   // default PWM value
  Toff = TOFF_0; //  default PWM value
  pinLOAD = 0;
  
  while(1) {
    P3 |= 0x0f; // enable P3.0-3 pull-up
    __asm__("nop");
    __asm__("nop");
    if (pinRXD == 0){
      pinLOAD = 1;
      delay_5us(T_ON); // initial ON
      pinLOAD = 0;
      while(pinRXD == 0){
	P3 |= 0x0f; // enable P3.0-3 pull-up
	__asm__("nop");
	__asm__("nop");
	pinLOAD = 1;
	delay_5us(Ton); // initial ON
	pinLOAD = 0;
	delay_5us(Toff);
      }
    }
  }
}

