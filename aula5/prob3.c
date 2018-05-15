#include <detpic32.h>

int main(void){
	//volatile int aux;
	//Configure A/D module
	TRISBbits.TRISB4=1;	//RB4 digital output disconnected
	AD1PCFGbits.PCFG4=0;	//RB4 configured as analog input (AN4)
	AD1CON1bits.SSRC=7;	//COnversion trigger selection bits
	AD1CON1bits.CLRASAM=1;	//Stop conversions when the 1st A/D converter interrupt is generated. At the same time, hardware clears the ASAM bit.
	AD1CON3bits.SAMC=16;	//Sample time is 16 TAD (TAD=100ns)
	AD1CON2bits.SMPI=4-1;	//Interrupt is generated after XX samples (replace XX by the desired number of consecutive samples)
	AD1CHSbits.CH0SA=4;	//replace x by the desired input analog channel (o to 15)
	AD1CON1bits.ON=1;	//Enable A/D converter. This must be the last command of the A/D configuration sequence
	//configure RB6 as a digital output port
	while(1){
		AD1CON1bits.ASAM=1;		//start conversion	
		while(IFS1bits.AD1IF==0);	//wait while conversion not done (AD1IF==0)
		int *p=(int *)(&ADC1BUF0);
		int count=0;
		int sum=0;
		for(; p<= (int *)(&ADC1BUF3); p+=4){
			sum+=*p;
			count++;
			//printInt(*p,16 | 3 <<16);
			//putChar('\n');
		}
		sum=sum/count,10;
		sum=(sum*33+511)/1023;
		printInt(sum,10);
		putChar('\n');
		IFS1bits.AD1IF=0;		//Reset AD1IF;
	}
}
