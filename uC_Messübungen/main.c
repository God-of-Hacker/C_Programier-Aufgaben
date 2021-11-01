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
      uint16_t adcEingang = 0;
      uint16_t adcLampe = 0;
      uint16_t uEingang_mv = 0;
      uint16_t uLampe_mv = 0;
      uint8_t inControlLoop = 0;
      //Initialisieren
      initBoard(1);
      pinInitX4PortL(2,OUTPUT);
      pinInitX4PortL(3,OUTPUT);//...-> init PL3 als Ausgang
      pinWriteX4PortL(3,0);//...-> pinWrite 0V an PL3      
      pinInitX4PortL(4,INPUT_PULLUP);
      pinInitX4PortF(0,INPUT_ADC);
      pinInitX4PortF(1,INPUT_ADC);
      
      //Unendlichschlaufe
      while(1)
      {
          //Eingabe------------------------------------------------------------------
          adcEingang = adcRead(ADC_00_X4_PORTF_BIT0);
          adcLampe = adcRead(ADC_01_X4_PORTF_BIT1);//...-> adcLampe einlesen
          
          inControlLoop = pinReadX4PortL(4);
          
          
          //Verarbeitung-------------------------------------------------------------
          
          //Ausgabe------------------------------------------------------------------
          
          if (adcEingang < 20)
          {
              lcdLog("Keine Speisung");
          } 
          else
          {
              lcdLog("speisung vorhandel");
//               lcdLog("Lampe ein und OK");
//               lcdLog("Lampe ein und defekt");
//               lcdLog("Keine Speisung");
//               lcdLog("keine Verbindung"          }
          
          }          
             
          
          
      }
  }

  //Lampe einschalten (FET leitet), Pin gibt 1 (5V) aus
  //pinWriteX4PortL(2,1);

  //Lampe ausschalten (FET sperrt), Pin gibt 0 (0V) aus
 // pinWriteX4PortL(2,0);

