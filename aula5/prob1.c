#include <detpic32.h>

void main(void){
	//CONFIGURE O PORTO rb4 como entrada analogica
	TRISBbits.TRISB4=1;	//RB4 digital output disconnected
	AD1PCFGbits.PCFG4=0;	//RB4 configured as analog input (AN4)
	AD1CON1bits.SSRC=7;	//COnversion trigger selection bits
	AD1CON1bits.CLRASAM=1;	//Stop conversions when the 1st A/D converter interrupt is generated. At the same time, hardware clears the ASAM bit.
	AD1CON3bits.SAMC=16;	//Sample time is 16 TAD (TAD=100ns)
	AD1CON2bits.SMPI=1-1;	//Interrupt is generated after XX samples (replace XX by the desired number of consecutive samples)
	AD1CHSbits.CH0SA=4;	//replace x by the desired input analog channel (o to 15)
	AD1CON1bits.ON=1;	//Enable A/D converter. This must be the last command of the A/D configuration sequence
	while(1){
		AD1CON1bits.ASAM=1;		//start conversion
		while(IFS1bits.AD1IF==0);	//Wait while conversion not done
		printInt(ADC1BUF0, 16| 3 <<16);
		putChar('\n');
		IFS1bits.AD1IF=0;//Reset AD1IF
	}
}
