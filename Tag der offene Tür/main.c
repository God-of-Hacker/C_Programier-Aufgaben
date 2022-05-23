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
#define IN_MASKE_BUTTON_START   1
#define IN_MASKE_BUTTON_RESET   2

#define TIME_START              700
#define PROGRAMMTAKT_MS         10

#define X                       0
#define Y                       1

typedef enum Zustand_t {START, SNAKE,JOYSTICK} zustand_t;

void led_stripe_set_pixel(uint16_t posX, uint16_t posY, rgb_color color, rgb_color * ledArray)
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

int main(void)
{
    //Variablen
    uint8_t snakexy [256] [2];
    uint16_t snakeLenght=1;
    
    rgb_color fruchtArray[5] =
    {
        (rgb_color){25, 15, 0},  //orange
        (rgb_color){25, 25, 0},  //gelb
        (rgb_color){20, 0, 20},  //violett
        (rgb_color){15, 8, 0},   //braun
        (rgb_color){25, 25, 25}  //weiss
    };
    
    rgb_color leds[LED_COUNT];
    
    uint8_t inTaster        = 0;
    uint8_t inTasterAlt     = 0;
    uint8_t posFlanken      = 0;
    uint8_t buttonStart     = 0;
    uint8_t reset           = 0;
    
    uint32_t joystickY      = 0;
    uint32_t joystickX      = 0;
    int16_t horizontal     = 15;
    int16_t vertikal       = 4;
    uint8_t rechts          = 0;
    uint8_t links           = 0;
    uint8_t oben            = 0;
    uint8_t unten           = 0;
    uint32_t neueFrucht    = 0;
    
    uint64_t timerStart_ms  = 0;
    uint64_t timerBewegung  = 0;
    
    
    uint16_t randomx        = 0 ;
    uint16_t randomy        = 0 ;
    
    uint64_t punktzahl      = 0;
    
    zustand_t state         = SNAKE;
    
    //Initialisieren
    
    initBoard(0);
    
    //Anfangswert für Zufallsgenerator setzen, gleicher Anfengswert ergibt die gleiche
    //.. Abfolge von Zufallszahlen. Wir lesen einen offenen (undefinierten) ADC ein
    //.. und setzen somit einen undefinierten (zufälligen) Anfangswert.
    
    srandom(adcRead(ADC_00_X4_PORTF_BIT0));
    
    for (uint16_t i=0;i<LED_COUNT;i++)
    {
        snakexy[i][X]=0;
        snakexy[i][Y]=0;
    }
    
    //Unendlichschlaufe
    
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        
        if (neueFrucht)
        {
            randomx = random()%31;
            randomy = random()%7;
            while (randomx==horizontal && randomy==vertikal)
            {
                neueFrucht =1;
                
            }
        }
        
        
        inTasterAlt = inTaster;                                                                                 //Alter Buttonzustand
        inTaster    = buttonReadAllPL();                                                                           //Neuer Buttonzustand
        posFlanken  = (inTaster ^ inTasterAlt) & inTaster;                                                       //Positive Flanken aus altem und neuem Buttonzustand auslessen
        buttonStart = posFlanken & IN_MASKE_BUTTON_START;
        reset = posFlanken & IN_MASKE_BUTTON_RESET;
        joystickY   = adcRead(ADC_11_JOYSTICK_Y);
        joystickX   = adcRead(ADC_10_JOYSTICK_X);
        
        //Verarbeitung-------------------------------------------------------------
        if (reset)
        {
            state = SNAKE;
        }
        if (joystickX >= 850)
        {
            rechts =1;
            links =0;
            unten =0;
            oben =0;
        }
        else if (joystickX <= 200)
        {
            links=1;
            rechts=0;
            unten =0;
            oben =0;
        }
        if (joystickY <= 200)
        {
            oben = 1;
            unten = 0;
            links=0;
            rechts=0;
        }
        else if (joystickY >= 850)
        {
            oben = 0;
            unten = 1;
            links=0;
            rechts=0;
        }

        switch (state)
        {
            case SNAKE:
            timerBewegung =0;
            timerStart_ms =0;
            for (uint16_t i=0; i<LED_COUNT; i++)
            {
                leds[i] = (rgb_color){0,0,0};
            }
            punktzahl=0;
            lcdWriteText(1,3,"    SNAKE  ");
            if (buttonStart)
            {
                state = START;
            }
            break;
            
            case START:
            neueFrucht = 1;
            timerBewegung =0;
            lcdWriteText(1,7,"START");
            if (timerStart_ms >= TIME_START)
            {
                state = JOYSTICK;
            }
            break;
            
            case JOYSTICK:
            timerStart_ms = 0;
            
            lcdWriteText(1,7,"       ");
            led_stripe_set_pixel(horizontal,vertikal,(rgb_color){0,0,0},leds);
            if (links&&!(timerBewegung%100))
            {
                horizontal = horizontal -1;
                if ( horizontal < 0)
                {
                    horizontal = 31;
                }
            }
            else if (rechts&&!(timerBewegung%100))
            {
                horizontal = horizontal +1;
                if ( horizontal >= 32)
                {
                    horizontal = 0;
                }
            }
            if (oben&&!(timerBewegung%100))
            {
                vertikal = vertikal -1;
                if ( vertikal < 0)
                {
                    vertikal = 7;
                }
            }
            else if (unten&&!(timerBewegung%100))
            {
                vertikal = vertikal +1;
                if ( vertikal >= 8)
                {
                    vertikal = 0;
                }
            }
            led_stripe_set_pixel(horizontal,vertikal,(rgb_color){0,50,5},leds);
            
            if (neueFrucht)
            {
                led_stripe_set_pixel(randomx,randomy,fruchtArray[random()%5],leds);
                neueFrucht =0;
            }
            
            if (((randomx == horizontal)&&(randomy == vertikal))&& !(timerBewegung%100))
            {
                punktzahl = punktzahl +1;
                neueFrucht =1;
            }
            lcdWriteText(1,3,"Score: %4u",punktzahl );
            
            
            break;
            
            default:
            
            //Wird nicht erreicht
            
            break;
        }
        //
        //Ausgabe------------------------------------------------------------------
        led_strip_write(leds);
        timerStart_ms = timerStart_ms + PROGRAMMTAKT_MS;
        timerBewegung = timerBewegung + PROGRAMMTAKT_MS;
        _delay_ms(PROGRAMMTAKT_MS);
        
        
    }
}

