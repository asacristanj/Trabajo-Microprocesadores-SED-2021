/*
 * control_persianas.h
 *
 *  Created on: Jan 17, 2022
 *      Author: Raul
 */

#ifndef INC_CONTROL_PERSIANAS_H_
#define INC_CONTROL_PERSIANAS_H_
#include "main.h"

int estado_persianas=0; //0 parado 1 subiendo 2 bajando
int estado_anterior_persianas=0;

void setEstadoPersianas(int n){
	estado_anterior_persianas=estado_persianas;
	estado_persianas=n;
	if(estado_persianas==0){
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
	}else if(estado_persianas==1){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
	}else if(estado_persianas==2){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
	}
	actualizar_pantalla(4);
}

int getEstadoPersianas(){
	return estado_persianas;
}

int getEstadoAnteriorPersianas(){
	return estado_anterior_persianas;
}

void cambiarEstadoPersianas(){
	if(getEstadoPersianas()==0 && getEstadoAnteriorPersianas()==2){
		setEstadoPersianas(1);
	}else if(getEstadoPersianas()==0 && getEstadoAnteriorPersianas()==1){
		setEstadoPersianas(2);
	}else if(getEstadoPersianas()==0 && getEstadoAnteriorPersianas()==0){
		setEstadoPersianas(2);
	}else if(getEstadoPersianas()==1){
		setEstadoPersianas(0);
	}else if(getEstadoPersianas()==2){
		setEstadoPersianas(0);
	}
}

#endif /* INC_CONTROL_PERSIANAS_H_ */
