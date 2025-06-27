#include "bcd.h"
#include "stm32f103xb.h"

int pulsA=1, pulsB=2, pulsC=3;
int salidas[7]={1,2,3,4,5,6,7};

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
             GPIOB -> CRL &= ~(0xF << salidas[i]*4);//limpio el pin
             GPIOB -> CRL |= (0b0001 << salidas[i]*4);//output push-pull
    }

}