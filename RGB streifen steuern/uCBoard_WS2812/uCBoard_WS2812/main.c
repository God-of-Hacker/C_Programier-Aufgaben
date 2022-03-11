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
#pragma GCC optimize 0
#include <stdlib.h>
#include "ucBoardDriver.h"
#include "ws2812.h"


led_strip_set_pixel(uint8_t posX, uint8_t posY, rgb_color color, rgb_color * ledArray)
{
    uint16_t valueArrayIndexSelected = 0;
    if (posX%2)
    {
        valueArrayIndexSelected = posX * 8 + 7 - posY;
    }
    else
    {
        valueArrayIndexSelected = posX * 8 + posY;
    }
    if (valueArrayIndexSelected<LED_COUNT)
    {
        ledArray[valueArrayIndexSelected] = color;
    }
}
//Hauptprogramm
int main(void){
    //Variablen
    rgb_color leds[LED_COUNT];
    uint16_t i = 0;
    uint16_t j = 0;
    uint16_t schreiben = 1;
    
    
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
            
                if ((j== 0 ||j== 8 || j==16 || j==32 ||j== 40 ||j== 48||j== 56)&&schreiben)
                {
                   uint8_t x=2;
                   
                   led_strip_set_pixel(x,0,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,1,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,2,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,3,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,4,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,5,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,6,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,7,(rgb_color){20,0,20},leds);
                   x=x+4;
                   led_strip_set_pixel(x,0,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,1,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,2,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,3,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,4,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,5,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,6,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,7,(rgb_color){20,0,20},leds);
                   
                   x=x+4;
                   led_strip_set_pixel(x,0,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,1,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,2,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,3,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,4,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,5,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,6,(rgb_color){20,0,20},leds);
                   led_strip_set_pixel(x,7,(rgb_color){20,0,20},leds);
                } 
                else
                {
                    leds[i+j] = (rgb_color){0,0,0};
                }
        }
        j = j+8;
       
        if (j > 248)
        {
            j = 0;
            schreiben=!schreiben;
        }
        for (i=0; i<8; i=i+1)
       {
           leds[i+j] = (rgb_color){50,0,0};
            
           _delay_ms(5);
        }
        
        led_strip_write(leds);
        //Eingabe------------------------------------------------------------------
        
        //Verarbeitung-------------------------------------------------------------
        
        //Ausgabe------------------------------------------------------------------
        
    }
}

