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
#define SIZE 256
#pragma gcc optimize 0
//Hauptprogramm
int main(void)
{
    //Variablen
    uint16_t meinArray [SIZE];
    //Initialisieren
    initBoard(1);
    lcdLogWaitBtn(1,"Start:");
    for (uint16_t i=0;i<SIZE; i=i+1)
    {
        meinArray[i]=i;
    }
    lcdLogWaitBtn(1,"%u:", meinArray[56]);
    
    for (uint16_t i=0;i<SIZE; i=i+1)
    {
        meinArray[i]=meinArray[i+1];
    }
    lcdLogWaitBtn(1,"%u:", meinArray[10]);
    
    for (uint16_t i=0;i<SIZE; i=i+1)
    {
        meinArray[i]=meinArray[i+5];
    }
    lcdLogWaitBtn(1,"%u:", meinArray[25]);
    
    for (uint16_t i=0;i<SIZE; i=i+1)
    {
        meinArray[i]=meinArray[i+5];
    }
    lcdLogWaitBtn(1,"%u:", meinArray[15]);
    

    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        //Verarbeitung-------------------------------------------------------------
        
        //Ausgabe------------------------------------------------------------------
        
    }
}

/****
1: Start
2: 56
3: 11
4: 31
5: 26
*****/
