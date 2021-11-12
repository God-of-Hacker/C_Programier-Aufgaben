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
#define     VERZOEGERUNG_ms   10
#define     SCHRITT_LAENGE      1
#define     LED_MUSTER_R   0x8000
#define     LED_MUSTER_L        1
#define     ANZAHL_LEDS        16
#define     START_POSITION      0










//Hauptprogramm
int main(void)
{
    //Variablen
    
    uint16_t i = START_POSITION;
    uint16_t goleft = 1;
    uint16_t goright = 0;
    uint16_t j = 0;
    
   
    
    
    //Initialisieren
    initBoard(0);
    
    //Unendlichschlaufe
    while(1)
    {
       if (goleft)
       {
            
            ledWriteAll(LED_MUSTER_L<<i);
            _delay_ms(VERZOEGERUNG_ms);
            i = i + SCHRITT_LAENGE;
            if (i>=ANZAHL_LEDS)
            {
                goleft = START_POSITION;
                goright = 1;
               
            }
            
       } 
      if (goright)
      {
           ledWriteAll(LED_MUSTER_R>>j);
           _delay_ms(VERZOEGERUNG_ms);
           j = j + SCHRITT_LAENGE;
           if (j>=ANZAHL_LEDS)
           {
               goleft = 1;
               goright = START_POSITION;
               j = START_POSITION;
               i = START_POSITION;
           }
          
      }
       
     
     
        
            
          
             
           
         
      
     
     
         
         
      }       
         
     }
        
       
       
  
        
   


