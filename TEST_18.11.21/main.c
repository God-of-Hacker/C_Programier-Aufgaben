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
* Hardware : uC-Board, ATmega2560v von Atmel§
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
#define     IN_MASKE_ON_OFF_SCHALTER        (0b00000001)
#define     IN_MASKE_TEMPERATURSENSOR       (0b00000010)
#define     IN_MASKE_SCHALTER_HEIZSTUFE     (0b00111100) 
#define     IN_OFFSET_HEIZSZUFE              2

#define     OUT_POWER_LED            (0b00000001)
#define     OUT_TEMP_LED             (0b00000010)
#define     OUT_ANZEIGE_HEIZSTUFE0   (0b00000000)
#define     OUT_ANZEIGE_HEIZSTUFE1   (0b00001000)
#define     OUT_ANZEIGE_HEIZSTUFE2   (0b00011000)
#define     OUT_ANZEIGE_HEIZSTUFE3   (0b00111000)
#define     OUT_ANZEIGE_HEIZSTUFE4   (0b01111000)
#define     OUT_ANZEIGE_HEIZSTUFE5   (0b11111000)
#define     OUT_GROSSE_KOCHFELD      (0b00000100)
#define     OFF                         0


//Hauptprogramm
int main(void)
{
    //Variablen
    uint16_t schalter = 0;
    uint16_t on_off_schalter = 0;
    uint16_t temperaturSchalter = 0;
    uint16_t drehknopf = 0;
    uint16_t on_off_Led = 0;
    uint16_t temperatur_Led = 0;
    uint16_t heizstufe_Led = 0;
    uint16_t grossherd_Led = 0;
    
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        schalter = switchReadAll();
        on_off_schalter = schalter & IN_MASKE_ON_OFF_SCHALTER;
        temperaturSchalter = schalter & IN_MASKE_TEMPERATURSENSOR;
        drehknopf = switchReadAll() & IN_MASKE_SCHALTER_HEIZSTUFE;
        drehknopf = drehknopf >> IN_OFFSET_HEIZSZUFE;
        //Verarbeitung-------------------------------------------------------------
        if (temperaturSchalter)
        {
            temperatur_Led = OFF;
        } 
        else
        {
            temperatur_Led = OUT_TEMP_LED;
        }
        if (on_off_schalter)
        {
            on_off_Led = OUT_POWER_LED;         
        } 
        else
        {
            on_off_Led = OFF;
        }
        if (on_off_schalter)
        {
          if (on_off_schalter && drehknopf <= 1)
          {
              heizstufe_Led = OUT_ANZEIGE_HEIZSTUFE0;
          } 
          else if (on_off_schalter && (drehknopf >= 2) && (drehknopf <= 3)) 
          {
              heizstufe_Led = OUT_ANZEIGE_HEIZSTUFE1;
          }
          else if (on_off_schalter && (drehknopf >= 4) && (drehknopf <= 5))
          {
              heizstufe_Led = OUT_ANZEIGE_HEIZSTUFE2;
          }
         else if (on_off_schalter && (drehknopf >= 6) && (drehknopf <= 7))
         {
             heizstufe_Led = OUT_ANZEIGE_HEIZSTUFE3;
         }
          else if (on_off_schalter && (drehknopf >= 8) && (drehknopf <= 9))
          {
              heizstufe_Led = OUT_ANZEIGE_HEIZSTUFE4;
          }
          else if (on_off_schalter && (drehknopf >= 10) && (drehknopf <= 15))
          {
              heizstufe_Led = OUT_ANZEIGE_HEIZSTUFE5;
             if (on_off_schalter && drehknopf >= 15)
             {
                 grossherd_Led = OUT_GROSSE_KOCHFELD;
             } 
          }
        } 
        
        else
        {
            heizstufe_Led = OFF;
            grossherd_Led = OFF;
        }
        
        
        //Ausgabe------------------------------------------------------------------
        ledWriteAll(on_off_Led | temperatur_Led | heizstufe_Led | grossherd_Led);
        
    }
}

