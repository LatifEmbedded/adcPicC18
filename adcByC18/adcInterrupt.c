#include <p18f452.h>
#pragma config WDT = OFF

void acquisitionTime(void);

#pragma interrupt myFunction				// INTERRUPT SERVICE ROUTINE 
void myFunction(void)
{	
	if(PIR1bits.ADIF == 1)					// CHECK IF ADC INTERRUPTS THE CPU
	{
		unsigned int result = 0;
		PIR1bits.ADIF = 0;
		result = ADRESH;
		result <<= 8;
		result += ADRESL;
		PORTC = (result*9)/1023;	
		acquisitionTime();					// ACQUIZITION TIME
		ADCON0bits.GO = 1;					// START CONVERSION
	}
}

#pragma code myIvct = 0x00008				// HIGH PRIORITY INTERRUPT
void myIvct(void)
{
	_asm
		GOTO myFunction
	_endasm
}	
#pragma code								// RETURN TO DEFAULT SECTION CODE

void main(void)
{
	TRISAbits.TRISA0 = 1;	
	TRISC = 0xF0;
	ADCON0 = 0x41;
	ADCON1 = 0x8E;
	INTCONbits.GIE = 1;
	INTCONbits.PEIE = 1;
	PIR1bits.ADIF = 0;
	PIE1bits.ADIE = 1;
	acquisitionTime();
	ADCON0bits.GO = 1;
	while(1);
}

void acquisitionTime(void)
{
	T0CON = 0x48;
	TMR0L = 0xEB;
	INTCONbits.TMR0IF = 0;
	T0CONbits.TMR0ON = 1;
	while(INTCONbits.TMR0IF == 0);
	INTCONbits.TMR0IF = 0;
	T0CONbits.TMR0ON = 0;	
}