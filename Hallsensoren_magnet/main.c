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
    uint16_t inAdcHall1 = 0;
    uint16_t inAdcHall2 = 0;
    int16_t inHall1_mv = 0;
    int16_t inHall2_mv = 0;
    int16_t outgausshall1 = 0;
    int16_t outgausshall2 = 0;
    //Initialisieren
    initBoard(1);
    inAdcHall1 = adcRead(ADC_14_MAGNET_1);  //0v...5v
    inAdcHall2 = adcRead(ADC_15_MAGNET_2);  //0v...5v
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        inHall1_mv = (uint32_t)inAdcHall1 * 5000 /1023;  //0mv...5000mv
        inAdcHall2 = (uint32_t)inAdcHall2 * 5000 /1023;  //0mv...5000mv
        //Verarbeitung------------------------------------------------------------- // unsigned ohne Vorzeichen, signed mit Vorzeigen
        
        outgausshall1 = 
        
        //Ausgabe------------------------------------------------------------------   //Zeile von rechts nach links ,spalte von oben nach unten
        lcdWriteText(1,0,"ADC: %42" inAdcHall1);  
        lcdWriteText(1,10,"%4d" inAdcHall2);
        lcdWriteText(2,0,"mv: %4d" inHall1_mv);  
        lcdWriteText(2,10,"%4d" inHall2_mv); 
        lcdWriteText(3,0,"Gauss: %4d" outgausshall1 );
        lcdWriteText(3,10,"%4d" outgausshall2 );
    }
}

