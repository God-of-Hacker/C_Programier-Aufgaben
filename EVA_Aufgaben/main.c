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
        inWohnzimmer = schalter & 0b00000001;
        inBad = schalter & 0b00000010; 
        in1Og = schalter & 0b00000100;
        in2Og = schalter & 0b00001000;
        inEg  = schalter & 0b00010000;
        inLichtSens = schalter & 0b00100000;
        
        //Verarbeitung-------------------------------------------------------------
        if (inWohnzimmer)
        {   
            outWohnzimmer = outWohnzimmer | (0x01);
        } 
        else
        {  
             outWohnzimmer = outWohnzimmer & ~(0x01);
        }
        if (inBad)
        {
            outBad = outBad | (0x02);
            outSpiegel = outSpiegel | (0x04);
        } 
        else
        {
            outBad = outBad & ~(0x02);
            outSpiegel = outSpiegel & ~(0x04);
        }
        if (in1Og | in2Og | inEg  )
        {
            outEg = outEg | (0x08);
            outOg = outOg | (0x08);
           
        } 
        else
        {
            outEg = outEg & ~(0x08);
            outOg = outOg & ~(0x08);
            
        }
        if (inLichtSens)
        {
            outEg = outEg & ~(0x08);
            outOg = outOg & ~(0x08);
        } 
        else
        {
           
        }
        //Ausgabe------------------------------------------------------------------
        ledWriteAll(outWohnzimmer | outBad | outSpiegel | outEg | outOg);
    }
}

