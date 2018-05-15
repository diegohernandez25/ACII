#include <detpic32.h>

void send2displays(unsigned char value){
	unsigned char display7Scodes[]={0x3F, 0x06, 0x5B,0X4F, 0x55,0x6D, 0x7D,0x07, 0x78, 0x57, 0x77, 0x7C, 0x39, 0x5D, 0x79,0x71};
	unsigned int hd;
	unsigned int ld;
	///ELIMINAR DEPOIS
	LATB=(LATB & 0x00FF);
	LATDbits.LATD5=1;	//display low active
	LATDbits.LATD6=0;	//display high inactive
	TRISB=(TRISB & 0X00FF); //RB8-RB14 as outputs
	TRISDbits.TRISD5=0;	//RD5 output
	TRISDbits.TRISD6=0;	//RD6 output
	/////////////////
	//send digit_high to display_high
	LATDbits.LATD5=0;
	LATDbits.LATD6=1;
	hd=(value & 0xFF00);
	LATB=((LATB & 0x00FF)|hd);
	//send digit_high to display_high
	LATDbits.LATD5=1;
	LATDbits.LATD6=0;
	ld=((value & 0x00FF)<<8);	
	LATB=((LATB & 0x00FF)|ld);

}
