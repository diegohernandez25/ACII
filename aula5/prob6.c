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
	TRISBbits.TRISB4=1;	//digital output disabled
	AD1PCFGbits.PCFG4=0;	//analog input
	AD1CON1bits.SSRC=7;	//conversion trigger selection bits
	AD1CON1bits.CLRASAM=1;	//stop conversion when the 1st a/d converter interrupt is generated
	AD1CON3bits.SAMC=16;	//sample time
	AD1CON2bits.SMPI=4-1;	//number of samples
	AD1CHSbits.CH0SA=4;	//analog channel
	AD1CON1bits.ON=1;	//enable A/D converter


	LATB=(LATB & 0x00FF);
	LATDbits.LATD5=1;	//display low active
	LATDbits.LATD6=0;	//display high inactive
	TRISB=(TRISB & 0X00FF); //RB8-RB14 as outputs
	TRISDbits.TRISD5=0;	//RD5 output
	TRISDbits.TRISD6=0;	//RD6 output
	int sum=0;
	int i=0;
	int vol=0;
	while(1){
		sum=0;
		delay(10);
		if(++i==25){				//250ms
			AD1CON1bits.ASAM=1;		//start conversion
			while(IFS1bits.AD1IF==0);	//wait while conversion not done
			int *p=(int *) (&ADC1BUF0);	//read conversion result
			for(i=0; i<4;i++){
				sum+=p[i*4];
					
			}
			sum=sum/4;		//calculate voltage average
			vol=(sum*33)/1023;	//calculate voltage amplitude to decimal
			i=0;	
		}
		send2displays(toBcd(vol));
	}
}

