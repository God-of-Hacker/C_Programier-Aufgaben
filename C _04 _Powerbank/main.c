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
    uint16_t On = 0;
    uint16_t OffLed = 0;
    uint16_t Laden = 0;
    uint16_t LadenLed = 0; 
    uint8_t inBattStatus = 0;
    uint16_t outBattLed=0;
    uint16_t strom = 0;
    uint16_t LedStrom = 0;
    
    
    
   
    
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        schalter = switchReadAll();
        On = schalter & (1<<0);
        Laden = schalter & (1<<1);      
        inBattStatus = switchReadAll() & 0b00111100;
        inBattStatus = inBattStatus >> 2;
        strom = schalter & 0b10000000;
        
        
        //Verarbeitung-------------------------------------------------------------
        if (On)
        {
            OffLed = 1;
            
        } 
        else
        {
            OffLed = 0;
        }
        if (Laden && On)
        {
            LadenLed = 0x02;
        } 
        else
        {
            LadenLed = 0;
        }
                    
       if (On)
       {
           if (On && inBattStatus == 0)
         {
             outBattLed = 0;
         }
         else if (On && inBattStatus < 5)
         {
             outBattLed = 0b10000000;
             
         }
         
        else if  (On &&(inBattStatus >= 5)  && (inBattStatus <= 11 )  )
         {
             outBattLed =  0b11000000;
             
             
         }
         else if (On &&(inBattStatus >=  12) && (inBattStatus <= 15) )
         {
             
             outBattLed = 0b11100000;
         }            

       } 
       else
       {
           outBattLed = 0;
       }
         
        
       if ( strom && (inBattStatus < 15))
       {
         LedStrom =  0x4;
       }
      else 
      {
          LedStrom = 0;
      
      }
          
                
        
        //Ausgabe------------------------------------------------------------------
        
        ledWriteAll(OffLed | LadenLed |outBattLed | LedStrom);
    }
}

      
    