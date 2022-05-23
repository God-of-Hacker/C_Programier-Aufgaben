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
* Projekt  : Labor Test
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
* 29.11.2021  F.  Russom      V1.0      Neuerstellung
*
\*********************************************************************************/

//uC-Board-Treiber hinzufügen
#include "ucBoardDriver.h"

//Hauptprogramm
int main(void)
{
    //Variablen
    uint16_t spannung_mv = 0;
    uint16_t spannung_Messwert_mV = 0;
    uint16_t spannungWiederstand1_mV = 0;
    
    //Initialisieren
    initBoard(1);
    lcdLogSetLinesToUse(2);
    lcdLog("Start");
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        spannung_mv = (uint32_t)adcRead(ADC_00_X4_PORTF_BIT0) * 5000 / 1023; // Eingang Speisung Extern
        spannung_Messwert_mV = (uint32_t)adcRead(ADC_01_X4_PORTF_BIT1)  * 5000 / 1023; // Spanuung über die LED
        if (spannung_mv >= spannung_Messwert_mV )
        {
            spannungWiederstand1_mV = spannung_mv - spannung_Messwert_mV;
        }
        else
        {
            spannungWiederstand1_mV = 0;
        }
        //Verarbeitung-------------------------------------------------------------
        
        if (spannung_mv > 4000)
        {
            lcdWriteText(1,0,"Speisung  OK     ");
            
            //lcdWriteText(3,0,"V= %5u mV        ", spannung_mv);
           if ((spannung_Messwert_mV >= 2200) && (spannung_Messwert_mV <= 2700)) // grüne LED
           {
               rgbWrite(0,1023,0);
               lcdWriteText(3,0,"V-Dioden: %5u mV ", spannung_Messwert_mV);
              lcdWriteText(2,0,"Resultat: Grün      ");
           }
           if ((spannung_Messwert_mV >= 1700) && (spannung_Messwert_mV <= 2100)) // Rot LED
           {
               rgbWrite(1023,0,0);
               lcdWriteText(3,0,"V-Dioden: %5u mV ", spannung_Messwert_mV);
               lcdWriteText(2,0,"Resultat: Rot       ");
               
           }
           if ((spannung_Messwert_mV >= 2700) && (spannung_Messwert_mV <= 3100)) // Rot LED
           {
               rgbWrite(1023,0,0);
               lcdWriteText(3,0,"V-Dioden: %5u mV ", spannung_Messwert_mV);
               lcdWriteText(2,0,"Resultat: Blau      ");
               
           }
           if ((spannung_Messwert_mV >= 3100) && (spannung_Messwert_mV <= 3400)) // Rot LED
           {
               rgbWrite(1023,0,0);
               lcdWriteText(3,0,"V-Dioden: %5u mV ", spannung_Messwert_mV);
               lcdWriteText(2,0,"Resultat: Weiss     ");
               
           }
            if ((spannung_Messwert_mV >= 500) && (spannung_Messwert_mV <= 700)) // Rot LED
            {
                rgbWrite(1023,0,0);
                lcdWriteText(3,0,"V-Dioden: %5u mV ", spannung_Messwert_mV);
                lcdWriteText(2,0,"Resultat: Diode     ");
                
            }
            if (spannung_Messwert_mV < 100)
            {
                rgbWrite(1023,0,1023);
                lcdWriteText(1,0,"Status  Kurzschluss ");
                lcdWriteText(2,0,"V= %5u mV        ", spannungWiederstand1_mV);
            }
            if (spannung_Messwert_mV > 4900)
            {
                rgbWrite(0,0,1023);
                lcdWriteText(1,0,"Status: Unterbruch  ");// ok
                //lcdLog("Unterbruch");
            }
        }
        else
        {
            rgbWrite(1023,1023,0);
            lcdWriteText(1,0,"Speisung nicht OK    "); // ok
            //Speisung unter 4.5 V

        }
        
        
        
        
        
        

        //Ausgabe------------------------------------------------------------------
        lcdWriteText(0,0,"LED-Tester V1.0 FR    " );
        
        
    }
}

