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

#define ANZAHL 256

//Hauptprogramm
int main(void)
{
    //Variablen
    uint16_t leds = 0;
    //Speicherplatz für 255 Muster reserviert
    uint16_t muster [ANZAHL];
    
    uint8_t schalter=0;
    //Initialisieren
    initBoard(1);
    //alle Muster auf 0 setzen  
    for (uint16_t i = 0; i < ANZAHL; i=i+1)
    {
        muster[i] = 0;
    }
    
    //Bestimte Muster definieren
    muster[0]   = 0xFFFF;
    muster[1]   = 0x0001;
    muster[2]   = 0xAAAA;
    muster[128] = 0xF000;
    muster[192] = 0x0F0F;
    muster[255] = 0x000F;
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        schalter = switchReadAll();
        //Verarbeitung-------------------------------------------------------------
        leds = muster[schalter];
        //Ausgabe------------------------------------------------------------------
        ledWriteAll(leds);
    }
}

