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
* Projekt  : DMS Waage
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
*09.05.2022 F. Russom      V1.0      Neuerstellung
*
\*********************************************************************************/

//uC-Board-Treiber hinzufügen
#include "ucBoardDriver.h"
#define START_HI          140
#define HANDY_HI          3000
#define STABILO_HI        185
#define MAUS_HI           1000
#define EISEN_HI          3400

#define START_LO          100
#define HANDY_LO          2300
#define STABILO_LO        150
#define MAUS_LO           950
#define EISEN_LO          3200

#define UNBEKANNT_HI        850
#define UNBEKANNT_LO        200
#define UNBEKANNT_1_HI        1900
#define UNBEKANNT_1_LO        1100
#define UNBEKANNT           3500
//Hauptprogramm
int main(void)
{
    //Variablen
    uint32_t eingangSpannung_mv = 0;
    uint32_t gewicht=0;
    uint32_t faktor =0;

    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        eingangSpannung_mv = (uint32_t)adcRead(ADC_00_X4_PORTF_BIT0) * 5000 / 1023 - 450; // Eingang Speisung Extern
        gewicht = eingangSpannung_mv * 353 / 3300;
        //Verarbeitung-------------------------------------------------------------
        if (eingangSpannung_mv <=START_HI)
        {
            rgbWrite(0,0,0);
            lcdWriteText(2,0,"Gegenstand: leer     ");
        }
        else if (eingangSpannung_mv < START_LO)
        {
            rgbWrite(250,0,0);
            lcdWriteText(2,0,"Gegenstand: Unbekannt    ");
        }
        
        else if ((eingangSpannung_mv >= STABILO_LO) && (eingangSpannung_mv <= STABILO_HI))
        {
            rgbWrite(0,0,250);
            lcdWriteText(2,0,"Gegenstand: Stabilo   ");
        }
        else if ((eingangSpannung_mv >= UNBEKANNT_LO) && (eingangSpannung_mv <= UNBEKANNT_HI))
        {
            rgbWrite(250,0,0);
        }
        else if ((eingangSpannung_mv >= MAUS_LO) && (eingangSpannung_mv <= MAUS_HI))
        {
            rgbWrite(0,0,250);
            lcdWriteText(2,0,"Gegenstand: Maus     ");
        }
        else if ((eingangSpannung_mv >= UNBEKANNT_1_LO) && (eingangSpannung_mv <= UNBEKANNT_1_HI))
        {
            rgbWrite(250,0,0);
        }
        else if ((eingangSpannung_mv >= HANDY_LO) && (eingangSpannung_mv <= HANDY_HI))
        {
            rgbWrite(0,0,250);
            lcdWriteText(2,0,"Gegenstand: Handy    ");
        }
        else if ((eingangSpannung_mv >= EISEN_LO) && (eingangSpannung_mv <= EISEN_HI))
        {
            rgbWrite(0,0,250);
            lcdWriteText(2,0,"Gegenstand: Eisen     ");
        }
        else if (eingangSpannung_mv > UNBEKANNT)
        {
            rgbWrite(250,0,0);
            lcdWriteText(2,0,"Gegenstand: Unbekannt    ");
        }
        
        
        //Ausgabe------------------------------------------------------------------
        lcdWriteText(0,0,"DMS-Waage  V1.0  FR");
        lcdWriteText(1,0,"Masse =  %04u g   ", gewicht);
        rgbWrite(0,0,0);
        lcdWriteText(3,0,"Eingang: %04u ", eingangSpannung_mv);
    }
}

