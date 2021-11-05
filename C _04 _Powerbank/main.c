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
        
        
        //Verarbeitung-------------------------------------------------------------
        if (On)
        {
            OffLed = OffLed | (0b00000001);
            
        } 
        else
        {
            OffLed = OffLed & ~(0x01);
        }
        if (Laden && On)
        {
            LadenLed = LadenLed | (0x02);
        } 
        else
        {
            LadenLed = LadenLed & ~(0x02);
        }
                    
         if (inBattStatus == 0)
         {
             outBattLed = outBattLed  & ~(0xff);
         }
         else if (inBattStatus < 5)
         {
             outBattLed = outBattLed | 0b10000000;
             
         }
         
        else if  ((inBattStatus >= 5)  && (inBattStatus <= 11 ))
         {
             outBattLed = outBattLed | 0b11000000;
             
             
         }
         else if ((inBattStatus >=  12) && (inBattStatus <= 15))
         {
             
             outBattLed = outBattLed | 0b11100000;
         }            
        
       
        
               
        
        //Ausgabe------------------------------------------------------------------
        
        ledWriteAll(OffLed | LadenLed |outBattLed);
    }
}

      
    