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
    
    //Initialisieren
    initBoard(1);
    pinInitX4PortL(2,OUTPUT);
    uint16_t uIst_mV=0;
    uint16_t uSoll_mV1=0;
    uint16_t outQ1leit = 0;
    uint8_t counter = 0;
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        uIst_mV = (uint32_t)adcRead(ADC_03_X4_PORTF_BIT3) * 5000 /1023;
        uSoll_mV1 = 2500;//(uint32_t)adcRead(ADC_08_POTI_1) * 5000 /1023;
        //Verarbeitung-------------------------------------------------------------
        if (uIst_mV >= uSoll_mV1)
        {
            outQ1leit= 0xffff;
        }
        else
        {
            outQ1leit= 0;
        }
        //Ausgabe------------------------------------------------------------------
       pinWriteX4PortL(2,!outQ1leit);
       ledWriteAll(outQ1leit);
       counter = counter +1;
        if (!(counter%200))
        {
            lcdWriteText( 0,0,"ist:  %4u mV" ,uIst_mV);
            lcdWriteText( 1,0,"Soll: %4u mv",uSoll_mV1);
        }
        
    }
}

