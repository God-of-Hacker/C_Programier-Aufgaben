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
#define     VERZOEGERUNG    100
#define     VERZOEGERUNG_1  200
#define     ON_LEDS         (0xffff)
#define     OFF_LEDS        0
#define     SYSTEMTAKT      10
#define     OFF              0
#define     IN_MASKE_EIN_AUS   (0x01)
//Hauptprogramm
int main(void)
{
    //Variablen
    uint16_t schalter = 0;
    uint16_t powerschalter = 0;
    uint16_t LedAusgabe = 0;
    uint64_t stopuhr_ms = 0;
    //Initialisieren
    initBoard(0);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------       
        schalter = switchReadAll();
        powerschalter = schalter & IN_MASKE_EIN_AUS ;
        
        //Verarbeitung-------------------------------------------------------------
     if (powerschalter)
     {
         if (stopuhr_ms <= VERZOEGERUNG)
         {
             LedAusgabe = ON_LEDS;
         }
         else if (stopuhr_ms >= VERZOEGERUNG)
         {
             LedAusgabe = OFF_LEDS;
             if (stopuhr_ms>= VERZOEGERUNG_1)
             {
                 stopuhr_ms = 0;
             }
             
         }
         
     } 
     else
     {
         LedAusgabe = OFF;
         stopuhr_ms = 0;
     }
       
      
     
        
        
        //Ausgabe------------------------------------------------------------------
        ledWriteAll(LedAusgabe);
        
       //Warten--------------------------------------------------------------------
       _delay_ms(SYSTEMTAKT);
       stopuhr_ms = stopuhr_ms + SYSTEMTAKT;

    }
}






