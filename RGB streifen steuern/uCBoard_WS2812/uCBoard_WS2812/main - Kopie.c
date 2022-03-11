/*********************************************************************************\
*
* MMMMMMMMMMMMMMMMMM   SSSSSSSSSSSSSSSSSS   WWWW   WWWW   WWWW
* MMMMMMMMMMMMMMMMMM   SSSSSSSSSSSSSSSSSS   WWWW   WWWW   WWWW   MECHATRONIK
* MMMMMMMMMMMMMMMMMM   SSSSS                WWWW   WWWW   WWWW
* MMMM   MMMM   MMMM   SSSSSSSSSSSSSSSSSS   WWWW   WWWW   WWWW   SCHULE
* MMMM   MMMM   MMMM   SSSSSSSSSSSSSSSSSS   WWWW   WWWW   WWWW
* MMMM   MMMM   MMMM                SSSSS   WWWWWWWWWWWWWWWWWW   WINTERTHUR
* MMMM   MMMM   MMMM   SSSSSSSSSSSSSSSSSS   WWWWWWWWWWWWWWWWWW
* MMMM   MMMM   MMMM   SSSSSSSSSSSSSSSSSS   WWWWWWWWWWWWWWWWWW   www.msw.ch
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
#include <stdlib.h>
#include "ucBoardDriver.h"
#include "ws2812.h"

//Hauptprogramm
int main(void)
{
    //Variablen
    rgb_color leds[LED_COUNT];
    
    //Initialisieren
    initBoard();
    //Anfangswert für Zufallsgenerator setzen, gleicher Anfengswert ergibt die gleiche
    //.. Abfolge von Zufallszahlen. Wir lesen einen offenen (undefinierten) ADC ein
    //.. und setzen somit einen undefinierten (zufälligen) Anfangswert.
    srandom(adcRead(ADC_00_X4_PORTF_BIT0));
    
    //Unendlichschlaufe
    while(1)
    {
        //TEST---------------------------------------------------------------------
        leds[0].red = 255;
        leds[0].green = 0;
        leds[0].blue = 0;
        
        leds[0] = (rgb_color){255, 0, 0};
        
        leds[1].red = 0;
        leds[1].green = 255;
        leds[1].blue = 0;
        
        leds[2].red = 0;
        leds[2].green = 0;
        leds[2].blue = 255;
        
        leds[3].red = 0;
        leds[3].green = 0;
        leds[3].blue = 0;
        //random() % 256 ergibt eine Zufallszahl zwischen 0 und 255
        leds[4].red = random() % 256;
        leds[4].green = random() % 256;
        leds[4].blue = random() % 256;
        
        led_strip_write(leds);
        _delay_ms(500);
        
        leds[0].red = 0;
        leds[0].green = 0;
        leds[0].blue = 0;
        
        leds[1].red = 0;
        leds[1].green = 0;
        leds[1].blue = 0;
        
        leds[2].red = 0;
        leds[2].green = 0;
        leds[2].blue = 0;
        
        leds[3].red = 0;
        leds[3].green = 0;
        leds[3].blue = 0;
        
        leds[4].red = 0;
        leds[4].green = 0;
        leds[4].blue = 0;
        
        led_strip_write(leds);
        _delay_ms(500);
        
        //Eingabe------------------------------------------------------------------
        
        //Verarbeitung-------------------------------------------------------------
        
        //Ausgabe------------------------------------------------------------------
        
    }
}

