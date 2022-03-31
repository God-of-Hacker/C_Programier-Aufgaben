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
* Projekt  : Lautsprecher
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
* 16.03.2022   F.Russom      V1.0      Neuerstellung
*
\*********************************************************************************/

//uC-Board-Treiber hinzufügen
#include "ucBoardDriver.h"
#define IN_MASKE_POWER_TASTER 1
#define IN_MASKE_MUSTER_TASTER 2
#define IN_MASKE_SCHALTER_AKKU 0b11111111

#define MUSTER_0        (0xffff)
#define MUSTER_1        (0b1000000000000001)
#define MUSTER_2        (0b1110000000000111)
#define MUSTER_3        (0b0001111001111000)
#define MUSTER_4        (0b0000000110000000)
#define MUSTER_5        (0b1010101010101010)
#define MUSTER_6        (0b0101010101010101)
#define MUSTER_7        (0b1100110011001100)
#define MUSTER_8        (0b0011001100110011)
#define MUSTER_9        (0b1111000011110000)
#define MUSTER_10       (0b1111111100000000)
#define MUSTER_11       (0b1011001110001111)
#define MUSTER_12       (0b1111000111001101)
#define MUSTER_13       (0b0100000000000010)
#define MUSTER_14       (0b0010000000000100)
#define MUSTER_15       (0b0001111111111000)

#define SYSTEM_TICK_MS      10
#define ON_TIME             150
#define OFF_TIME            50
#define PERIOD              (ON_TIME+OFF_TIME)

#define SIZE 65535

#define IN_MASKE_AKKU_SPANNUNGSMESSUNG   0b11111111
#define BALKENGROESSE_LEDS               5
//#pragma gcc optimize 0

//typedef enum zustand_t {MUSTER_0, MUSTER_1, MUSTER_2, MUSTER_3, MUSTER_4, MUSTER_5, MUSTER_6,
// MUSTER_7, MUSTER_8, MUSTER_9, MUSTER_10, MUSTER_11, MUSTER_12, MUSTER_13, MUSTER_14, MUSTER_15 } zustand_t;

const uint16_t DECORDER[] = {MUSTER_0, MUSTER_1, MUSTER_2, MUSTER_3, MUSTER_4, MUSTER_5, MUSTER_6,
MUSTER_7, MUSTER_8, MUSTER_9, MUSTER_10, MUSTER_11, MUSTER_12, MUSTER_13, MUSTER_14, MUSTER_15};


//Hauptprogramm
int main(void)
{
    //Variablen
    uint8_t inTaster=0;
    uint8_t inTasterAlt=0;
    uint8_t power=0;
    uint8_t muster=0;
    uint8_t schalterAkku = 0;
    uint16_t leds = 0;
    uint8_t inDez =0;
    uint16_t i = 0;
    uint64_t timerBlink_ms = 0;
    uint8_t blink = 0;
    uint16_t spannung_mv = 0;
    
    //zustand_t state = AUS;
    
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        inTasterAlt = inTaster;
        inTaster = buttonReadAllPL();
        power = (inTasterAlt ^ inTaster) & inTaster & IN_MASKE_POWER_TASTER;
        muster = (inTasterAlt ^ inTaster) & inTaster & IN_MASKE_MUSTER_TASTER;
        
        schalterAkku = switchReadAll() & IN_MASKE_SCHALTER_AKKU;
        
        
        
        
        
        
        //Verarbeitung-------------------------------------------------------------
        
        if (power)
        {
            rgbWrite(0,100,0);

            
            if (muster)
            {
                i=i+1;
                muster = buttonReadAllPL() & IN_MASKE_MUSTER_TASTER;
                inDez = DECORDER[i];
                leds = inDez;
            }
            
            if (schalterAkku < 100)
            {
                blink = 1;
                leds = 0;
            }
            else 
            {
                blink = 0;             
            }
        }
        else
        {
            
        }
        //Ausgabe------------------------------------------------------------------
        lcdLog("%u",blink);
        lcdLog("%u",schalterAkku);
        if (blink)
        {
            if (timerBlink_ms >= ON_TIME)
            {
                rgbWrite(0,0,0);
            }
            if (timerBlink_ms >= PERIOD)
            {
                rgbWrite(100,0,0);
                timerBlink_ms = 0;
            }
        }
        else
        {
            timerBlink_ms = PERIOD;
        }
        ledWriteAll(leds);
        //Warten-------------------------------------------------------------------
        _delay_ms(SYSTEM_TICK_MS);
        timerBlink_ms = timerBlink_ms + SYSTEM_TICK_MS;
    }
}

