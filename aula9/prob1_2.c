#include <detpic32.h>


/*Escreva, as macros de ativacao e desativacao das interrupcoes de rececao e de transmissao da UART1*/

#define DisableUart1RxInterrupt()	IEC0bits.U1RXIE=0
#define EnableUart1RxInterrupt()	IEC0bits.U1RXIE=1
#define DisableUart1TxInterrupt()	IEC0bits.U1TXIE=0
#define EnableUart1TxInterrupt()	IEC0bits.U1TXIE=1

/*Defina a constance BUF_SIZE com valor 8*/

#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE - 1)
#define FALSE 0
#define TRUE 1
/*Implementar um buffer circular e criar instancias: rxb e txb*/

typedef struct{
	unsigned char data[BUF_SIZE];
	unsigned int head;
	unsigned int tail;
	unsigned int count;
}circularBuffer;

/*Transmission buffer*/

volatile circularBuffer txb;	

/*Reception buffer*/

volatile circularBuffer rxb;

void comDrv_flushRx(void);
void comDrv_flushTx(void);
void comDrv_putc(char);
void comDrv_puts(char*);
void comDrv_config(unsigned int, char, unsigned int);
char comDrv_getc(char*);


int main(void){
	char c;
	/*default pterm parameters with TX and RX interrupts disabled*/
	comDrv_config(115200,'N',1);

	comDrv_flushRx();
	comDrv_flushTx();
	EnableInterrupts();
	EnableUart1RxInterrupt();
	comDrv_puts("PIC32 UART Device-driver\n");
	while(1){
		/*Read character from reception buffer*/
		comDrv_getc(&c);
		/*Send character to the transmission buffer*/
		comDrv_putc(c);
		if(c=='s'){
			comDrv_puts("Diego Caldeira Hernandez n_mec:77013");
		}
		c=0;
	}
	return 0;
}
void comDrv_flushRx(void){
	
	/*Initialize variable of the reception buffer*/
	
	unsigned int i;
	for(i=0;i<BUF_SIZE;i++){
		rxb.data[i]=0;
	}
	rxb.head=0;
	rxb.tail=0;
	rxb.count=0;
	return;
}

void comDrv_flushTx(void){
	
	/*Initialize variable of the transmission buffer*/

	unsigned int i;
	for(i=0;i<BUF_SIZE;i++){
		txb.data[i]=0;
	}
	txb.head=0;
	txb.tail=0;
	txb.count=0;
	return;
}

/* 
 * Funcao escreve um carater no buffer de transmissao e atualiza as variaveis tail e count.
 * Esta funcao devera ainda esperar que haja espaco livre no buffer antes de copiar um novo carater
*/
void comDrv_putc(char ch){

	/*Wait while buffer is full*/
	while(txb.count==BUF_SIZE);
	/*Copy character to the transmission buffer at position "tail"*/
	txb.data[txb.tail]=ch;
	/*Increment "tail" index (mod. BUF_SIZE)*/
	/*retira o digito 10, quando for 1*/
	txb.tail=(txb.tail + 1) & INDEX_MASK;	
	/*Begin Critical Section*/
	DisableUart1TxInterrupt();
	/*Increment "count" variable*/
	txb.count++;
	/*End Critical Section*/
	EnableUart1TxInterrupt();
	return;

}

/*Funcao evoca envio para a linha serie de string. Terminada com o carater '\0'*/
void comDrv_puts(char *s){
	
	int i=0;
	while(s[i]!='\0'){
		comDrv_putc(s[i++]);	
	}
	return;
}

/*Rotina a interrupcao de transmissao e receicao da UART1*/
void _int_(24) isr_uart1(void){

	//if U1TXIF is set
	//UxTXIF: Transmit Inteerupt Flag Status Bit
	if(IFS0bits.U1TXIF==1){
		/*If "count" variable (transmission buffer, txb) is greater than 0*/
		/*The UTXBF is set whenever the buffer is full. If a user attempts to write to a full buffer, the new data will not be accepted into the FIFO*/
		if(txb.count>0){
			/*Copy character pointed by "head" to U1TXREG register*/
			U1TXREG=txb.data[txb.head];
			/*Increment "head" variable (mod BUF_SIZE)*/
			txb.head=(txb.head +1) & INDEX_MASK;
			txb.count--;
		}
		if(txb.count==0){
			DisableUart1TxInterrupt();
		}
		/*Reset UART1 TX interrupt flag*/
		IFS0bits.U1TXIF=0;
	}

	/*If U1RXIF is set*/
	if(IFS0bits.U1RXIF==1){
		/*Read character from UART and write it to the "tail" position of the reception buffer*/
		rxb.data[rxb.tail]=U1RXREG;
		/*Increment "tail" cariable (mod BUF_SIZE)*/
		rxb.tail=(rxb.tail+1) & INDEX_MASK;
		/*If reception buffer is not full then ncrement "count" variable*/
		if(rxb.count<BUF_SIZE){
			rxb.count++;
		}
		else{
			/*Increment "head" variable (discard oldest character)*/
			rxb.head=(rxb.head+1) & INDEX_MASK;
		}
		IFS0bits.U1RXIF=0;
	}
	return;
}

void comDrv_config(unsigned int baud, char parity, unsigned int stopbits){
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
	
	/*Receive Interrupt Mode Selection Bit*/
	U1STAbits.URXISEL=00;
	/*"with TX and RX interrupts disabled"*/
	IEC0bits.U1TXIE=0;
	IEC0bits.U1RXIE=0;
	/*Specify the interrupt piority
	 * Interrupt Priority Bits*/
	IPC6bits.U1IP=3;
	/*Enable Error Detection*/
	IEC0bits.U1EIE=1;
	/*Receive & Transmit Interrupt Flag Status Bit*/
	IFS0bits.U1RXIF=0;
	IFS0bits.U1TXIF=0;
	return;	
}

char comDrv_getc(char *pchar){
	/*Test "count" variable (reception buffer) and return FALSE if it is zero*/
	if(rxb.count==0){
		return FALSE;
	}
	/*Begin Critical Section*/
	DisableUart1RxInterrupt();
	/*Copy "CHARACTER" POINTED by "head" to *pchar*/
	*pchar=rxb.data[rxb.head];
	/*Decrement "count" variable*/
	rxb.count--;
	/*Increment "head" variable (mod BUF_SIZE)*/
	rxb.head=(rxb.head+1) & INDEX_MASK;
	/*End of Critical Section*/
	EnableUart1RxInterrupt();
	return TRUE;
}
