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

#define ZAHL_7SEG_0  (SEGMENT_A | SEGMENT_B | SEGMENT_C| SEGMENT_D| SEGMENT_E| SEGMENT_F )
#define ZAHL_7SEG_1  (SEGMENT_B | SEGMENT_C)
#define ZAHL_7SEG_2  (SEGMENT_A | SEGMENT_B | SEGMENT_G | SEGMENT_E | SEGMENT_D)
#define ZAHL_7SEG_3  (SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_G)
#define ZAHL_7SEG_4  (SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G)
#define ZAHL_7SEG_5  (SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G)
#define ZAHL_7SEG_6  (SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G)
#define ZAHL_7SEG_7  (SEGMENT_A | SEGMENT_B | SEGMENT_C)           
#define ZAHL_7SEG_8  (SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G)
#define ZAHL_7SEG_9  (SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G)
#define ZAHL_7SEG_10 (SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_E | SEGMENT_F | SEGMENT_G)
#define ZAHL_7SEG_11 (SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G)
#define ZAHL_7SEG_12 (SEGMENT_A | SEGMENT_F | SEGMENT_E | SEGMENT_D)
#define ZAHL_7SEG_13 (SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_G)
#define ZAHL_7SEG_14 (SEGMENT_A | SEGMENT_F | SEGMENT_E | SEGMENT_D | SEGMENT_G)
#define ZAHL_7SEG_15 (SEGMENT_A | SEGMENT_F | SEGMENT_E | SEGMENT_G)
#define OFF 0
#define  OFFSET 4
#define  SCHALTER_EINER  0b00001111
#define  SCHALTER_ZEHNER 0b11110000

const uint8_t DECORDER[] = {ZAHL_7SEG_0, ZAHL_7SEG_1, ZAHL_7SEG_2, ZAHL_7SEG_3, ZAHL_7SEG_4, ZAHL_7SEG_5,
                            ZAHL_7SEG_6, ZAHL_7SEG_7, ZAHL_7SEG_8, ZAHL_7SEG_9, ZAHL_7SEG_10,
                            ZAHL_7SEG_11, ZAHL_7SEG_12, ZAHL_7SEG_13, ZAHL_7SEG_14, ZAHL_7SEG_15};

//Hauptprogramm
int main(void)
{
    //Variablen
    uint8_t schalter = 0;
    uint8_t schalter2 = 0;
    uint8_t zahl = 0;
    //Initialisieren
    initBoard(1);
    DDRD = 0xff; //Alle Bits von PortD als Ausgang konfugurieren
    //Unendlichschlaufe
    while(1)
    {
        schalter = switchReadAll() & SCHALTER_EINER;
        schalter2 = (switchReadAll() & SCHALTER_ZEHNER) >> OFFSET;
        if ((schalter > 9) || (schalter2 > 9))
        { 
            PORTD = SEGMENT_DP;
            
        } 
        else 
        {
           zahl = schalter + (10*schalter2);
           if (zahl > 15)
           {
               PORTD = OFF;
           }
           else
           {
               PORTD = DECORDER[zahl];
           }
        }
        
            
         
        
           
        
        
        
    }
}

