void set PWM(unsigned int dutyCycle){
	// duty_cycle must be in the range [0,100]
	OC1RS= (PR1*dutyCycle)/100;
}
