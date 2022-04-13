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

//Hauptprogramm
int main(void)
{
    //Variablen
    char meinName[] = {70, 105, 108, 109, 111, 110, 0};
    char meinName2[] = {'F', 'i', 'l', 'm', 'o', 'n',0};
    char meinName3[] = "Filmon";
    uint16_t length = 0;
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        
        //Verarbeitung-------------------------------------------------------------
        length= 0;
        for (uint16_t i=0; meinName3[i]!=0; i=i+1)
        {
            length =length + 1;
        }
        
        //Ausgabe------------------------------------------------------------------
        lcdWriteText(1,0,"text: %s", meinName3);
        lcdWriteText(2,0,"length: %u", length);
    }
}

