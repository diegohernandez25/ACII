#include <detpic32.h>

void main(void){
	LATB=(LATB & 0X00FF);
	LATDbits.LATD5=0;
	LATDbits.LATD6=1;
	TRISB=(TRISB & 0X00FF);
	TRISDbits.TRISD5=0;
	TRISDbits.TRISD6=0;
	while(1){
		char c=getChar();
		LATB=(LATB & 0X00FF);
		switch(c){
			case 'A':
			case 'a':
					LATBbits.LATB8=1;					
					break;
			case 'B':
			case 'b':
					LATBbits.LATB9=1;					
					break;
			case 'C':
			case 'c':
					LATBbits.LATB10=1;					
					break;
			case 'D':
			case 'd':
					LATBbits.LATB11=1;					
					break;
			case 'E':
			case 'e':
					LATBbits.LATB12=1;					
					break;
			case 'F':
			case 'f':
					LATBbits.LATB13=1;					
					break;
			case 'G':
			case 'g':
					LATBbits.LATB14=1;					
					break;
			case '.':
					LATBbits.LATB15=1;					
					break;
			default:
					break;
		}	
	}
	

}
