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
int main(void){
    //Variablen
    rgb_color leds[LED_COUNT];
    uint16_t i = 0;
    uint16_t j = 0;
    
    
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
        for (i=0; i<8; i=i+1)
        {
            leds[i+j] = (rgb_color){0,0,0};
        }               
        j = j+8;
        if (j > 248)
        {
            j = 0;            
        }
        for (i=0; i<8; i=i+1)
        {
            leds[i+j] = (rgb_color){0,50,0};
                _delay_ms(5);            
        }
        led_strip_write(leds);
        
        
        

        
        
        
        //Eingabe------------------------------------------------------------------
        
        //Verarbeitung-------------------------------------------------------------
        
        //Ausgabe------------------------------------------------------------------
        
    }
}

