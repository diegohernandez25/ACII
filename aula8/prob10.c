#include <detpic32.h>

void putc(char);
void configUart(unsigned int, char, unsigned int);

int main(void){
	configUart(115200,'N',1);
	IEC0bits.U1RXIE=1;	//Enable UART1 Rx Interrupt
	IEC0bits.U1TXIE=0;	//Disable UART1 Tx Interrupt

	U1STAbits.URXISEL=00;	//Receive Interrupt Mode Selection bit
	IPC6bits.U1IP=3;	//Interrupt priority bits
	IEC0bits.U1EIE=1;	//Ativar deteção de erros por interrupção
	IFS0bits.U1RXIF=0;	// Receive Interrupt Flag status bit
	
	EnableInterrupts();
	while(1);
}

void putc(char byte2send){
	/*O bit UTXBF (Transmit Buffer Full) e um bit de status que quando ativo indica que o FIFO de transmissao esta cheio. E necessario esperar que este bit fique inativo para colocar informacao no FIFO*/
	while(U1STAbits.UTXBF==1);	//wait while UTXBF==1
	U1TXREG=byte2send;		//Copy byte2send to the UxTXREG register
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

void _int_(24) isr_uart1(void){
	putc(U1RXREG);
	IFS0bits.U1RXIF=0;	//Clear UART1 rx interrupt flag
}
