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
#define     IN_SELBER_LADE_GERAET      (1<<1)
#define     IN_LADE_GERAET             (1<<2)
#define     IN_SPANNUNGSMESSUNG        0b11000000
#define     IN_OFFSET_SPANNUNG         6

#define     OFF                          0
#define     OUT_POWER_LED             0x01
#define     OUT_LADE_ANZEIGE_LED      0x02
#define     OUT_SPANNUG_0            0b00000000
#define     OUT_SPANNUG_1            0b00100000
#define     OUT_SPANNUG_2            0b01100000
#define     OUT_SPANNUG_3            0b11100000

#define SYSTEM_TICK_MS      10
#define ON_TIME             250
#define OFF_TIME            250
#define PERIOD              (ON_TIME+OFF_TIME)
//Hauptprogramm
int main(void)
{
    //Variablen
     uint8_t inSpannungsmessung = 0;
     uint8_t On = 0;
     uint8_t ger_Laden = 0;
     uint8_t selber_laden = 0;
     uint16_t powerLed = 0;
     uint16_t ladenAnzeige_Blinken = 0;
     uint16_t ladenAnzeige_Led = 0;
     uint16_t akkuanzeige_Led = 0;
     uint16_t outSpannungLed = 0; 
     uint64_t timerBlink_ms = 0;
    
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        inSpannungsmessung = switchReadAll() & IN_SPANNUNGSMESSUNG;
        inSpannungsmessung = inSpannungsmessung >> IN_OFFSET_SPANNUNG;
        ger_Laden = switchReadAll() & IN_LADE_GERAET;
        selber_laden = switchReadAll() & IN_SELBER_LADE_GERAET;
        On = switchReadAll() & ON_OFF_SCHALTER;
        
        //Verarbeitung-------------------------------------------------------------
       
       if (ger_Laden)
       {
           ladenAnzeige_Blinken = 1;
       }
       else
       {
           ladenAnzeige_Blinken = 0;
           ladenAnzeige_Led = OFF;
       }
       
        
        //Ausgabe------------------------------------------------------------------
         if (On)
         {
             powerLed = OUT_POWER_LED;
             
             if (ladenAnzeige_Blinken)
             {
                 if (timerBlink_ms >= ON_TIME)
                 {
                     ladenAnzeige_Led = OFF;
                 }
                 if (timerBlink_ms >= PERIOD)
                 {
                     ladenAnzeige_Led = IN_LADE_GERAET;
                     timerBlink_ms = 0;
                 }
             }
             else
             {
                 timerBlink_ms = PERIOD;
             }
             
         }
         else
         {
             powerLed = OFF;
         }
        
        
        
        ledWriteAll(powerLed | ladenAnzeige_Led);
        
     //Warten-------------------------------------------------------------------
     _delay_ms(SYSTEM_TICK_MS);
     timerBlink_ms = timerBlink_ms + SYSTEM_TICK_MS;
     
    }
}

