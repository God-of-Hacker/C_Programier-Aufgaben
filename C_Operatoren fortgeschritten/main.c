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

//Hauptprogramm
int main(void)
{
    //Variablen
    uint16_t schalter = 0;
    uint16_t leds = 0;
    uint16_t S7 = 0;
    uint16_t S6 = 0;
    uint16_t S5 = 0;
    uint16_t S4 = 0;
    uint16_t S3 = 0;
    uint16_t S2 = 0;
    uint16_t S1 = 0;  
    uint16_t S0 = 0; 
    uint16_t S1_3 = 0;
    
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        schalter = switchReadAll();
        S7 = schalter & 0b10000000;
        S6 = schalter & 0b01000000;
        S5 = schalter & 0b00100000;
        S4 = schalter & 0b00010000;
        S3 = schalter & 0b00001000;
        S2 = schalter & 0b00000100;
        S1 = schalter & 0b00000010;
        S0 = schalter & 0b00000001;
        S1_3 = switchReadAll() & 0b00001110;
        S1_3 = S1_3 >> 1;
       
        
        //Verarbeitung-------------------------------------------------------------
        if (S7)
        {   leds = leds | 0x01;         //Aufgabe 1.1
        } 
        else
        {   leds = leds & 0xfffe;
        }
       if (S7 && S6 && !S5)
       {    leds = leds | (1<<1);       //Aufgabe 1.2
       } 
       else
       {    leds = leds & ~(1<<1);
       }
       if (S6 && S5 && S4)
       {    leds = leds & ~(1<<2);      //Aufgabe 1.3
       } 
       else
       {    leds = leds | (1<<2);
       }
      if (S1_3 % 2)
      {     leds = leds | (0x04);       //Aufgabe 1.4
      } 
      else
      {     leds = leds & ~(0x04);
      }
        //Ausgabe------------------------------------------------------------------
        ledWriteAll(leds);
        
    }
}

