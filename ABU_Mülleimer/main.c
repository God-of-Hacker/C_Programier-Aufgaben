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
#define TASTER_ABFALL                       (0b00000001)
#define TASTER_ABFALL_DRINNEN               (0b00000010)

#define SCHALTER_INFRAROT                   (0b00000001)
#define SCHALTER_TON                        (0b00000010)
#define SCHALTER_LEITFEAHIGKEIT             (0b00000100)
#define LED_TONNE_AUF                       (0x01)
#define LED_TONNE_ZU                        (0x02)

#define SYSTEMTACKT_MS                     10

typedef enum Zustand_t {TONNE_ZU, TONNE_AUF, PET, ALU, MUELL_ANALYSIEREN, MUELL} zustand_t;
//Hauptprogramm
int main(void)
{
    //Variablen
    uint8_t inTaster=0;
    uint8_t inTasterAlt=0;
    uint8_t posFlanken=0;
    
    uint16_t schalter_infrarot =0;
    uint16_t schalter_ton =0;
    uint16_t schalter_leitfaehigkeit =0;
    uint16_t taster_Abfall =0;
    uint16_t taster_Abfall_drinnen =0;
    

    uint16_t led_Tonne_auf =0;
    uint16_t led_Tonne_zu =0;
    uint16_t rot = 0;
    uint16_t blau = 0;
    uint16_t gruen = 0;
    
    zustand_t state = TONNE_AUF;
    
    
    
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        inTasterAlt = inTaster;                                                                                 //Alter Buttonzustand
        inTaster = buttonReadAllPL();                                                                           //Neuer Buttonzustand
        posFlanken = (inTaster ^ inTasterAlt) & inTaster;
        taster_Abfall = posFlanken & TASTER_ABFALL;
        taster_Abfall_drinnen = posFlanken & TASTER_ABFALL_DRINNEN;
        schalter_infrarot = switchReadAll() & SCHALTER_INFRAROT;
        schalter_leitfaehigkeit = switchReadAll() & SCHALTER_LEITFEAHIGKEIT;
        schalter_ton = switchReadAll() & SCHALTER_TON;
        //Verarbeitung-------------------------------------------------------------
        switch (state)
        {
            case TONNE_AUF:
            led_Tonne_auf = TONNE_AUF;
            led_Tonne_zu = 0;
            rot = 0;
            gruen = 0;
            blau = 0;
            lcdLog("Tonne Auf");
            if (taster_Abfall)
            {
                state = TONNE_ZU; //Zustand auf Tonne zu setzen;
            }
            break;
            case TONNE_ZU:
            led_Tonne_zu = TONNE_ZU;
            led_Tonne_auf = 0;
            lcdLog("Muell drinnen");
            _delay_ms(1000);
            lcdLog("Tonne Zu");
            _delay_ms(100);
            state = MUELL_ANALYSIEREN;
            break;
            case MUELL_ANALYSIEREN:
            led_Tonne_zu = 0;
            led_Tonne_auf = 0;
            lcdLog("Muell analysieren");

            if (schalter_infrarot && !schalter_leitfaehigkeit)
            {
                _delay_ms(1500);
                state = PET; // auf PET setzen;
            }
            else if (!schalter_leitfaehigkeit)
            {                
                _delay_ms(1500);
                state = PET; //PET setzen;
            }
            
            else if (!schalter_leitfaehigkeit && schalter_ton)
            {
                _delay_ms(1500);
                state = PET; //auf PET setzen;
            }
            else if (schalter_infrarot && schalter_ton)
             {
                 _delay_ms(1500);
                 state = PET; //auf PET setzen;
             }
             else
             {
                 _delay_ms(1500);
                 state = ALU; //auf Alu setzen;
             }
            break;
            case PET:
            led_Tonne_zu = 0;
            led_Tonne_auf = 0;           
            rot = 0;
            gruen = 250;
            blau = 0;
            rgbWrite(rot, gruen, blau);
            lcdLog("PET erkannt");
            _delay_ms(2500);
            state = TONNE_AUF;
            if (taster_Abfall_drinnen)
            {
                state = TONNE_AUF; //auf Tonne auf setzen;
                
            }
            break;
            case ALU:
            rot = 0;
            gruen = 0;
            blau = 250;
            led_Tonne_zu = 0;
            led_Tonne_auf = 0;
            rgbWrite(rot, gruen, blau);
            lcdLog("Alu erkannt");
            _delay_ms(2500);
            state = TONNE_AUF;
            if (taster_Abfall_drinnen)
            {
                state = TONNE_AUF; //Zustand auf Tonne auf setzen;
                
            }
            break;
            default:
            ;
        }
        //Ausgabe------------------------------------------------------------------
        ledWriteAll( led_Tonne_auf | led_Tonne_zu );
        rgbWrite(rot, gruen, blau);
        _delay_ms(SYSTEMTACKT_MS);
    }
}

