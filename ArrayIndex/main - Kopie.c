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

#define ANZAHL  256

//Hauptprogramm
int main(void)
{
    //Variablen
    
    //Speicherplatz für 255 Muster reservieren
    uint16_t muster[ANZAHL];
    
    uint8_t adresse=0;
    uint16_t ausgabe=0;
    uint8_t inTaster=0;
    uint8_t inTasterAlt=0;
    uint8_t posWrite=0;
    uint8_t posRead=0;
    
    //Initialisieren
    initBoard(1);
    //alle Muster auf 0 setzen
    for (uint16_t i = 0; i < ANZAHL; i=i+1)
    {
        muster[i] = 0;
    }
    
    
    
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        inTasterAlt=inTaster;
        inTaster=buttonReadAllPL();
        posRead=(inTasterAlt^inTaster)&inTaster&1;
        posWrite=(inTasterAlt^inTaster)&inTaster&2;
        adresse = switchReadAll();
        
        //Verarbeitung-------------------------------------------------------------
        
        if (posRead)
        {
            lcdLog("RI: %u W: %u",adresse,muster[adresse]);
        }
        if (posWrite)
        {
            //Wert von ADC08 (Poti0) im Array speichern
            //(Adresse von den Schaltern)
            muster[adresse] = adcRead(ADC_08_POTI_1);
            lcdLog("WI: %u W: %u",adresse,muster[adresse]);
        }
        
        //Ausgabe------------------------------------------------------------------
    }
}

