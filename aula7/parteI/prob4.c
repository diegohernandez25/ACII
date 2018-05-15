#include <detpic32.h>

void main(void){
	T1CONbits.TCKPS=3; 	//1:256 prescaler
	PR1= 39061;		//Fout
	TMR1=0;			//Reset timer T1 count register
	
	T3CONbits.TCKPS=5; 	//1:32 prescaler
	PR3= 62499;		//Fout
	TMR3=0;			//Reset timer T3 count register
	
	T3CONbits.TON=1;	//Enable timer T3 (must be the last comand of the timer configuration sequence
	T1CONbits.TON=1;	//Enable timer T1 (must be the last comand of the timer configuration sequence


	IPC3bits.T3IP = 2;	//Interrupt Priority
	IEC0bits.T3IE = 1;	//Enable timer T3 interrupts
	IFS0bits.T3IF = 0;	//Reset timer T3 interrupt flag;
	
	IPC1bits.T1IP = 3;	//Interrupt Priority
	IEC0bits.T1IE = 1;	//Enable timer T3 interrupts
	IFS0bits.T1IF = 0;	//Reset timer T3 interrupt flag;


	EnableInterrupts();
	while(1);
}

void _int_(12) isr_T3(void){
	putChar('3');
	IFS0bits.T3IF=0;
}
void _int_(4) isr_T1(void){
	putChar('1');
	IFS0bits.T1IF=0;
}

