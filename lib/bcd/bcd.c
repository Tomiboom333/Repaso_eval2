#include "bcd.h"
#include "stm32f103xb.h"
#include <stdbool.h>

int numAct = 0, unidad = 0, decena = 0;
int pulsA=1, pulsB=2, pulsC=3;

int pines1[7] = {0,1,2,3,4,5,6};//falta declarar
int pines2[7] = {7,8,9,10,11,12,13};//falta declarar

bool botonA=false, botonB=false,botonC=false;

int numeros[10][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}  // 9
};


void BCD_Init(){
    RCC -> APB2ENR|= RCC_APB2ENR_IOPAEN; 
    RCC -> APB2ENR|= RCC_APB2ENR_IOPBEN; 
    RCC -> APB2ENR|= RCC_APB2ENR_AFIOEN;
    //declaro entradas
    GPIOA -> CRL &= ~(0xF << pulsA*4);//limpio el pin
    GPIOA -> CRL &= ~(0xF << pulsB*4);
    GPIOA -> CRL &= ~(0xF << pulsC*4);

    GPIOA -> CRL |= (0b0100 << pulsA*4);//floating input
    GPIOA -> CRL |= (0b0100 << pulsB*4);
    GPIOA -> CRL |= (0b0100 << pulsC*4);
    //declaro salidas
    for(int i= 0;i<7;i++){
        GPIOB -> CRL &= ~(0xF << pines1[i]*4);//limpio el pin
        GPIOB -> CRL |= (0b0001 << pines1[i]*4);//output push-pull

        GPIOB -> CRL &= ~(0xF << pines2[i]*4);//limpio el pin
        GPIOB -> CRL |= (0b0001 << pines2[i]*4);//output push-pull
    }

    AFIO -> EXTICR[0] &= ~(0xF << pulsA *4); // borrar los4 bits del registro 
    AFIO -> EXTICR[0] |= (0x0 << pulsA *4); // pa

    //interrupción pulsador A

    EXTI -> IMR |= (1<<pulsA);//desenmascaro el GPIO
    EXTI -> RTSR |= (1<< pulsA);//habilito flanco de subida 
    EXTI -> FTSR &= ~(1<< pulsA);//habilito flanco de bajada

    NVIC_EnableIRQ(EXTI0_IRQn);//habilito la interrupción
    NVIC_SetPriority(EXTI0_IRQn,2);//seteo la prioridad
    
    //interrupción pulsador B

    AFIO -> EXTICR[0] &= ~(0xF << pulsB *4); // borrar los4 bits del registro 
    AFIO -> EXTICR[0] |= (0x0 << pulsB *4); // pa

    EXTI -> IMR |= (1<<pulsB);//desenmascaro el GPIO
    EXTI -> RTSR |= (1<< pulsB);//habilito flanco de subida 
    EXTI -> FTSR &= ~(1<< pulsB);//habilito flanco de bajada

    NVIC_EnableIRQ(EXTI1_IRQn);//habilito la interrupción
    NVIC_SetPriority(EXTI1_IRQn,2);//seteo la prioridad
    
    //interrupción pulsador C

    AFIO -> EXTICR[0] &= ~(0xF << pulsC *4); // borrar los4 bits del registro 
    AFIO -> EXTICR[0] |= (0x0 << pulsC *4); // pa

    EXTI -> IMR |= (1<<pulsC);//desenmascaro el GPIO
    EXTI -> RTSR |= (1<< pulsC);//habilito flanco de subida 
    EXTI -> FTSR &= ~(1<< pulsC);//habilito flanco de bajada

    NVIC_EnableIRQ(EXTI2_IRQn);//habilito la interrupción
    NVIC_SetPriority(EXTI2_IRQn,2);//seteo la prioridad
}
void BCD_puls(char letra){
    letra = toupper(letra);
    if (letra = 'A') {
        numAct++;
        if (numAct > 99) numAct = 0;
    } 
    if(letra = 'B'){
      numAct--;
      if (numAct < 0) numAct = 99;
    }
    if(letra = 'C'){
        //reseteo todo
    }
    unidad = numAct % 10;
    decena = numAct / 10;

    mostrarUni(unidad);
    mostrarDec(decena);
}
void mostrarUni(int unidad) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(pines1[i], numeros[unidad][i]);//primer BCD
  }
}

void mostrarDec(int decena){
  for (int i = 0; i < 7; i++) {
    digitalWrite(pines2[i], numeros[decena][i]);//segundo BCD
  }
}