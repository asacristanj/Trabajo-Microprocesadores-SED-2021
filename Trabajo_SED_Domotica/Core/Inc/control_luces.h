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
int8_t adcval[10];
uint32_t counter_luces=0, tickstart_luces=0;
int umbral_luces=80;

void setLuces(int n){
	if(n==0){
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	}else if(n==1){
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
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

void setUmbralLuces(int u){
	umbral_luces=u;
}

void medirLDR(){
	if(estado_luces==2 && counter_luces>300){
		counter_luces=0;
		int media=0;
		tickstart_luces=HAL_GetTick();
		int i=0;
		for(i=0;i<10;i++){
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1, 100);
			adcval[i]=HAL_ADC_GetValue(&hadc1);
			HAL_ADC_Stop(&hadc1);
			media+=adcval[i];
		}
		media=media/10;
			if(media>umbral_luces){
				  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
			}else{
				  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
			}
	}else{
		counter_luces=HAL_GetTick()-tickstart_luces;
	}
}

void luces(){
	medirLDR();
}

#endif /* INC_CONTROL_LUCES_H_ */
