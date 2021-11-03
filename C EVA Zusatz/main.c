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
    uint8_t schalter = 0;
    uint8_t S5 = 0; 
    uint8_t S6 = 0; 
    uint8_t S7 = 0; 
    uint8_t S8 = 0; 
    uint8_t S9 = 0;
    uint8_t S10 = 0;
    uint8_t S11 = 0;
    uint16_t H0 = 0;
    uint16_t H1 = 0;
    uint16_t H2 = 0;
    uint16_t H3 = 0;
    uint16_t H4 = 0;
    uint16_t H5 = 0;
    uint16_t H6 = 0;
    
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        schalter = switchReadAll();
        S5 = schalter & (1<<0);
        S6 = schalter & (1<<1);
        S7 = schalter & (1<<2);
        S8 = schalter & (1<<3);
        S9 = schalter & (1<<4);
        S10 = schalter & (1<<5);
        S11 = schalter & (1<<6);
        
        
        
        //Verarbeitung-------------------------------------------------------------
        
        if (S5)
        {
            H0 = H0 | (0x01);
        } 
        else
        {
            H0 = H0 & ~(0x01);
        }
        if (S6)
        {
            H1 = H1 | (0x02);
        } 
        else
        {
             H1 = H1 & ~(0x02);
        }
       if (S7)
       {
           H1 = H1 | (0x02);
           H2 = H2 | (0x04);
           H3 = H3 | (0x08);
           H4 = H4 | (0x10);
           
       } 
       else
       {
           
           H2 = H2 & ~(0x04);
           H3 = H3 & ~(0x08);
           H4 = H4 & ~(0x10);
       }
       if (S8 | S9)
       {
           H5 = H5 | (0x20); 
       } 
       else
       {
           H5 = H5 & ~(0x20);
       }
       if (S10 | S11)
       {
           H6 = H6 | (0x40);
       } 
       else
       {
           H6 = H6 & ~(0x40);
       }
      
        
        
        //Ausgabe------------------------------------------------------------------
        
        ledWriteAll(H0 | H1 | H2 | H3 | H4 | H5 | H6);
        
    }
}

