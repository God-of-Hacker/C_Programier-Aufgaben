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

#define ANZAHL  1000

//Hauptprogramm
int main(void)
{
    //Variablen
    
    //Speicherplatz für 255 Muster reservieren
    uint8_t muster[ANZAHL];
    
    
    
    //Initialisieren
    initBoard(1);
    //alle Muster auf 0 setzen
    for (uint16_t i = 0; i < ANZAHL; i=i+1)
    {
        muster[i] = 0;
    }
    
    lcdLog("Start Aufzeichnung");
    for (uint16_t i = 0; i < ANZAHL; i=i+1)
    {
        uint8_t schalter = switchReadAll();
        muster[i] = schalter;
        ledWriteAll(schalter);
        _delay_ms(10);
    }
    
    
    //Unendlichschlaufe
    while(1)
    {
        lcdLog("Wiedergabe");
        for (uint16_t i = 0; i < ANZAHL; i=i+1)
        {
            ledWriteAll(muster[i]);
            _delay_ms(10);
        }
        
    }
}

