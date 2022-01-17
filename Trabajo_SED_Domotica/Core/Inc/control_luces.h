/*
 * control_luces.h
 *
 *  Created on: Jan 2, 2022
 *      Author: Raul
 */

#ifndef INC_CONTROL_LUCES_H_
#define INC_CONTROL_LUCES_H_
#include "main.h"

int estado=0;
int modo=0; //0 manual 1 auto

void setLuces(int n){
	if(n==0){
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);

	}else{
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
	}
	  estado=n;
}

int getEstadoLuces(){
	return estado;
}

void cambiarEstadoLuces(){
	if(estado==0){
		setLuces(1);
	}else{
		setLuces(0);
	}
}

void setModo(int n){
	if(n==0){
			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		}else{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		}
	  modo=n;
}
int getModo(){
	return modo;
}

void cambiarModo(){
	if(modo==0){
		setModo(1);
	}else{
		setModo(0);
	}
}

void medirLDR(){
	if(modo==1){
		int8_t adcval;
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1, 100);
			adcval=HAL_ADC_GetValue(&hadc1);
			HAL_ADC_Stop(&hadc1);
			if(adcval>500)
				setLuces(1);
			else
				setLuces(0);
	}
}

#endif /* INC_CONTROL_LUCES_H_ */
