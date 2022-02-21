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
#define ON_LED              0xFFFF
#define SYSTEM_TICK_MS      50
#define OFF                 0

//Hauptprogramm
int main(void)
{
    //Variablen
    uint16_t button_neu = 0;
    uint16_t button_alt = 0;
    uint16_t Positiv_flanke = 0;
    uint16_t flanken = 0;
    uint16_t negative_flanke = 0;
    uint16_t Besucher = 0;
    uint16_t posFlankePWr = 0;
    uint16_t negFlankePWr = 0;
    uint16_t posFlankePWr1 = 0;
    uint16_t negFlankePWr1 = 0;
    uint16_t posFlankePWr2 = 0;
    uint16_t negFlankePWr2 = 0;
    
    
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------   
        button_alt = button_neu;                    //1100 0000      start
        button_neu = buttonReadAllPL();             //0100 0011
        flanken = button_alt ^ button_neu;          //1000 0011
        Positiv_flanke = flanken & button_neu;      //0000 0011
        negative_flanke = flanken & button_alt;     //1000 0000
        posFlankePWr = Positiv_flanke & 0x01;       // mit dem kann ich Button bestimmen PL0,PL1,PL6,PL7
        negFlankePWr = negative_flanke & 0x01;
        posFlankePWr1 = Positiv_flanke & 0x02;       // mit dem kann ich Button bestimmen 01, 02, 04, 08
        negFlankePWr1 = negative_flanke & 0x02;
        posFlankePWr2 = Positiv_flanke & 0x08;       // mit dem kann ich Button bestimmen PL0,PL1,PL6,PL7
        negFlankePWr2 = negative_flanke & 0x08;
        
        //Verarbeitung-------------------------------------------------------------
          if (posFlankePWr)
          {
              Besucher = Besucher + 1;
          }
          if (posFlankePWr1)
          {
              Besucher = Besucher - 1;
          }
          if (posFlankePWr2)
          {
              Besucher = Besucher == 0;
          }
        //Ausgabe------------------------------------------------------------------
        lcdWriteText(1,0,"%u Besucher          ",Besucher);
    
        
        //Warten-------------------------------------------------------------------
        _delay_ms(SYSTEM_TICK_MS);
       
       
        } 
        
         
         
}

