/*********************************************************************************\
*
* MMMMMMMMMMMM   SSSSSSSSSSSS   WW   WW   WW   MECHATRONIK
* MM   MM   MM   SS             WW   WW   WW   SCHULE
* MM   MM   MM   SSSSSSSSSSSS   WW   WW   WW   WINTERTHUR
* MM   MM   MM             SS   WW   WW   WW   
* MM   MM   MM   SSSSSSSSSSSS   WWWWWWWWWWWW   www.msw.ch
*
*
* Dateiname: main.c
*
* Projekt  : Muster
* Hardware : uC-Board, ATmega2560v von Atmel
*
* Copyright: MSW, E2
*
* Beschreibung:
* =============
* Siehe Prüfungsaufgabe
*
* Portbelegung:
* =============
* Port C:   Schalter
* Port A:   LEDs
* für genauere Beschreibung siehe Hardwarestruktur
*
* Verlauf:
* ========
* Datum:      Autor:         Version   Grund der Änderung:
* dd.mm.yyyy  M. Muster      V1.0      Neuerstellung
*
\*********************************************************************************/

//uC-Board-Treiber hinzufügen
#include "ucBoardDriver.h"
#define SEGMENT_A  (1<<5)
#define SEGMENT_B  (1<<6)
#define SEGMENT_C  (1<<2)
#define SEGMENT_D  (1<<3)
#define SEGMENT_E  (1<<4)
#define SEGMENT_F  (1<<0)
#define SEGMENT_G  (1<<1)
#define SEGMENT_DP (1<<7)

#define ZAHL_7SEG_0 (SEGMENT_A | SEGMENT_B | SEGMENT_C| SEGMENT_D| SEGMENT_E| SEGMENT_F )
#define ZAHL_7SEG_7 (SEGMENT_A | SEGMENT_B | SEGMENT_C)           
#define ZAHL_7SEG_8 (SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G)
#define ZAHL_7SEG_4 (SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G)
#define ZAHL_7SEG_2 (SEGMENT_A | SEGMENT_B | SEGMENT_G | SEGMENT_E | SEGMENT_D)
#define ZAHL_7SEG_9 (SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G)
#define ZAHL_7SEG_6 (SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G)

#define LED_AUS  (250)
#define LED_AN  (1000)
#define OFF 0




//Hauptprogramm
int main(void)
{
    //Variablen
    
    //Initialisieren
    initBoard(1);
    DDRD = 0xff; //Alle Bits von PortD als Ausgang konfugurieren
    //Unendlichschlaufe
    while(1)
    {
        //078 420 92 60
        
        PORTD = ZAHL_7SEG_0;
        _delay_ms(LED_AN);
        PORTD = OFF;
        _delay_ms(LED_AUS);
        
        PORTD = ZAHL_7SEG_7;
        _delay_ms(LED_AN);
        PORTD = OFF;
        _delay_ms(LED_AUS);
        
        PORTD = ZAHL_7SEG_8;
        _delay_ms(LED_AN);
        PORTD = OFF;
        _delay_ms(LED_AUS);
        
        PORTD = ZAHL_7SEG_4;
        _delay_ms(LED_AN);
        PORTD = OFF;
        _delay_ms(LED_AUS);
        
        PORTD = ZAHL_7SEG_2;
        _delay_ms(LED_AN);
        PORTD = OFF;
        _delay_ms(LED_AUS);
        
        PORTD = ZAHL_7SEG_0;
        _delay_ms(LED_AN);
        PORTD = OFF;
        _delay_ms(LED_AUS);
        
        PORTD = ZAHL_7SEG_9;
        _delay_ms(LED_AN);
        PORTD = OFF;
        _delay_ms(LED_AUS);
        
        PORTD = ZAHL_7SEG_2;
        _delay_ms(LED_AN);
        PORTD = OFF;
        _delay_ms(LED_AUS);
        
        PORTD = ZAHL_7SEG_6;
        _delay_ms(LED_AN);
        PORTD = OFF;
        _delay_ms(LED_AUS);
        
        PORTD = ZAHL_7SEG_0;
        _delay_ms(LED_AN);
        PORTD = OFF;
        _delay_ms(LED_AUS);
        
    
        
        PORTD = switchReadAll();
        
    }
}

