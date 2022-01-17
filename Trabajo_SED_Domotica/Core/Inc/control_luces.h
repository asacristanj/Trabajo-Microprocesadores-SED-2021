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
int estado_luces=0;//0 off 1 on 2 auto

void setLuces(int n){
	if(n==0){
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	}else if(n==1){
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	}else if(n==2){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	}
	estado_luces=n;
}

int getEstadoLuces(){
	return estado_luces;
}

void cambiarEstadoLuces(){
		if(estado_luces==0){
			setLuces(1);
		}else if(estado_luces==1){
			setLuces(2);
		}else if(estado_luces==2){
			setLuces(0);
		}

}

void medirLDR(){
	if(estado_luces==2){
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
