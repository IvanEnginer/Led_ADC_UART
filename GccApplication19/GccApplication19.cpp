#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define ADC_VREF_TYPE 0x40
#define ADC_CH1_MUX 0x1
#define ADC_CH2_MUX 0x2
#define ADC_CH3_MUX 0x3

uint8_t i = 0;
void LED_init(void)
{
	printf("Led initialization....\r\n");
	char j =0;
	
	DDRD|= (1<<DDD7);
	DDRB|= ((1<<DDB0)|(1<<DDB1)|(1<<DDB2));
	
	for(j=0;j<5;j++)
	{
		PORTD|=(1<<PD7);
		PORTB|=(1<<PB0)|(1<<PB1)|(1<<PB2)
		_delay_ms(600);
		PORTB&= ~((1<<PB0)|(1<<PB1)|(1<<PB2));
		PORTD&= ~(1<<PD7);
		_delay_ms(600);
	}
}

static int my_putchar(char c,FILE *stream)
{
	while((UCSRA&(1<<UDRE)) == 0);
	UDR = c;
	return 0;
}

void myUART_init(void)
{
	UCSRA=0x00;
	UCSRB=0x08;
	UCSRC=0x80;
	UBRRH=0x00;
	UBRRL=0x19;
	
	static FILE mystdout = FDEV_SETUP_STREAM(my_putchar, NULL,_FDEV_SETUP_WRITE);
	stdout = &mystdout;
	
	printf("UART initialization....\r\n");
}

void ADC_init(void)
{
	printf("ADC initialization");
	ADMUX=ADC_VREF_TYPE;
	ADCSRA=0x8E;
}

ISR(ADC_vect)
{
	printf("I am in interrupt....\r\n");
		uint16_t data = ADCL;
	data+= (ADCH<<8);
		printf("Ch_%u=%u\r\n",i,data);
		uint8_t pr = ADMUX;
		printf("ADMUX = %u\r\n",pr);
		data = 0;
		
if (i == 3)
{
	i = 0;
	ADMUX&= ~ADC_CH3_MUX;
	
	PORTB&= ~(1<<PB2);
	PORTD|=(1<<PD7);
	
	data =ADMUX;
	printf("3to0,ADMUX = %u\r\n",data);
	data=0;
	
	_delay_ms(25);
} 
else
{
	if (i == 0)
	{
		ADMUX&= ~ADC_CH1_MUX;
		
		PORTD&= ~(1<<PD7);
		PORTB|= (1<<PB2);
		
		data =ADMUX;
		printf("0to1,ADMUX = %u\r\n",data);
		data=0;
	}
	if (i == 1)
	{
		ADMUX&= ~ADC_CH1_MUX;
		ADMUX&= ~ADC_CH2_MUX;
		
		PORTB&= ~(1<<PD7);
		PORTB|= (1<<PB2);
		
		data =ADMUX;
		printf("0to1,ADMUX = %u\r\n",data);
		data=0;
	}
	i++;
	_delay_ms(25);
}

ADCSRA|=0x40;
};

int main(void)
{
    while(1)
    {
        
    }
}