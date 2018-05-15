#include <detpic32.h>
void delay(int ms){
	for(;ms>0;ms--){
		resetCoreTimer();
		while(readCoreTimer()<20000);
	}		
}
void main(void){
	LATE=(LATE & 0xFFF0);
	TRISE=(TRISE & 0xFFF0);
	while(1){
		LATE+=1;
		delay(250);
	}
}
