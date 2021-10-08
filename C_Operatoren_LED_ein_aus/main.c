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
      uint16_t schalter = 0;
      uint8_t power = 0;
      uint16_t leds = 0;     
     uint16_t power1 = 0;
      uint16_t schalter1 = 0;
      
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------        
        schalter = switchReadAll();
        schalter1 = switchReadAll();
        power = schalter & 0b00000001;             //für eine schalter steüern PCO bis PC7
        power1 = schalter1 & 0b00000010;
       
      
        //Verarbeitung-------------------------------------------------------------
        if (power )
        {   leds = leds | 0x00ff;
              
        } 
        else
        {  leds = leds & 0xff00;
        }             
         if (power1)
         {   leds = leds & 0x00ff;
         } 
         else
         { leds = leds | 0xff00;
         }
       
            
        //Ausgabe---------------------------------------------------------------
        ledWriteAll(leds);
        
       
      
      
        
        
      
        
        
        
        
        
        
    }
}

