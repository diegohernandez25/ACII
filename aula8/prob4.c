#include <detpic32.h>

void delay(int);
void putc(char);
void configUart(unsigned int, char, unsigned int);
void putS(char*);

int main(void){
	configUart(115200,'N',1);
	while(1){
		putS("String de teste\n");
		delay(1000); 	//wait 1s
	}
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
