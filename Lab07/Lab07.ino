//***************************************************************************************************************************************
/* Librería para el uso de la pantalla ILI9341 en modo SPI
 * Basado en el código de martinayotte - https://www.stm32duino.com/viewtopic.php?t=637
 * Adaptación, migración y creación de nuevas funciones: Pablo Mazariegos y José Morales
 * Con ayuda de: José Guerra
 * Adaptación del ejemplo a librería: Luis Pablo Carranza
 * IE3027: Electrónica Digital 2 - 2023
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

#include <SD.h>
// El SPI es el 0
//MOSI va a PA_5
//MISO va a PA_4
//SCK va a PA_2

File myFile;
int chartohex(int val);
//***************************************************************************************************************************************
// Inicialización
//***************************************************************************************************************************************
void setup() {
  
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  SPI.setModule(0);
  Serial.println("Hola amigos");
  LCD_Init();
  LCD_Clear(0x00);

  // Fondo de noche
  FillRect(0, 0, 320, 60, 0x0000);
  FillRect(0, 60, 320, 40, 0x0021);
  FillRect(0, 100, 320, 30, 0x0022);
  FillRect(0, 130, 320, 20, 0x0063);
  FillRect(0, 150, 320, 20, 0x0084);
  FillRect(0, 170, 320, 20, 0x00a5);
  FillRect(0, 190, 320, 20, 0x00a5);

  // Edificio
  
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
  delay(1500);

  FillRect(0,0, 320, 30, 0x869e);
  LCD_Sprite(0, 30, 41, 36, Background, 1, 0, 0, 0);
  LCD_Sprite(40, 30, 41, 36, Background, 1, 0, 1, 0);
  LCD_Sprite(80, 30, 41, 36, Background, 1, 0, 0, 0);
  LCD_Sprite(120, 30, 41, 36, Background, 1, 0, 1, 0);
  LCD_Sprite(160, 30, 41, 36, Background, 1, 0, 0, 0);
  LCD_Sprite(200, 30, 41, 36, Background, 1, 0, 1, 0);
  LCD_Sprite(240, 30, 41, 36, Background, 1, 0, 0, 0);
  LCD_Sprite(279, 30, 41, 36, Background, 1, 0, 1, 0);
  
  for(int x = 0; x <319; x++){
    LCD_Bitmap(x, 208, 30, 30, piso);
    x+=28;
   }
   
  FillRect(0, 66, 320, 144, 0x5bae);

  LCD_Bitmap(20, 69, 60, 42, helicopter);
}
//***************************************************************************************************************************************
// Loop Infinito
//***************************************************************************************************************************************
void loop() {
  for(int x = 0; x<280; x++){
    delay(15);
    int anim1 = (x/15)%2;
    LCD_Sprite(x, 168, 40, 42, Metal1,6, anim1, 0, 0); 
    V_line( x -1, 168, 41, 0x5bae);
    }
  FillRect(280, 168, 40, 42, 0x5bae);
}

int chartohex(int val){
  switch(val){
    case 48:
      return 0;
    case 49:
      return 1;
    case 50:
      return 2;
    case 51:
      return 3;
    case 52:
      return 4;
    case 53:
      return 5;
    case 54:
      return 6;
    case 55:
      return 7;
    case 56:
      return 8;
    case 57:
      return 9;
    case 97:
      return 10;
    case 98:
      return 11;
    case 99:
      return 12;
    case 100:
      return 13;
    case 101:
      return 14;
    case 102:
      return 15;
    }
  }
