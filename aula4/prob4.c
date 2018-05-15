#include <detpic32.h>

void delay(int ms){
	for(;ms>0;ms--){
		while(readCoreTimer()<20000);
		resetCoreTimer();
	}	
}
void main(void){
	int i;
	unsigned char segment;
	LATB=(LATB & 0X00FF);
	LATDbits.LATD5=0;	//display low inactive
	LATDbits.LATD6=1;	//display high active
	TRISB=(TRISB & 0X00FF); //RB8-RB14 as outputs
	TRISDbits.TRISD5=0;	//RD5 output
	TRISDbits.TRISD6=0;	//RD6 output
	while(1){
		LATDbits.LATD6=!LATDbits.LATD6;
		LATDbits.LATD5=!LATDbits.LATD5;
		segment=1;
		for(i=0;i<7;i++){
			LATB=LATB & 0x00FF;
			LATB=LATB|(segment<<8);
			delay(1000);
			segment= segment<<1;		
		}
	}
}
