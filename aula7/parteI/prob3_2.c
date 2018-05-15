#include <detpic32.h>

void main(void){
	T3CONbits.TCKPS=7; 	//1:256 prescaler
	PR3= 39061;		//Fout
	TMR3=0;			//Reset timer T3 count register
	T3CONbits.TON=1;	//Enable timer T3 (must be the last comand of the timer configuration sequence

	IPC3bits.T3IP = 2;	//Interrupt Priority
	IEC0bits.T3IE = 1;	//Enable timer T3 interrupts
	IFS0bits.T3IF = 0;	//Reset timer T3 interrupt flag;

	EnableInterrupts();
	while(1);
}

void _int_(12) isr_T3(void){
	if(TMR3==0){
		TMR3=1;
	}
	else{
		putChar('.');
		IFS0bits.T3IF=0;	//Reset T3 interrupt flag
		TMR3=0;
	}
}

