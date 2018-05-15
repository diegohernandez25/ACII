#include <detpic32.h>

void setPWM( unsigned int);
void send2displays(unsigned char value);
void delay(int);
unsigned char toBcd(unsigned char);
void configureAll(void);

volatile int voltage;

void main(void){
	int dutyCycle;
	int portVal;
	configureAll();

	EnableInterrupts();

	while(1){
		portVal=(PORTB & 0X0003);		
		switch(portVal){	
			case 0:	//Measure input voltage
				IEC0bits.T1IE = 1;      //Enble timer T1 interrupts
				setPWM(0);		//LED OFF
				break;
			
			case 1: //Freeze
				//congela o valor atual da tensao 
				IEC0bits.T1IE = 0;      //Disable timer T1 interrupts
				setPWM(100);		//LED ON (maximum bright)
				break;
			
			default://LED brightness control
				//brilho do LED depende do valor da tensão medido pelo sistema
				IEC0bits.T1IE = 1;      //Enble timer T1 interrupts
				dutyCycle= voltage *3;
				setPWM(dutyCycle);
				break;
		}
	}
}

void configureAll(void){
	
	//Configure all (digital I/0, analog input, A/D module)
	TRISBbits.TRISB4=1;	//digital output disabled
	AD1PCFGbits.PCFG4=0;	//analog input
	AD1CON1bits.SSRC=7;	//conversion trigger selection bits
	AD1CON1bits.CLRASAM=1;	//stop conversion when the 1st a/d converter interrupt is generated
	AD1CON3bits.SAMC=16;	//sample time
	AD1CON2bits.SMPI=8-1;	//number of samples
	AD1CHSbits.CH0SA=4;	//analog channel
	AD1CON1bits.ON=1;	//enable A/D converter

	LATB=(LATB & 0x00FF);
	LATDbits.LATD5=1;	//display low active
	LATDbits.LATD6=0;	//display high inactive
	TRISB=(TRISB & 0X00FF); //RB8-RB14 as outputs
	TRISDbits.TRISD5=0;	//RD5 output
	TRISDbits.TRISD6=0;	//RD6 output

	TRISBbits.TRISB6=0;
	//Configure interrupt system
	IPC6bits.AD1IP=2;		//configure priority of A/D interrupts
	IEC1bits.AD1IE=1;		//enable A/D interrupts
	IFS1bits.AD1IF=0;		//clear A/D interrupt flag; Reset AD1IF flag
	AD1CON1bits.ASAM=1;		//start conversion

	
	T1CONbits.TCKPS=3;		//1:256
	PR1=19530;
	TMR1=0;
        T1CONbits.TON=1;        //Enable timer T1 (must be the last comand of the timer configuration sequence
	
	T3CONbits.TCKPS=2;		//1:4
	PR3=49999;
	TMR3=0;
	T3CONbits.TON=1;        //Enable timer T3 (must be the last comand of the timer configuration sequence

	OC1CONbits.OCM=6;	//PWM mode on OC1; fault pin disabled
	OC1CONbits.OCTSEL=1;	//Use timer T3 as the time base for PWM generation
	OC1CONbits.ON=1;	//Enable OC1 module
	OC1RS=12500;

	IPC3bits.T3IP = 2;      //Interrupt Priority
        IEC0bits.T3IE = 1;      //Enable timer T3 interrupts
	IFS0bits.T3IF = 0;      //Reset timer T3 interrupt flag;

	IPC1bits.T1IP = 2;      //Interrupt Priority
	IEC0bits.T1IE = 1;      //Enable timer T1 interrupts
	IFS0bits.T1IF = 0;      //Reset timer T1 interrupt flag;

	TRISDbits.TRISD0=0;
	TRISBbits.TRISB0=1;
	TRISBbits.TRISB1=1;
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
	displayFlag=!displayFlag;

}


void _int_(4) isr_T1(void){

	AD1CON1bits.ASAM=1;	//Start conversion
	IFS0bits.T1IF=0;	//Reset T1IF flag
}

void _int_(12) isr_T3(void){
	// Send "voltage" global variable to displays
	// Reset T3IF flag
	send2displays(voltage);
	IFS0bits.T3IF=0;	//Reset T3IF flag
}

void _int_(27) isr_adc(void){
	int *p=(int *)(&ADC1BUF0);
	int sum=0;
	int i=0;
	for(i=0;i<8;i++){
		sum+=p[i*4];
	}	
	sum=sum/8;
	voltage=toBcd((sum*33)/1023);
	IFS1bits.AD1IF=0;		//Reset AD1IF flag
}
