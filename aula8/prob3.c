#include <detpic32.h>

void delay(int);
void putc(char);
void confUART(void);
void putS(char*);

int main(void){
	confUART();
	while(1){
		putS("String de teste\n");
		delay(1000); 	//wait 1s
	}
}

void confUART(){
	U1MODEbits.BRGH=0;	//divisor de 16
	U1BRG=10;		//baudrate
	U1MODEbits.PDSEL0=0;	//sem paridade e 8 data bits
	U1MODEbits.PDSEL1=0;	//sem paridade e 8 data bits
	U1MODEbits.STSEL=0;	//stop bits
	//U1STA=0;		//Is it necessary??
	U1STAbits.UTXEN=1;	//Ativar os modulos de transmissao
	U1STAbits.URXEN=1;	//Ativar os modulos de rececao
	U1MODEbits.ON=1;	//Ativar a UART
}

void delay(int ms){
	for(;ms>0;ms--){
		resetCoreTimer();
		while(readCoreTimer()<20000);
	}
}

void putc(char byte2send){
	while(U1STAbits.UTXBF==1);	//wait while UTXBF==1
	U1TXREG=byte2send;		//Copy byte2send to the UxTXREG register
}

void putS(char *str){
	int i=0;
	while(str[i]!='\0'){
		putc(str[i++]);
	}
}
