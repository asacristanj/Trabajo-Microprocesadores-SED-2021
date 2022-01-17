/*
 * control_luces.h
 *
 *  Created on: Jan 2, 2022
 *      Author: Raul
 */

#ifndef INC_CONTROL_LUCES_H_
#define INC_CONTROL_LUCES_H_
#include "main.h"

ADC_HandleTypeDef hadc1;
int estado_luces=0;
int modo_luces=0; //0 manual 1 auto

void setLuces(int n){
	if(n==0){
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);

	}else{
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
	}
	estado_luces=n;
}

int getEstadoLuces(){
	return estado_luces;
}

void cambiarEstadoLuces(){
		if(estado_luces==0){
			setLuces(1);
		}else{
			setLuces(0);
		}

}

void setModoLuces(int n){
	if(n==0){
			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		}else{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		}
	modo_luces=n;
}
int getModoLuces(){
	return modo_luces;
}

void cambiarModoLuces(){
	if(modo_luces==0){
		setModoLuces(1);
	}else{
		setModoLuces(0);
	}
}

void medirLDR(){
	if(modo_luces==1){
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

void luces(){
	medirLDR();
}

#endif /* INC_CONTROL_LUCES_H_ */
