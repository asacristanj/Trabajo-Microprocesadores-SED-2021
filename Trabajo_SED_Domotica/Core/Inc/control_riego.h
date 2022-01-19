/*
 * control_riego.h
 *
 *  Created on: Jan 17, 2022
 *      Author: Raul
 */

#ifndef INC_CONTROL_RIEGO_H_
#define INC_CONTROL_RIEGO_H_
#include "main.h"

int estado_riego=2; //0 abrir electrovalvula, 1 cerrar electrovalvula, 2 parar electrovalvula
int estado_anterior_riego=2;
uint32_t tickstart_riego;

void setEstadoRiego(int n){
	estado_anterior_riego=estado_riego;
	estado_riego=n;
	tickstart_riego=HAL_GetTick();
	if(estado_riego==0){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	}else if(estado_riego==1){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
	}
	else if(estado_riego==2){
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
		}
}

void cambiarEstadoRiego(){
	if(estado_anterior_riego==0){
		setEstadoRiego(1);
	}else if(estado_anterior_riego==1){
		setEstadoRiego(0);
	}else{
		setEstadoRiego(0);
	}
}
int getEstadoAnteriorRiego(){
	return estado_anterior_riego;
}
void temporizador_riego(){
	if(estado_riego==0 || estado_riego==1){
		if(HAL_GetTick()-tickstart_riego>1000){
			setEstadoRiego(2);
		}
	}
}
void riego(){
	temporizador_riego();
}
#endif /* INC_CONTROL_RIEGO_H_ */
