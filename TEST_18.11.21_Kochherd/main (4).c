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
* Projekt  : Kochfeld
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
* 19.11.2011  D. Dündar      V1.0      Neuerstellung
*
\*********************************************************************************/

//uC-Board-Treiber hinzufügen
#include "ucBoardDriver.h"

#define IN_MASK_POWER           (0b1<<0)
#define IN_MASK_TEMP_SENS       (0b1<<1)
#define IN_MASK_DREHKNOPF       (0b1111<<2)
#define OFFSET_DREHKNOPF        2
#define MAX_DREHKNOPF           15

#define OUT_MASK_POWER          (1<<0)
#define OUT_MASK_TEMP_WARN      (1<<1)
#define OUT_MASK_GROSSE_PLATTE  (1<<2)
#define OUT_MASKE_HEIZSTUFE     (31<<3)
#define OFFSET_HEIZSTUFE        3
#define MAX_HEIZSTUFE           5

#define OFF                     0

//Hauptprogramm
int main(void)
{
    //Variablen
    uint8_t inSchalter=0;
    uint8_t inPower=0;
    uint8_t inTempSens=0;
    uint8_t inDrehknopf=0;
    uint8_t inHeizstufe=0;
    
    uint8_t outPower=0;
    uint8_t outTempWarn=0;
    uint8_t outGrossePlatte=0;
    uint8_t outHeizstufe=0;
    //Initialisieren
    initBoard(0);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        inSchalter = switchReadAll();
        inPower = inSchalter & IN_MASK_POWER;
        inTempSens = !(inSchalter & IN_MASK_TEMP_SENS);
        inDrehknopf = (inSchalter & IN_MASK_DREHKNOPF)>>OFFSET_DREHKNOPF;
        if (inDrehknopf <= 10)
        { 
            inHeizstufe = inDrehknopf/2;
        }
        else
        {
            inHeizstufe = 5;
        }
        
        //Verarbeitung-------------------------------------------------------------
        if (inPower)
        {
            outPower = OUT_MASK_POWER;
            //Einzeiler
            outHeizstufe =  (OUT_MASKE_HEIZSTUFE>>(MAX_HEIZSTUFE-inHeizstufe)) &
                            OUT_MASKE_HEIZSTUFE;
            //----------------------------------------------------
            //for-Loop
            outHeizstufe = 0;
            for (uint8_t i = 0; i < inHeizstufe; i = i + 1)
            {
                outHeizstufe = outHeizstufe << 1;
                outHeizstufe = outHeizstufe + 1;
            }
            outHeizstufe = outHeizstufe << OFFSET_HEIZSTUFE;
            //----------------------------------------------------
            if (inDrehknopf == MAX_DREHKNOPF)
            {
                outGrossePlatte = OUT_MASK_GROSSE_PLATTE;
            }
        } 
        else
        {
            outPower = OFF;
            outHeizstufe = OFF;
            outGrossePlatte = OFF;
        }
        
        if (inTempSens)
        {
            outTempWarn = OUT_MASK_TEMP_WARN;
        } 
        else
        {
            outTempWarn = OFF;
        }
        
        
        
        //Ausgabe------------------------------------------------------------------
        ledWriteAll(outPower | outTempWarn | outHeizstufe | outGrossePlatte);
    }
}


