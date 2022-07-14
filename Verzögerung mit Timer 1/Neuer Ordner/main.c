/*
 * Port.c
 *
 * Created: 14.07.2022 16:11:18
 * Author : Filmon
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
    DDRB = 0xff;
    PORTB = 0x55;
    
    DDRA = 0xff;
    PORTA =0x55;
    
    TCCR1A = 0x00;
    TCCR1B = 0x0D;
    
    OCR1A = 15624;
    TIFR1 = TIFR1 | 0x02;
    
    while (1) 
    {
        while((TIFR1 & 0x02) == 0);
        TIFR1 = TIFR1 | 0x02;
        PORTA = ~PORTA;
        PORTB = ~PORTB;
    }
}

