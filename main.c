#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "gpio.h"
#include "gpiote.h"
#include "ppi.h"

ssize_t _write(int fd, const void *buf, size_t count){
    char * letter = (char *)(buf);
    for(int i = 0; i < count; i++){
    uart_send(*letter);
    letter++;
    }
    return count;
  }


int main(){
  // Definerer led som output og setter lave
  for(int i = 4; i <= 15; i++){
    GPIO->DIRSET = (1 << i);
    GPIO->OUTCLR = (1 << i);
  }

  // Konigurerer de 4 GPIOTE kanalene
  GPIOTE -> CONFIG[0] = (1<<0) | (17 << 8) | (1 << 16) | (1 << 20);   //A button

  GPIOTE -> CONFIG[1] = (3<<0) | (13 << 8) | (3 << 16) | (1 << 20);
  GPIOTE -> CONFIG[2] = (3<<0) | (14 << 8) | (3 << 16) | (1 << 20);
  GPIOTE -> CONFIG[3] = (3<<0) | (15 << 8) | (3 << 16) | (1 << 20);


  //PPI
  PPI->PPI_CH[0].EEP = (uint32_t)&(GPIOTE->IN[0]);
  PPI->PPI_CH[0].TEP = (uint32_t)&(GPIOTE->OUT[1]);

  PPI->PPI_CH[1].EEP = (uint32_t)&(GPIOTE->IN[0]);
  PPI->PPI_CH[1].TEP = (uint32_t)&(GPIOTE->OUT[2]);

  PPI->PPI_CH[2].EEP = (uint32_t)&(GPIOTE->IN[0]);
  PPI->PPI_CH[2].TEP = (uint32_t)&(GPIOTE->OUT[3]);

  PPI->CHENSET = (7 << 0);

  while(1) {};
  return 0;
}
