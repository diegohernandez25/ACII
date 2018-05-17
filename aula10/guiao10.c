#include <detpic32.h>
#include "i2c.h"

void configureAll(void);
unsigned char toBcd(unsigned char);
void send2displays(unsigned char);

int getTemperature(int *temperature){
	int ack;
	i2c1_init(TC74_CLK_FREQ);
	i2c1_start();
	ack=i2c1_send(ADDR_WR);
	ack+=i2c1_send(RTR);
	i2c1_start();
	ack+=i2c1_send(ADDR_RD);
	*temperature=i2c1_receive(I2C_NACK);
	i2c1_stop();
	return ack;
}

int main(void){
	configureAll();
	int temperature;
	int ack;
	while(1){
		ack=getTemperature(&temperature);
		if(ack!=0){
			printStr("Error!\n");
			i2c1_stop();
			break; 
		}
		printStr("Temperature: ");
		printInt10(temperature);
		putChar('\n');
		if((PORTB & 0x0003)==0x0003){
			int i;
			for(i=0; i<10;i++){
				send2displays(toBcd(temperature));
				delay(10);
			}
		}
		delay(100);
	}
	return 0;
}

void configureAll(void){
	LATB=(LATB & 0x00FF);
	LATDbits.LATD5=1;
	LATDbits.LATD6=0;
	TRISB=(TRISB & 0x00FF);
	TRISDbits.TRISD5=0;
	TRISDbits.TRISD6=0;
	TRISBbits.TRISB6=0;

	TRISBbits.TRISB0=1;
	TRISBbits.TRISB1=1;
}


unsigned char toBcd(unsigned char value){
	return ((value/10)<<4)+(value%10);
}

void send2displays(unsigned char value){
	unsigned char display7Scodes[]={0x3F, 0x06, 0x5B,0X4F, 0x66,0x6D, 0x7D,0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79,0x71};
	unsigned int hd;
	unsigned int ld;
	unsigned int index;
	int pair=value%2;
	static char displayFlag=0;

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
