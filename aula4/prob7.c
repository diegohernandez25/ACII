#include <detpic32.h>

void main(void){
	unsigned char segment;
	int index;
	unsigned char display7Scodes[]={0x3F, 0x06, 0x5B,0X4F, 0x66,0x6D, 0x7D,0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79,0x71};
	LATB=(LATB & 0x00FF);
	LATDbits.LATD5=0;	//display low active
	LATDbits.LATD6=1;	//display high inactive
	TRISB=(TRISB & 0X00FF); //RB8-RB14 as outputs
	TRISDbits.TRISD5=0;	//RD5 output
	TRISDbits.TRISD6=0;	//RD6 output
	TRISB=(TRISB | 0x000F);	//configure RB0 to RB3 as inputs
	while(1){
		index=(PORTB & 0x000F);
		segment=display7Scodes[index];		
		LATB=(LATB & 0x00FF)|(segment<<8);
	}
}
