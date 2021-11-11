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

#define     IN_MASKE_WOHNZIMMER     (0x01)
#define     IN_MASKE_BAD            (0x04)
#define     IN_MASKE_1OG            (0x10)
#define     IN_MASKE_2OG            (0x20)
#define     IN_MASKE_EG             (0x40)
#define     IN_MASKE_LICHTSENS      (0x80)

#define     OUT_MASKE_WOHNZIMMER    (0x01)
#define     OUT_MASKE_BAD           (0x02)
#define     OUT_MASKE_SPIEGEL       (0x04)
#define     OUT_MASKE_OG            (0x08)
#define     OUT_MASKE_EG            (0x10)

#define     OFF                        0




//Hauptprogramm
int main(void)
{
    //Variablen
    uint8_t schalter = 0;
    uint8_t inWohnzimmer = 0;
    uint8_t outWohnzimmer = 0;
    uint8_t inBad = 0;
    uint8_t outBad = 0;
    uint8_t outSpiegel = 0;
    uint8_t in1Og = 0;
    uint8_t in2Og = 0;
    uint8_t inEg = 0;
    uint8_t outEg = 0;
    uint8_t inLichtSens = 0;
    uint8_t outOg = 0;
    
    
    
    
    
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        schalter = switchReadAll();
        inWohnzimmer = schalter & IN_MASKE_WOHNZIMMER;
        inBad = schalter & IN_MASKE_BAD; 
        in1Og = schalter & IN_MASKE_1OG;
        in2Og = schalter & IN_MASKE_2OG;
        inEg  = schalter & IN_MASKE_EG;
        inLichtSens = schalter & IN_MASKE_LICHTSENS;
        
        //Verarbeitung-------------------------------------------------------------
        if (inWohnzimmer)
        {   
            outWohnzimmer = OUT_MASKE_WOHNZIMMER;
        } 
        else
        {  
             outWohnzimmer = OFF;
        }
        if (inBad)
        {
            outBad = OUT_MASKE_BAD;
            outSpiegel = OUT_MASKE_SPIEGEL;
        } 
        else
        {
            outBad = OFF;
            outSpiegel = OFF;
        }
        if (in1Og | in2Og | inEg  )
        {
            outEg = OUT_MASKE_EG;
            outOg = OUT_MASKE_OG;
           
        } 
        else
        {
            outEg = OFF;
            outOg = OFF;
            
        }
        if (inLichtSens)
        {
            outEg = OFF;
            outOg = OFF;
        } 
        
        //Ausgabe------------------------------------------------------------------
        ledWriteAll(outWohnzimmer | outBad | outSpiegel | outEg | outOg);
    }
}

