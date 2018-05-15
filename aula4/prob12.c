#include <detpic32.h>

unsigned char toBcd(unsigned char value){
	return ((value/10)<<4)+(value %10);
}
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
	int pair=value%2;
	static char displayFlag=0;	//ariavel nao perde o valor entre chamadas a funcao (static)
	if(displayFlag==0){
		LATDbits.LATD5=0;
		LATDbits.LATD6=1;
		index=(value>>4);
		hd=(display7Scodes[index])<<8;
		LATB=((LATB & 0x00FF)|hd);
		if(pair!=0){
			LATBbits.LATB15=1;
		}
	}
	else{
		LATDbits.LATD5=1;
		LATDbits.LATD6=0;
		index=(value & 0x0F);
		ld=(display7Scodes[index])<<8;
		LATB=((LATB & 0x00FF)|ld);
		if(pair==0){
			LATBbits.LATB15=1;
		}
	}
//	delay(200);
	displayFlag=!displayFlag;
}

void main(void){
	int counter=1;
	int i,j;
	int flagBlink=0;
	LATB=(LATB & 0x00FF);
	LATDbits.LATD5=1;	//display low active
	LATDbits.LATD6=0;	//display high inactive
	TRISB=(TRISB & 0X00FF); //RB8-RB14 as outputs
	TRISDbits.TRISD5=0;	//RD5 output
	TRISDbits.TRISD6=0;	//RD6 output
	while(1){
		counter=counter%60;
		if(counter==0){
			i=0;
			do{
				flagBlink=!flagBlink;
				j=0;
				do{
					delay(5);
					if(flagBlink){
						send2displays(0);
					}
					else{
						LATB=(LATB & 0x00FF);
					}
				}while(++j<100);

			}while(++i<8);
		}
		i=0;
		do{	
			delay(10);
			send2displays(toBcd(counter));
		}while(++i<100);
		counter++;
	}
}


