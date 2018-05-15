#include <detpic32.h>

void setPWM(unsigned int);

void main(void){
	TRISDbits.TRISD0=0;	//output	
	T3CONbits.TCKPS=2;	//:4 prescaler (100Hz)
	PR3= 49999;
	TMR3=0;			//Reset timer T3 count register
	T3CONbits.TON=1;	//Enable timer T3 (must be the last command of the timer configuration sequence)
	
	OC1CONbits.OCM=6;	//PWM mode on OC1; fault pin disabled
	OC1CONbits.OCTSEL=1;	//Use timer T3 as the time base for PWM generation
	//OC1CONbitsOCTSEL=0;	//or Use timer T2 as the time base for PWM generation
	setPWM(10);

	OC1CONbits.ON=1;	//Enable OC1 module
	

}

void setPWM(unsigned int dutyCycle){
	//duty cycle must be in range [0,100]
	if(dutyCycle>=0 && dutyCycle<=100){
		//evaluate OC1RS as a function of duty cycle
		OC1RS=((PR1+1)*dutyCycle)/100;
	}
	else{
		OC1RS=0;
	}
}
