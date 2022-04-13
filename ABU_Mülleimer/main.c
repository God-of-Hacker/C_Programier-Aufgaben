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
* dd.mm.yyyy  F. Russom      V1.0      Neuerstellung
*
\*********************************************************************************/

//uC-Board-Treiber hinzufügen
#include "ucBoardDriver.h"
#define SCHALTER_MUELL_OFFEN                (0b00000001)
#define SCHALTER_MUELL_REINWERFEN           (0b00000010)
#define SCHALTER_PET                        (0b00000100)
#define SCHALTER_ALU                        (0b00001000)
#define SCHALTER_MUELL                      (0b00010000)
#define SCHALTER_PET_ALU_MUELL_DRINNEN      (0b00100000)

#define LED_MUELL_OFFEN                (0b00000001)
#define LED_MUELL_ZU                   (0b00000010)
#define LED_PET                        (0b00000100)
#define LED_ALU                        (0b00001000)
#define LED_MUELL                      (0b00010000)

typedef enum Zustand_t {STARTUP, MUELLEIMER_AUF, MUELLEIMER_ZU, PET, ALU, MUELL} zustand_t;
//Hauptprogramm
int main(void)
{
    //Variablen
    uint16_t schalter_Muell_offen =0;
    uint16_t schalter_Muell_reinwerfen =0;
    uint16_t schalter_Pet =0;
    uint16_t schalter_Alu =0;
    uint16_t schalter_Muell =0;
    uint16_t schalter_Pet_Alu_Muell_drinnen =0;
    
    uint16_t led_Muell_offen =0;
    uint16_t led_Muell_zu =0;
    uint16_t led_Pet =0;
    uint16_t led_Alu =0;
    uint16_t led_Muell =0;
    
    
    zustand_t state = STARTUP;
    
    
    
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        schalter_Muell_offen = switchReadAll() & SCHALTER_MUELL_OFFEN;
        schalter_Muell_reinwerfen = switchReadAll() & SCHALTER_MUELL_REINWERFEN;
        schalter_Pet = switchReadAll() & SCHALTER_PET;
        schalter_Alu = switchReadAll() & SCHALTER_ALU;
        schalter_Muell = switchReadAll() & SCHALTER_MUELL;
        schalter_Pet_Alu_Muell_drinnen = switchReadAll() & SCHALTER_PET_ALU_MUELL_DRINNEN;
        //Verarbeitung-------------------------------------------------------------
        switch (state)
        {
            case STARTUP:
            lcdWriteText(0,0,"STARTUP   ");

            if (schalter_Muell_offen)
            {
                
               state =  MUELLEIMER_AUF;//Zustand auf Mülleimer auf setzen;
            }
            break;
            case MUELLEIMER_AUF:
            lcdWriteText(0,0,"MUELLEIMER AUF");
            led_Muell_offen = LED_MUELL_OFFEN;// LED Mülleimer auf;
            led_Muell_zu =0;
            led_Pet =0;
            led_Alu =0;
            led_Muell =0;

            if (schalter_Muell_reinwerfen && schalter_Muell_offen )
            {
               state = MUELLEIMER_ZU;// Zustand auf Mülleimer zu setzen;
            }
            break;
            case MUELLEIMER_ZU:
            lcdWriteText(0,0,"MUELLEIMER ZU");
            led_Muell_zu = LED_MUELL_ZU;// Mülleimer zu;
            led_Muell_offen =0;
            led_Pet =0;
            led_Alu =0;
            led_Muell =0;

            if (schalter_Pet)
            {
                state = PET;// auf PET setzen;
            }
            if (schalter_Alu)
            {
                state = ALU;//Zustand auf Alu setzen;
            }
            if (schalter_Muell)
            {
                state = MUELL;//Zustand auf Müll setzen;
            }
            break;
            case PET:
            lcdWriteText(0,0,"PET  ERKNAT   ");
            if (schalter_Pet_Alu_Muell_drinnen)
            {
                state = STARTUP;
            }
            
            led_Pet = LED_PET;
            led_Muell_offen =0;
            led_Muell_zu =0;
            led_Alu =0;
            led_Muell =0;

            lcdWriteText(1,0,"Eimer Pet auf");
            lcdWriteText(2,0,"Eimer Alu zu");
            lcdWriteText(3,0,"Eimer Müll zu");

            break;
            case ALU:
            lcdWriteText(0,0,"ALU ERKNAT  ");
            if (schalter_Pet_Alu_Muell_drinnen)
            {
                state = STARTUP;
            }
            led_Alu = LED_ALU;
            led_Muell_offen =0;
            led_Muell_zu =0;
            led_Pet =0;
            led_Muell =0;

            lcdWriteText(1,0,"Eimer Alu auf");
            lcdWriteText(2,0,"Eimer Pet zu");
            lcdWriteText(3,0,"Eimer Müll zu");
            break;
            case MUELL:
            lcdWriteText(0,0,"MUELL ERKNAT   ");
            if (schalter_Pet_Alu_Muell_drinnen)
            {
                state = STARTUP;
            }
            led_Muell = LED_MUELL;
            led_Muell_offen =0;
            led_Muell_zu =0;
            led_Pet =0;
            led_Alu =0;

            lcdWriteText(1,0,"Eimer Müll auf");
            lcdWriteText(2,0,"Eimer Alu zu");
            lcdWriteText(3,0,"Eimer Pet zu");
            break;
            default:
            ;
        }
        //Ausgabe------------------------------------------------------------------
        ledWriteAll(led_Muell_offen | led_Muell_zu | led_Pet | led_Alu | led_Muell);
    }
}

