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
#define     ON_OFF_SCHALTER         (1<<0)
#define     LADE_GERAET             (1<<1)
#define     AKKUZUSTAND             0b00111100  
#define     STROM                   0b10000000
#define     IN_OFFSET_AKKUZUSTAND   2

#define     OFF                              0
#define     ON_OFF_LED                       1
#define     OUT_POWER_LED                    1
#define     OUT__AKKU_LEER                   0             
#define     OUT__AKKU_FAST_LEER     0b10000000 
#define     OUT__AKKU__HALB_VOLL    0b11000000
#define     OUT__AKKU__FAST_VOLL    0b11100000
#define     OUT_LADE_POWERBANK_LED         0x4

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
        On = schalter & ON_OFF_SCHALTER;
        Laden = schalter & LADE_GERAET;      
        inBattStatus = switchReadAll() & AKKUZUSTAND;
        inBattStatus = inBattStatus >> IN_OFFSET_AKKUZUSTAND;
        strom = schalter & STROM;
        
        
        //Verarbeitung-------------------------------------------------------------
        if (On)
        {
            OffLed = ON_OFF_LED;
            
        } 
        else
        {
            OffLed = OFF;
        }
        if (Laden && On)
        {
            LadenLed = LADE_GERAET;
        } 
        else
        {
            LadenLed = OFF;
        }
                    
       if (On)
       {
           if (On && inBattStatus == 0)
         {
             outBattLed = OUT__AKKU_LEER;
         }
         else if (On && inBattStatus < 5)
         {
             outBattLed = OUT__AKKU_FAST_LEER;
             
         }
         
        else if  (On &&(inBattStatus >= 5)  && (inBattStatus <= 11 )  )
         {
             outBattLed =  OUT__AKKU__HALB_VOLL;
             
             
         }
         else if (On &&(inBattStatus >=  12) && (inBattStatus <= 15) )
         {
             
             outBattLed = OUT__AKKU__FAST_VOLL;
         }            

       } 
       else
       {
           outBattLed = OFF;
       }
         
        
       if ( strom && (inBattStatus < 15))
       {
         LedStrom =  OUT_LADE_POWERBANK_LED;
       }
      else 
      {
          LedStrom = OFF;
      
      }
          
                
        
        //Ausgabe------------------------------------------------------------------
        
        ledWriteAll(OffLed | LadenLed |outBattLed | LedStrom);
    }
}

      
    