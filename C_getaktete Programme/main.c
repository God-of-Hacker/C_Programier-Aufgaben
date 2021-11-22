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
#define     START_POSITION      0
#define     LED_MUSTER          1
#define     SCHRITT_LAENGE      1
#define     SYSTEMTAKT          50


//Hauptprogramm
int main(void)
{
    //Variablen
    
   uint16_t i = 0;
   uint16_t schalter = 0;
   uint16_t LedAusgabe = 0;
   uint64_t stopuhr_ms = 0; 
    
    //Initialisieren
    initBoard(0);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
       schalter = switchReadAll() & 0b00000111;
        
        //Verarbeitung-------------------------------------------------------------
      
   if ( schalter == 0)
   {
      // if (stopuhr_ms <=100)
       {
           ledWriteAll(LED_MUSTER+i);
           i = i + SCHRITT_LAENGE;
           _delay_ms(100);
           if (i>=16)
           {
               i = START_POSITION;
           }
          // if (stopuhr_ms >=200)
          // {
          //     stopuhr_ms = 0;
          // }
       }
       
   } 
   else if (schalter == 1)
   {
      // if ((stopuhr_ms <=200))
       {
           ledWriteAll(LED_MUSTER+i);
            i = i + SCHRITT_LAENGE;
         _delay_ms(200);
         if (i>=16)
       {
           i = START_POSITION;
       }
        // if (stopuhr_ms >=500)
        // {
        //     stopuhr_ms = 0;
        // }
       }
       
   }
   else if (schalter == 2)
   {
       //if (stopuhr_ms <=500)
       {
           ledWriteAll(LED_MUSTER+i);
           i = i + SCHRITT_LAENGE;
          _delay_ms(500);
           if (i>=16)
           {
               i = START_POSITION;
           }
            // if (stopuhr_ms >=1000)
            // {
            //     stopuhr_ms = 0;
            // }
       }
       
   }
   else if (schalter == 3)
   {
    //   if (stopuhr_ms <=1000)
       {
           ledWriteAll(LED_MUSTER+i);
           i = i + SCHRITT_LAENGE;
           _delay_ms(1000);
           if (i>=16)
           {
               i = START_POSITION;
           }
            // if (stopuhr_ms >=1500)
            // {
            //     stopuhr_ms = 0;
            // }
       }
       
   }
 else if (schalter == 4)
 {
    // if (stopuhr_ms <=1500)
     {
         ledWriteAll(LED_MUSTER+i);
     i = i + SCHRITT_LAENGE;
     _delay_ms(1500);
     if (i>=16)
     {
         i = START_POSITION;
     } 
      // if (stopuhr_ms >=2000)
      // {
      //     stopuhr_ms = 0;
      // }
     }
    
 }
 else if (schalter == 5)
 {
    // if (stopuhr_ms <=2000)
     {
          ledWriteAll(LED_MUSTER+i);
          i = i + SCHRITT_LAENGE;
          _delay_ms(2000);
          if (i>=16)
          {
              i = START_POSITION;
          }
           // if (stopuhr_ms >=5000)
           // {
           //     stopuhr_ms = 0;
           // }
     }
    
 }     
 else if (schalter == 6)
 {
   // if (stopuhr_ms <=5000)
    {
       ledWriteAll(LED_MUSTER+i);
       i = i + SCHRITT_LAENGE;
       _delay_ms(5000);
       if (i>=16)
       {
           i = START_POSITION;
       } 
        // if (stopuhr_ms >=10000)
        // {
        //     stopuhr_ms = 0;
        // }
    }
     
 }    
 else if (schalter == 7)
 {
    // if (stopuhr_ms <=10000)
     {
         ledWriteAll(LED_MUSTER+i);
         i = i + SCHRITT_LAENGE;
         _delay_ms(10000);
         if (i>=16)
         {
             i = START_POSITION;
         }
          // if (stopuhr_ms >=11000)
          // {
          //     stopuhr_ms = 0;
          // }
     }
     
 }     
      
      
        //Ausgabe------------------------------------------------------------------
       
     
        
        //Warten------------------------------------------------------------------

        _delay_ms(SYSTEMTAKT);
        stopuhr_ms = stopuhr_ms + SYSTEMTAKT;
      
        
           
    }
}

