/*
 * BAM_m1284p_test.cpp
 *
 * Created: 19-Mar-24 8:50:12 PM
 * Author : sadeed
 */ 
#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

 const int array_us[] = {0,10,20,40,80,160,320,640,1280};
// const int array_us[] = {0,1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8124};
const int array_Counter[] = {20,40,80,160,320,640,1280,2560,5120};
		

volatile int Luma = 0;

 int n1 =0; // to double count brightness update
volatile int i = 0;

volatile unsigned char *PortLetter;
unsigned char PortBit;

volatile uint8_t BAM_Flag = 0;
 
void delay_us(uint16_t n) {

	while (n--)
	{
		//_delay_us(0.1);
		asm volatile (
		"    rjmp 1f	\n"
		"1:	\n"
		);
	}
}

void BAM(int Luma, volatile unsigned char *PortLetter, unsigned char PortBit)
{
		int Luma_Buff = Luma;
		int Fbit = 0;
		int Pulse = 0;

	for (int n = 0; n<9; n++)
	{
		Fbit = Luma_Buff & 0x01;
		Luma_Buff = Luma>>n;
		Pulse = array_us[n];
		
		if (Fbit==1)
		{
			*PortLetter |= (1<<PortBit);
			delay_us(Pulse);
		}
		else
		{
			*PortLetter &= ~(1<<PortBit);
			delay_us(Pulse);
		}
	 }
	 
}

void Timer3_init()
{
	TCCR3B |= 1<<WGM32 | 1<<CS30;
	
	TIMSK3 |= 1<<OCIE3A;
}

void Set_BAM(int SLuma, volatile unsigned char *SPortLetter, unsigned char SPortBit)
{
	Luma = SLuma;
	PortLetter = SPortLetter;
	PortBit = SPortBit;
}

int main(void)
{
    DDRA =0xff;
	
	Timer3_init();
		TCNT3 = 0;
		// Luma: 0 to 255
		Luma = 10; // set brightness using this variable //////////////////////////////////////////////////////////////////////////////////
		sei();
		OCR3A = 5100;

		Set_BAM(Luma,&PORTA,PINA0); // for setting it for first time
		
		
//		BAM(Luma,PortLetter,PortBit); // to call func. and set

    while (1) 
    {
// dont use _delay() function, it wont work, instead use interrupt based timer to increment Luma value
		if (BAM_Flag == 1)
		{
			BAM(Luma,&PORTA,PINA0);
			BAM_Flag = 0;
		}
    }
}

// second technique, put if statement in bam func. when interrupt fires

ISR(TIMER3_COMPA_vect)
{
	BAM_Flag = 1;
}



