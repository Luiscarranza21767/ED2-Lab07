//***************************************************************************************************************************************
/* Librería para el uso de la pantalla ILI9341 en modo SPI
 * Basado en el código de martinayotte - https://www.stm32duino.com/viewtopic.php?t=637
 * Adaptación, migración y creación de nuevas funciones: Pablo Mazariegos y José Morales
 * Con ayuda de: José Guerra
 * Adaptación del ejemplo a librería: Luis Pablo Carranza
 * IE3027: Electrónica Digital 2 - 2023
 * Laboratorio 7 Luis Pablo Carranza
 */
//***************************************************************************************************************************************
#include <stdint.h>
#include <ili9341_SPI.h>
#include <SPI.h>
#include "bitmaps.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include <avr/pgmspace.h>

#define Player11 PUSH1
#define Player12 PUSH2
// El SPI es el 0
//MOSI va a PA_5
//MISO va a PA_4
//SCK va a PA_2

int buttonState1 = 1;
int buttonState2 = 1;
int xp1 = 0;
int comp = 0;                   // Variable para comparar posición del jugador 1
int comp2 = 35;                   // Variable para comparar posición del jugador 
//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  pinMode(Player11, INPUT_PULLUP);
  pinMode(Player12, INPUT_PULLUP);
  
  Serial.begin(9600);
  SPI.setModule(0);
  LCD_Init();
  LCD_Clear(0x00);

  // Fondo de noche difuminado
  FillRect(0, 0, 320, 60, 0x0000);
  FillRect(0, 60, 320, 40, 0x0021);
  FillRect(0, 100, 320, 30, 0x0022);
  FillRect(0, 130, 320, 20, 0x0063);
  FillRect(0, 150, 320, 20, 0x0084);
  FillRect(0, 170, 320, 20, 0x00a5);
  FillRect(0, 190, 320, 20, 0x00a5);

  // Edificio 1
  FillRect(40, 150, 70, 60, 0x0862);
  for(int y = 10; y < 50; y++){
    FillRect(41, 150 + y, 68, 4, 0x62e2);
    y += 11;  
  }

  // Edificio 2
  FillRect(125, 120, 60, 90, 0x10c3);
  for(int y = 10; y < 90; y++){
    FillRect(130, 120 + y, 50, 6, 0x7aa1);
    y += 13;  
  }
  FillRect(144, 120, 2, 90, 0x10c3);
  FillRect(164, 120, 2, 90, 0x10c3);
  FillRect(130, 115, 50, 5, 0x10c3);
  FillRect(135, 110, 40, 5, 0x10c3);
  FillRect(140, 100, 30, 10, 0x10c3);
  FillRect(144, 105, 22, 8, 0x7aa1);
  FillRect(150, 90, 10, 10, 0x10c3);

  // Edificio 3
  FillRect(200, 110, 60, 100, 0x1082);
  for(int y = 10; y < 90; y++){
    FillRect(205, 110 + y, 50, 6, 0x62e2);
    y += 13;  
  }
  FillRect(205, 100, 50, 10, 0x1082);
  FillRect(214, 110, 3, 100, 0x1082);
  FillRect(229, 110, 3, 100, 0x1082); 
  FillRect(244, 110, 3, 100, 0x1082);
  LCD_Print("Laboratorio 7", 53, 30, 2, 0x0000, 0xffff);
  LCD_Print("Luis Pablo Carranza", 10, 50, 2, 0x0000, 0xffff);
  LCD_Print("PRESS ANY BUTTON", 25, 215, 2, 0x0000, 0xffff);
  delay(1000);

  while(buttonState1 == HIGH & buttonState2 == HIGH){
    buttonState1 = digitalRead(Player11);
    buttonState2 = digitalRead(Player12);
    LCD_Print("                   ", 10, 215, 2, 0x0000, 0x0000);
    delay(500);
    LCD_Print("PRESS ANY BUTTON", 25, 215, 2, 0x0000, 0xffff);
    delay(500);
    };

  FillRect(0, 0, 320, 240, 0x0000);
  
  // Fondo de pantalla del juego
  FillRect(0,0, 320, 30, 0x869e);
  LCD_Sprite(0, 30, 41, 36, Background, 1, 0, 0, 0);
  LCD_Sprite(40, 30, 41, 36, Background, 1, 0, 1, 0);
  LCD_Sprite(80, 30, 41, 36, Background, 1, 0, 0, 0);
  LCD_Sprite(120, 30, 41, 36, Background, 1, 0, 1, 0);
  LCD_Sprite(160, 30, 41, 36, Background, 1, 0, 0, 0);
  LCD_Sprite(200, 30, 41, 36, Background, 1, 0, 1, 0);
  LCD_Sprite(240, 30, 41, 36, Background, 1, 0, 0, 0);
  LCD_Sprite(279, 30, 41, 36, Background, 1, 0, 1, 0);

    
  // Se debe llenar el piso en la longitud de la pantalla
  for(int x = 0; x <319; x++){
    LCD_Bitmap(x, 208, 30, 30, piso);
    x+=28;
   }

  FillRect(0, 66, 320, 144, 0x5bae);

  // Se llenan las montañas
  for(int x = 0; x<385; x++){
    LCD_Bitmap(x, 193, 35, 17, backpart1);
    x += 16;
    }

  // Dibujo del helicóptero
  LCD_Bitmap(20, 69, 60, 42, helicopter);
  // Dibujo del jugador 2
  LCD_Bitmap(250, 100, 33, 42, Metal2);

  LCD_Sprite(0, 173, 35, 37, Metal1, 5, 1, 0, 0);
  delay(1000);
}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {
  
  buttonState2 = digitalRead(Player12);
  buttonState1 = digitalRead(Player11);
  
  if(buttonState2 == LOW){
    while((buttonState2 == LOW) && (xp1<285)){   // Animación del personaje
      buttonState2 = digitalRead(Player12);
      delay(15);

      // Relleno de la parte que va dejando el jugador
      LCD_Sprite(xp1-1, 193, 1, 17, backpart1, 34, comp, 0, 0);
      if(comp <= 35){
        comp ++;
        }
      else{
        comp = 0;
        }
        
      // Animación del jugador
      int anim1 = (xp1/15)%2;
      LCD_Sprite(xp1, 173, 35, 37, Metal1, 5, anim1, 0, 0);
      xp1++;
      }

      // Si superó el límite redibuja la última parte
      if(xp1 >= 285){
        xp1 = 0;
        LCD_Bitmap(250, 193, 35, 17, backpart1);  // Rellena lo que se quedó al topar el borde
        LCD_Bitmap(285, 193, 35, 17, backpart1);
        FillRect(280, 173, 39, 20, 0x5bae);
        }
    
  }
  if(buttonState1 == LOW){
    while((buttonState1 == LOW) && (xp1>0)){   // Animación del personaje
      buttonState1 = digitalRead(Player11);
      delay(15);
      LCD_Sprite(xp1+35, 193, 1, 17, backpart1, 34, comp2, 0, 0);
      if(comp2 > 0){
        comp2--;
        }
      else{
        comp2 = 35;
        }
      

      int anim1 = (xp1/15)%2;
      LCD_Sprite(xp1, 173, 35, 37, Metal1,5, anim1, 1, 0);
      xp1--;
      }
      if(xp1 <= 35){
        xp1 = 284;
        LCD_Bitmap(0, 193, 35, 17, backpart1);  // Rellena lo que se quedó al topar el borde
        LCD_Bitmap(0, 193, 35, 17, backpart1);
        FillRect(0, 173, 39, 20, 0x5bae);
        }
    
    }
}
