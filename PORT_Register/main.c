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
* dd.mm.yyyy  F. Russom      V1.0      Neuerstellung
*
\*********************************************************************************/

//uC-Board-Treiber hinzufügen
#include "ucBoardDriver.h"
#define  LED_0_1                (0b11<<0) //0b00000011
#define  LED_2_3                (0b11<<2) //0b00001100
#define  LED_4_5                (0b11<<4) //0b00110000
#define  LED_6_7                (0b11<<6) //0b11000000


//Gute Namen für Maske
#define PULL_UO_SWITCHES
#define DDR_LED_B
#define DDR_SWITCHES
#define OUT_LED_A
#define OUT_LED_B
#define IN_SWITCHES



//Hauptprogramm
int main(void)
{
    //Variablen                                                             C Bibel seite 31
    
    //Initialisieren
    initBoard(0);
    
//     DDRA=0xff;
//     PORTA=0;
//     while(1);
    
    
    //1) Ausgang alles LED Low :                    für Ausgang muss DDRA auf 1 sein und für LED Low muss PORT auf 0 sein
    DDRA = DDRA | LED_0_1;
    PORTA = PORTA & (~LED_0_1);
    
    //2) Ausgang alles LED High :                   für Ausgang muss DDRA auf 1 sein und für LED High muss PORT auf 1 sein
    DDRA = DDRA | LED_2_3;
    PORTA = PORTA | LED_2_3;
    
    //3) Eingang mit Pullup :                       für Eingang muss DDRA auf 0 sein und mit Pullup muss PORT auf 1 sein
    DDRA = DDRA & (~LED_4_5);
    PORTA = PORTA | LED_4_5;
    
    //4) Eingang ohne Pullup :                      für Eingang muss DDRA auf 0 sein und ohne Pullup muss PORT auf 0 sein
    DDRA = DDRA & (~LED_6_7);
    PORTA = PORTA & (~LED_6_7);
    
    //5) Schalter Eingang mit Pullup
    DDRC = 0;
    PORTC = 0xff;
    
    //6) PORTB ist für die Anzeige an LED's und es muss auf Ausgang definiert sien also alles (1) ein
    DDRB = 0xff;
    
    //Unendlichschlaufe
    while(1)
    {
        //7) für LED's zeigt PORT
        PORTB = PINC;
        
    }
}

