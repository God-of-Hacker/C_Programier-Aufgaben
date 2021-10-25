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
    uint8_t leds = 0;
    uint8_t S7 = 0;
    uint8_t S6 = 0;
    uint8_t S5 = 0;
    uint8_t S4 = 0;
    uint8_t S3 = 0;
    uint8_t S2 = 0;
    uint8_t S1 = 0;  
    uint8_t S0 = 0; 
    uint8_t S1_3 = 0;
    uint8_t S0_7 = 0;
    uint8_t S4_6 = 0;
    uint16_t LedAusgabe = 0;
    
    
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        schalter = switchReadAll();
        S7 = schalter & 0b10000000;
        S6 = schalter & 0b01000000;
        S5 = schalter & 0b00100000;
        S4 = schalter & 0b00010000;
        S3 = schalter & 0b00001000;
        S2 = schalter & 0b00000100;
        S1 = schalter & 0b00000010;
        S0 = schalter & 0b00000001;
        S1_3 = switchReadAll() & 0b00001110;
        S1_3 = S1_3 >> 1;
        S0_7 = switchReadAll() & 0b11111111;
        S4_6 = switchReadAll() & 0b01110000;
        S4_6 = S4_6 >>4;
       
        
        //Verarbeitung-------------------------------------------------------------
        if (S7)
        {   leds = leds | 0x01;         //Aufgabe 1.1 Schalter 7 wird an Led 0 angezeigt.
        } 
        else
        {   leds = leds & 0xfffe;
        }
       if (S7 && S6 && !S5)
       {    leds = leds | (1<<1);       //Aufgabe 1.2: Wenn Schalter 7 und Schalter 6 aber nicht Schalter 5 aktiv ist, leuchtet LED 1, sonst nicht.
       } 
       else
       {    leds = leds & ~(1<<1);
       }
       if (S6 && S5 && S4)
       {    leds = leds & ~(1<<2);      //Aufgabe 1.3: Wenn Schalter 6, Schalter 5 oder Schalter 4 inaktiv ist, leuchtet LED 2, sonst nicht.
       } 
       else
       {    leds = leds | (1<<2);
       }
      if (S1_3 % 2)
      {     leds = leds | (1<<3);       //Aufgabe 1.4: Wenn der Wert von Schalter 1-3(Schalter 1 ist LSB, Schalter 3 ist MSB)
                                        //ungeradeist, leuchtet LED 3, sonst nicht.
      } 
      else
      {     leds = leds & ~(1<<3);      
      }
        if (S1_3 % 2)
        {     leds = leds & ~ (1<<4);        //Aufgabe 1.5: Wenn der Wert von Schalter 1-3(Schalter 1 ist LSB, Schalter 3 ist MSB)
                                            //geradeist, leuchtet LED 4, sonst nicht
        }
        else
        {leds = leds |(1<<4);
        }
         
      if (S0_7 % 17 )
      {     leds = leds & ~(0x20);       //Aufgabe 1.6: Wenn der Wert an Schalter 0 bis 7 (Schalter 0 ist LSB, 
                                        //Schalter 7 ist MSB) durch 17 teilbar ist, leuchtet LED 5, sonst nicht.
      } 
      else
      {     leds = leds | (0x20);       //Aufgabe 1.7: der Wert der Schalter 1-3 grösser ist als der Wert an Schalter4-6
      }                                 // leuchtet Led 6, sonst nicht.
      if (S1_3 > S4_6)
      {     leds = leds | (0x40);
      } 
      else
      {     leds = leds & ~(0x40);
      }                                 //Falls die beiden Werte gleich gross sind, leuchtet Led 7, sonst nicht.
      if (S1_3 == S4_6)
      {     leds = leds | (0x80);
      }
      else
      {     leds = leds & ~(0x80);
      }
      
      if (S0)
      {     LedAusgabe = 0xffff;       //Aufgabe 1.8:Wenn Schalter 0 aktiv ist,soll die gesamte Ausgabe invertiert werden.
            
      } 
      else
      {    LedAusgabe = 0;
      }
    
      
        //Ausgabe------------------------------------------------------------------
        ledWriteAll(leds ^ LedAusgabe);
        
        
        
    }
}


