#include <detpic32.h>
void main(void){
	
	//Configure all (digital I/0, analog input, A/D module)
	TRISBbits.TRISB4=1;	//digital output disabled
	AD1PCFGbits.PCFG4=0;	//analog input
	AD1CON1bits.SSRC=7;	//conversion trigger selection bits
	AD1CON1bits.CLRASAM=1;	//stop conversion when the 1st a/d converter interrupt is generated
	AD1CON3bits.SAMC=16;	//sample time
	AD1CON2bits.SMPI=1-1;	//number of samples
	AD1CHSbits.CH0SA=4;	//analog channel
	AD1CON1bits.ON=1;	//enable A/D converter

	//Configure interrupt system
	IPC6bits.AD1IP=2;		//configure priority of A/D interrupts
	IEC1bits.AD1IE=1;		//enable A/D interrupts
	IFS1bits.AD1IF=0;		//clear A/D interrupt flag; Reset AD1IF flag
	EnableInterrupts();		//global interrupt enable
	AD1CON1bits.ASAM=1;		//start conversion
	//while(IFS1bits.AD1IF==0);	//wait while conversion not done
	//int *p=(int *)(&ADC1BUF0);	//read conversion result
	//for(i=0;i<4;i++){
		//sum+=p[i*4]
	//}
	while(1){}
}
// Interrupt Handler
void _int_(27) isr_adc(void){		//ir a datasheet	
	printInt(ADC1BUF0, 16 | 3<<16);	//print ADC1BUF0 value
	putChar('\n');		
	AD1CON1bits.ASAM=1;		//start conversion
	IFS1bits.AD1IF=0;		//reset AD1IF flag
}
