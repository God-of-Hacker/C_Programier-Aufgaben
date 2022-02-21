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
    int16_t uT1 = 0;
    int16_t uT2 = 0;
    uint16_t ledshall1 = 0;
    uint16_t ledshall2 = 0;
    
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        inAdcHall1 = adcRead(ADC_14_MAGNET_1);  //0v...5v
        inAdcHall2 = adcRead(ADC_15_MAGNET_2);  //0v...5v

        //Eingabe------------------------------------------------------------------
        inHall1_mv = (uint32_t)inAdcHall1 * 5000 /1023;  //0mv...5000mv
        inHall2_mv = (uint32_t)inAdcHall2 * 5000 /1023;  //0mv...5000mv
        //Verarbeitung------------------------------------------------------------- // unsigned ohne Vorzeichen, signed mit Vorzeigen
        outgausshall1 = (inHall1_mv - 2500) *10/15;
        outgausshall2 = (inHall2_mv - 2500) *10/15;
        uT1 = outgausshall1 * 100;
        uT2 = outgausshall2 * 100;
        
        ledshall1 = (0b11111110>>(7-outgausshall1)) &      // mit dem schalter kann man auf LED binär zählen
        0b11111110;
        
        //Ausgabe------------------------------------------------------------------   //Zeile von rechts nach links ,spalte von oben nach unten
        lcdWriteText(0,0,"ADC: %4d",inAdcHall1);  
        lcdWriteText(0,12,"%4d",inAdcHall2);
        lcdWriteText(1,0,"mv:  %4d",inHall1_mv);  
        lcdWriteText(1,12,"%4d",inHall2_mv); 
        lcdWriteText(2,0,"Gauss:%4d",outgausshall1);
        lcdWriteText(2,12,"%4d",outgausshall2 );
        lcdWriteText(3,0,"uT: %6d",uT1 );
        lcdWriteText(3,12,"%6d",uT2 );
        ledRead(ledshall1 | ledshall2);
    }
}

