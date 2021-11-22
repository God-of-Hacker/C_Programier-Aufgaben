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
     uint16_t Messstrom_uA = 0;
     uint16_t spannung_Messwert_mV = 0;
     uint16_t spannungWiederstand1_mV = 0;
     uint16_t Wiederstand_Ohm = 0;
     
    
     uint16_t spannung_mv = 0;
    
    
    //Initialisieren
    initBoard(1);
    lcdLogSetLinesToUse(2);
    lcdLog("Start");
    pinInitX4PortL(2,OUTPUT);
    
 
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------       
        spannung_mv = (uint32_t)adcRead(ADC_00_X4_PORTF_BIT0) * 5000 / 1023;
        spannung_Messwert_mV = (uint32_t)adcRead(ADC_01_X4_PORTF_BIT1)  * 5000 / 1023;
        if (spannung_mv >= spannung_Messwert_mV )
        {
            spannungWiederstand1_mV = spannung_mv - spannung_Messwert_mV;
        } 
        else
        {
            spannungWiederstand1_mV = 0;
        }
        
        Messstrom_uA = spannungWiederstand1_mV / 1  +1;
        Wiederstand_Ohm = (uint32_t)spannung_Messwert_mV * 1000 / Messstrom_uA;
        
        //Verarbeitung-------------------------------------------------------------
         if (spannung_mv > 4000)
         {
             //lcdWriteText(1,0,"Speisung OK         "); 
                        
             if ((Wiederstand_Ohm >= 100) && (Wiederstand_Ohm <= 10000))
             {
                 rgbWrite(0,1023,0);
                 lcdWriteText(1,0,"R= %05u Ohm        ", Wiederstand_Ohm);
                 //lcdLog("Wert in Bereich");
             }
             else
             {//kein gültiger Wert
                 if (Wiederstand_Ohm < 100)
                 {
                     rgbWrite(1023,0,0);
                     lcdWriteText(1,0,"Kurzschluss         ");
                     //lcdLog("Kurzschluss");
                 }
                 else
                 {
                     if (Wiederstand_Ohm > 10000)
                     {
                         rgbWrite(0,0,1023);
                         lcdWriteText(1,0,"Unterbruch          ");
                         //lcdLog("Unterbruch");
                     }
                 }
             }
             
         }
         else
         {
             rgbWrite(1023,0,1023);
             
             lcdWriteText(1,0,"Speisung nicht OK   "); 
             //lcdLog("Speisung kontrollieren...");

         }
        
        //Ausgabe------------------------------------------------------------------
         lcdWriteText(0,0,"R-Tester V1.0 FR    " );
       // lcdWriteZahl(0,0,spannungWiederstand1,4,0);
        //lcdWriteZahl(0,10,spannungWiederstand2,4,0);
         
         
        
         
    }
}

