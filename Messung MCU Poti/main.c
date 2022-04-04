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
    uint16_t inSpannung_ADC=0;
    uint16_t inSpannung_ADC1=0;
    uint16_t inSpannung_mV=0;
    uint16_t inSpannung_mV1=0;
    uint16_t outQ1leit = 0;
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        inSpannung_ADC = adcRead(ADC_08_POTI_1);
        inSpannung_ADC1 = adcRead(ADC_09_POTI_2);
        inSpannung_mV = (uint32_t)inSpannung_ADC * 5000 /1023;
        inSpannung_mV1 = (uint32_t)inSpannung_ADC1 * 5000 /1023;
        //Verarbeitung-------------------------------------------------------------
        if (inSpannung_mV >= inSpannung_mV1)
        {
            outQ1leit= 0;
        }
        else
        {
            outQ1leit= 0xffff;
        }
        //Ausgabe------------------------------------------------------------------
        lcdLog("%u , %u", inSpannung_mV, inSpannung_mV1);
        ledWriteAll(outQ1leit);
    }
}

