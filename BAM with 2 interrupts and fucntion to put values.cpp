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

void FadeIn_out()
{
//	cli();
	
if (n1>2)
{

		if (i>=1)
		{
			Luma--;
			if (Luma<=1)
			{
				i=0;
			}
			
		}
		
		if (i<=0)
		{
			Luma++;
			if (Luma>=255)
			{
				i=1;
			}
		}
		
		n1 =0;
}
n1++;
	
//	TCNT3 = 0;
//	sei();
}

void delay_us(uint16_t n) {
//	cli();
//	TCNT3 = 0;
	while (n--)
	{
		//_delay_us(0.1);
		asm volatile (
		"    rjmp 1f	\n"
		"1:	\n"
		);
	}
//	TCNT3 = 0;
//	sei();

}

void BAM(int Luma, volatile unsigned char *PortLetter, unsigned char PortBit)
{
//	cli();
		int Luma_Buff = Luma;
		int Fbit = 0;
		int Pulse = 0;
		
		
 // if (BAM_E==1)
//  { 
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
	 
// 	 c++;
// 	 if (c>7)
// 	 {
// 		 c=0;
// 		 
// 	 }

	//	BAM_E = 0;
	//}
	
	
//	sei();
}

void Timer3_init()
{
	TCCR3B |= 1<<WGM32 | 1<<CS30;
	TCCR1B |= 1<<WGM12 | 1<<CS10;
	
	TIMSK3 |= 1<<OCIE3A;
	TIMSK1 |= 1<<OCIE1A;
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
	

	//BAM(11);
	Timer3_init();
//		_delay_ms(500);
		TCNT3 = 0;
		
		
		
		Luma = 255; // set brightness using this variable //////////////////////////////////////////////////////////////////////////////////
		sei();
		OCR3A = 5100;
		OCR1A = 50000; // luma++ every 3 msec. for 60000
		
//		PortLetter = &PORTD;
//		PortBit = PIND1;
		Set_BAM(Luma,&PORTA,PINA0);
//		BAM(Luma,PortLetter,PortBit); // to call func. and set
		unsigned long int n =0;
    while (1) 
    {
//			_delay_us(70);
	//	Luma++;	
		// 	_delay_ms(1000);
// 		while (n>10)
// 		{
// 			n++;
// 		}
// 		n=0;
// 		
// 			PORTD ^=0xff		

		//PORTD = 0xff;
	//	BAM(Luma,&PORTD,PIND1);
    }
}

// second technique, put if statement in bam func. when interrupt fires

ISR(TIMER3_COMPA_vect)
{

//	cli();

//	BAM_E = 1;


//Set_BAM(Luma,&PORTD,PIND1);
	BAM(Luma,PortLetter,PortBit);


	//FadeIn_out();
//	Luma++;
//	TCNT3 = 0;
	
	
//	sei();
//	OCR3A = 5100; //10240
}

// timer is not the issue for sudden brightness, timer is an issue

ISR(TIMER1_COMPA_vect)
{
// 	_delay_ms(1000);
// 	PORTD ^=0xff;
	FadeIn_out();
//	OCR1A = 40000;
}