#include <detpic32.h>
void delay(int ms){
	for(;ms>0;ms--){
		resetCoreTimer();
		while(readCoreTimer()<20000);
	}		
}
void send2displays(unsigned char value){
	unsigned char display7Scodes[]={0x3F, 0x06, 0x5B,0X4F, 0x66,0x6D, 0x7D,0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79,0x71};
	unsigned int hd;
	unsigned int ld;
	unsigned int index;
	//send digit_high to display_high
	LATDbits.LATD5=0;
	LATDbits.LATD6=1;
	//index=((value & 0xFF00)>>8);
	index=value/10;
	hd=(display7Scodes[index])<<8;
	LATB=((LATB & 0x00FF)|hd);
	delay(200);
	//send digit_high to display_high
	LATDbits.LATD5=1;
	LATDbits.LATD6=0;
	//index=(value & 0x00FF);
	index=value%10;
	ld=(display7Scodes[index])<<8;
	LATB=((LATB & 0x00FF)|ld);

}

void main(void){
	unsigned char counter=0x0001;
	LATB=(LATB & 0x00FF);
	LATDbits.LATD5=1;	//display low active
	LATDbits.LATD6=0;	//display high inactive
	TRISB=(TRISB & 0X00FF); //RB8-RB14 as outputs
	TRISDbits.TRISD5=0;	//RD5 output
	TRISDbits.TRISD6=0;	//RD6 output
	while(1){
		counter=counter%100;			
		send2displays(counter);
		counter++;
		delay(200);

	}
}

