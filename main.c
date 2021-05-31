/**************************************************************
long timer�@for PIC10F222
1��1����x�̃^�C�}�[�i�J��Ԃ��j
�E������DIPSW�Őݒ�B1=4���ԁA6=24���ԁA0=10���A7=7����
�EON���Ԃ�1��
�E�����޸ރ^�C�}�[�ŋN������
 **************************************************************
	author yama
	Tools:�@MPLAB v8.76�AHitech PIC C pro9.82
---------------------------------------------------------------
Pin assignment 10F222
	1::NC
	2:pickit2-2:VDD(+)
	3:         :GP2:O:LED���_���i�o�͌��p�j
	4:pickit2-5:GP1:I:dipsw1�Ȃ��Ă��悢
	5:pickit2-4:GP0:I:dipsw0�Ȃ��Ă��悢
	6::NC
	7:pickit2-3:VSS:GND
	8:pickit2-1:MCLR/GP3:I:dipsw2   �Ȃ��Ă��悢
**************************************************************/
#include <htc.h>
#define LED GP2
#define SW0 GP0
#define SW1 GP1
#define SW2 GP3

__CONFIG(0x4);	// �����޸ރI��
/*_CONFIG
 bit 4 MCLRE: 1= MCLR        �@�@0= digital I/O
 bit 3 CP:    1= �R�[�h�ی�I�t�@0=�I��
 bit 2 WDTE:  1= �����޸ށ@�I���@0=�I�t
 bit 1 MCLR:�@1=�v���A�b�v�Ȃ��@ 0=�v���A�b�v����
 bit 0 �ۯ�:  1= 8 MHz           0= 4 MHz
*/
#define MHz 000000
#define _XTAL_FREQ 4MHz		//�@delay()�p

unsigned int loop1,loop2;
unsigned int tim;
void main(void){
	if( (STATUS & 0x10)!=0 ){	//power on
		ANS1 = ANS0 = 0;		//digital por
		OPTION   = 0b10111111;	//��ٱ��ߗL���AWDT������2.4sec
	/*	bit7 1=pin���荞�ݖ���
�@�@ 		bit6 1=pin�v���A�b�v����
�@�@ 		bit5 1=�^�C�}�[�N���b�N�͊O��pin
�@�@ 		bit4 1=�^�C�}�[�N���b�N�͗���������
�@�@ 		bit3 1=�^�C�}�[�v���X�P�[����WDT�A0=�^�C�}�[
�@�@ 		bit2-0 ������
	*/			//    bit3..0: 1=���́@0=�o��
    	TRISGPIO = 0b11111011;	// gp2�����o��
		//for(tim=10;tim;tim--){	// hello�`�J�`�J
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
	else{		//�����޸ރ^�C�}�[�ŋN������
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
