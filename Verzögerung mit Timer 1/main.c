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
    
    DDRB = 0xff;        // PORTB Bit 0...7 auf OûtPut (LED's)
    PORTB = 0x55;       // Jede zweite LED an PortB einschalten
    
    DDRA = 0xff;        // PORTA Bit 0...7 auf OûtPut (LED's)
    PORTA =0x55;        // Jede zweite LED an PortB einschalten
    
    // Timer Aufsetzen
    TCCR1A = 0x00;      //Toggle OC1A on compare match
    TCCR1B = 0x0D;      //In Autoload Modus (CTC) setzen und Frequenz = 16MHz / 1024 = 15'625 - 1 = 15'624
    
    OCR1A = 15624;              // 1 Sekunde
    TIFR1 = TIFR1 | 0x02;       // fals es auf 1 ist dann == Timer Request Flag Löschen mit Schreibvorgang ( Flag öschen nur mit High =1 )
    
    while (1)       // Programm Hauptschleife
    {
        while((TIFR1 & 0x02) == 0);     // Warten bis Timer 1 abgelaufen ist ( 1 Sekunde )
        TIFR1 = TIFR1 | 0x02;           // Timer Request Flag Löschen mit Schreibvorgang ( Flag öschen nur mit High =1 )
        PORTA = ~PORTA;         // LED's Invertieren (Testblinken)
        PORTB = ~PORTB;         // LED's Invertieren (Testblinken)
    }
}

