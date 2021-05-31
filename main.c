/**************************************************************
long timer　for PIC10F222
1日1回程度のタイマー（繰り返し）
・周期はDIPSWで設定。1=4時間、6=24時間、0=10分、7=7日間
・ON時間は1分
・ｳｵｯﾁﾄﾞｸﾞタイマーで起動する
 **************************************************************
	author yama
	Tools:　MPLAB v8.76、Hitech PIC C pro9.82
---------------------------------------------------------------
Pin assignment 10F222
	1::NC
	2:pickit2-2:VDD(+)
	3:         :GP2:O:LED負論理（出力兼用）
	4:pickit2-5:GP1:I:dipsw1なくてもよい
	5:pickit2-4:GP0:I:dipsw0なくてもよい
	6::NC
	7:pickit2-3:VSS:GND
	8:pickit2-1:MCLR/GP3:I:dipsw2   なくてもよい
**************************************************************/
#include <htc.h>
#define LED GP2
#define SW0 GP0
#define SW1 GP1
#define SW2 GP3

__CONFIG(0x4);	// ｳｵｯﾁﾄﾞｸﾞオン
/*_CONFIG
 bit 4 MCLRE: 1= MCLR        　　0= digital I/O
 bit 3 CP:    1= コード保護オフ　0=オン
 bit 2 WDTE:  1= ｳｵｯﾁﾄﾞｸﾞ　オン　0=オフ
 bit 1 MCLR:　1=プルアップなし　 0=プルアップあり
 bit 0 ｸﾛｯｸ:  1= 8 MHz           0= 4 MHz
*/
#define MHz 000000
#define _XTAL_FREQ 4MHz		//　delay()用

unsigned int loop1,loop2;
unsigned int tim;
void main(void){
	if( (STATUS & 0x10)!=0 ){	//power on
		ANS1 = ANS0 = 0;		//digital por
		OPTION   = 0b10111111;	//ﾌﾟﾙｱｯﾌﾟ有効、WDT周期は2.4sec
	/*	bit7 1=pin割り込み無効
　　 		bit6 1=pinプルアップ無効
　　 		bit5 1=タイマークロックは外部pin
　　 		bit4 1=タイマークロックは立ち下がり
　　 		bit3 1=タイマープリスケーラはWDT、0=タイマー
　　 		bit2-0 分周期
	*/			//    bit3..0: 1=入力　0=出力
    	TRISGPIO = 0b11111011;	// gp2だけ出力
		//for(tim=10;tim;tim--){	// helloチカチカ
		//	LED=tim & 1;
		//	__delay_ms(100);//0.1s
		//	}
		loop1=loop2=0;
		if(SW0)tim=1;else tim=0;
		if(SW1)tim+=2;
		if(SW2)tim+=4;
		if(tim==7)
			tim=7*6;	// 7day
		goto wakeup;
		}
	else{		//ｳｵｯﾁﾄﾞｸﾞタイマーで起動する
		loop1++;	//2.4sec
		if( (tim==0) && (loop1==250) ){  // 10min*(60/2.4=25)
			loop1=0;
				goto wakeup;
			}
		else if(loop1==6000){  // 4hour*60min*(60/2.4=25)
			loop1=0;loop2++;
			if( loop2==tim){
				loop2=0;
				goto wakeup;
				}	
			}
		goto reSleep;
		}
wakeup:
	LED=0;
	for(loop1=60; loop1; loop1--)	// 60sec ON
		__delay_ms(1000);
	LED=1;
reSleep:
	SLEEP();
	}
