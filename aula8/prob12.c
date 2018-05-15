

#include <detpic32.h>

unsigned char toBcd(unsigned char);
unsigned int voltMin=33, voltMax=0;

void delay(int);
void send2displays(unsigned char);
void configureAll(void);
void putc(char);
void putS(char *);
void configUart(unsigned int, char, unsigned int);
char getc(void);
void setPWM(unsigned int);

volatile int voltage;
volatile int count;

void main(void){
	int dutyCycle;
	int portVal;
	configureAll();
	
	//Interrupt flags
	IFS1bits.AD1IF=0;	
	IFS0bits.T1IF=0;	
	IFS0bits.T3IF=0;	
	
	EnableInterrupts();		//Global Interrupt Enable
	while(1){
		portVal=(PORTB & 0x0003);
		switch(portVal){
			case 0: //Measure input voltage
				IEC0bits.T1IE=1;	//Enable T1 interrupts
				setPWM(0);		//LED OFF
				break;
			case 1: //Freeze
				IEC0bits.T1IE=0; 	//Disable T1 interrupts
				setPWM(100);		//LED ON (maximum bright)
				break;
			
			default: //LED brightness control
				IEC0bits.T1IE=1;	//Enable timer T1 interrupts
				dutyCycle = voltage*3;
				setPWM(dutyCycle);
				break;
		}	
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

	OC1CONbits.OCM=6;       //PWM mode on OC1; fault pin disabled
	OC1CONbits.OCTSEL=1;    //Use timer T3 as the time base for PWM generation
	OC1CONbits.ON=1;        //Enable OC1 module
	OC1RS=12500;            //Ton constant


	IPC3bits.T3IP = 2;      //Interrupt Priority
        IEC0bits.T3IE = 1;      //Enable timer T3 interrupts
	IFS0bits.T3IF = 0;      //Reset timer T3 interrupt flag;

	IPC1bits.T1IP = 2;      //Interrupt Priority
	IEC0bits.T1IE = 1;      //Enable timer T1 interrupts
	IFS0bits.T1IF = 0;      //Reset timer T1 interrupt flag;

	TRISDbits.TRISD0=0;
	TRISBbits.TRISB0=1;
	TRISBbits.TRISB1=1;
	
	//UART

	configUart(115200,'N',1);
	U1STAbits.URXISEL=00;	//Receive Interrupt Mode Selection bit
	IEC0bits.U1RXIE=1;	//Enable UART1 Tx Interrupt
	IPC6bits.U1IP=3;	//Interrupt priority bits
	IEC0bits.U1EIE=1;	//Enable Error detection 
	IFS0bits.U1RXIF=0;	// Receive Interrupt Flag status bit
}

void putc(char byte2send){
	while(U1STAbits.UTXBF==1);
	U1TXREG=byte2send;
}

void putS(char *str){
	int i=0;
	while(str[i]!='\0'){
		putc(str[i++]);
	}
}

void configUart(unsigned int baud, char parity, unsigned int stopbits){
	U1MODEbits.BRGH=0;			//divisor de 16
	if(baud>=600 && baud<=115200){
		U1BRG=((20000000+8*baud)/(16*baud))-1;
	}
	else{
		U1BRG=10;
	}
	switch(parity){
		case 'E':
		case 'e':
			U1MODEbits.PDSEL0=1;	//sem paridade e 8 data bits
			U1MODEbits.PDSEL1=0;	//sem paridade e 8 data bits
			break;
		case 'O':
		case 'o':
			U1MODEbits.PDSEL0=0;	//sem paridade e 8 data bits
			U1MODEbits.PDSEL1=1;	//sem paridade e 8 data bits
			break;
		case 'N':
		case 'n':
		default:
			U1MODEbits.PDSEL0=0;	//sem paridade e 8 data bits
			U1MODEbits.PDSEL1=0;	//sem paridade e 8 data bits
			break;
	}
	if(stopbits==2){
		U1MODEbits.STSEL=1;
	}
	else{
		U1MODEbits.STSEL=0;		//1 stop bit
	}
	U1STAbits.UTXEN=1;	//Ativar os modulos de transmissao
	U1STAbits.URXEN=1;	//Ativar os modulos de rececao
	U1MODEbits.ON=1;	//Ativar a UART
	
}

char getc(void){
	int dummy;
	if(U1STAbits.OERR==1){
		U1STAbits.OERR=0;
	}	
	while(U1STAbits.URXDA==0);
	if(U1STAbits.FERR==1 || U1STAbits.PERR==1){
		dummy=U1RXREG;
	}
	else{
		return U1RXREG;
	}
	return 0;
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

void _int_(24) isr_uart1(void){
	int dummy;
	if(IFS0bits.U1EIF==1){
	
		if(U1STAbits.OERR==1){		//if overrun error...
			U1STAbits.OERR==0;	
		}
		else{
			dummy=U1RXREG;
		}
		IFS0bits.U1EIF=0;	//clear UART1 error interrupt flag
	}	
	if(U1RXREG =='l' || U1RXREG =='L'){
		
		putS("\nvoltMax: ");
		printInt10(voltMax);
		putS("\nvoltMin: ");
		printInt10(voltMin);
		putS("\n");

	}

	IFS0bits.U1RXIF=0;//Clear UART1 rx interrupt flag
}

void _int_(4) isr_T1(void){

	AD1CON1bits.ASAM=1;	//Start conversion
	IFS0bits.T1IF=0;	//Reset T1IF flag

}

void _int_(12) isr_T3(void){
	
	static int counter=0;
	
	send2displays(voltage);
	
	if(++counter==100){
		//putS((char*) voltage);	
		counter=0;
		U1RXREG=voltage;	//send voltage to the serial port UART1
	
	}

	IFS0bits.T3IF=0;		//Clear T3 interrupt flag
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
	
	if(voltage>voltMax){
		voltMax=voltage;
	}

	if(voltage<voltMin){
		voltMin=voltage;
	}

	IFS1bits.AD1IF=0;		//Reset AD1IF flag

}

