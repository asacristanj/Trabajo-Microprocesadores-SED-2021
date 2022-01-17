/*
 * control_seguridad.h
 *
 *  Created on: Jan 17, 2022
 *      Author: Raul
 */

#ifndef INC_CONTROL_SEGURIDAD_H_
#define INC_CONTROL_SEGURIDAD_H_
#include "main.h"

int estado_seguridad=0; //0 sistema apagado, 1 sistema encendiendose, 2 sistema_ activo
int alarma_seguridad=0; //0 alarma apagada, 1 alarma encendida

uint32_t tickstart_seguridad;



void setEstadoSeguridad(int n){
	estado_seguridad=n;
	if(estado_seguridad==0){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		alarma_seguridad=0;
	}else if(estado_seguridad==1){
		tickstart_seguridad=HAL_GetTick();
	}else if(estado_seguridad==2){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);

	}
}

void activar_sensor(){
	if(estado_seguridad==1){
		if(HAL_GetTick()-tickstart_seguridad>5000){
			setEstadoSeguridad(2);
		}
	}
}

int getEstadoSeguridad(){
	return estado_seguridad;
}

void cambiarEstadoSeguridad(){
	if(getEstadoSeguridad()==0){
		setEstadoSeguridad(1);
	}else {
		setEstadoSeguridad(0);
	}
}

void intruso_detectado(){
	alarma_seguridad=0;
	if(estado_seguridad==2){
		alarma_seguridad=1;
	}
}

void sonar_alarma(){

}

void seguridad(){
	activar_sensor();
	sonar_alarma();
}

#endif /* INC_CONTROL_SEGURIDAD_H_ */
