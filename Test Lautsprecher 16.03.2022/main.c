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
#define IN_MASKE_POWER_TASTER 0x01
#define IN_MASKE_MUSTER_TASTER 0x02
#define IN_MASKE_SCHALTER_AKKU 0b11111111
#define AKKU_LEER              3300
#define AKKU_VOLL              3700

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

#define PROGRAMMTAKT_MS      10
#define ON_TIME             150
#define OFF_TIME            50
#define PERIODE              (ON_TIME+OFF_TIME)




//#pragma gcc optimize 0


typedef enum zustand_t {AUS, AN, AKKU, MUSTER_WECHSELN} zustand_t;

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
    uint16_t spannung_mV = 0;
    
    uint8_t rot = 0;
    uint8_t gruen = 0;
    uint8_t blau = 0;
    
    zustand_t state = AUS;
    
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
        spannung_mV = (uint32_t) schalterAkku * 5000 / 255;
        
        //Verarbeitung-------------------------------------------------------------
        
        switch (state)
        {
            case AUS:
            lcdLog("Aus                 ");
            leds=0;// LED's aus;
            blink=0;
            rot=0;
            gruen=0;
            blau=0;
            if (power)
            {
                state = AN ;//setzen;
            }

            break;
            case AN:
            lcdLog("An                  ");
            rot=0;
            gruen=250;
            blau=0;// Grün leuchtet;
            if (muster)
            {
                state = MUSTER_WECHSELN;// setzen;
            }
            
            if (spannung_mV < AKKU_LEER)
            {
                state = AKKU ;
            }
            if (power)
            {
                state = AUS ;//setzen;
            }
            break;
            case AKKU:
            lcdLog("Akku Leer           ");
            blink=1;
            leds=0;
            if (spannung_mV > AKKU_VOLL)
            {
                state = AUS ;
            }
            if (power)
            {
                state = AUS ;//setzen;
            }
            
            break;
            case MUSTER_WECHSELN:
            lcdLog("Muster Wechseln     ");
            if (spannung_mV < AKKU_LEER)
            {
                state = AKKU ;
            }
            if (power)
            {
                state = AUS ;//setzen;
            }
            leds = DECORDER [i]; //Array
            if (muster)
            {
                i = i+1;
                if (i>16)
                {
                    i=0;
                }
            }
            break;
            default:
            ;
        }
        //Ausgabe------------------------------------------------------------------
        if (blink)
        {
            if (timerBlink_ms >= ON_TIME)
            {
                rot=0;
                gruen=0;
                blau=0;
            }
            if (timerBlink_ms >= PERIODE)
            {
                rot=250;
                gruen=0;
                blau=0;//RGB Rot blinkt;
                timerBlink_ms = 0;
            }
        }
        else
        {
            timerBlink_ms = PERIODE;
        }
        ledWriteAll(leds);
        rgbWrite(rot,gruen,blau);
        //Warten-------------------------------------------------------------------
        timerBlink_ms = timerBlink_ms + PROGRAMMTAKT_MS;
        _delay_ms(PROGRAMMTAKT_MS);
    }
}

