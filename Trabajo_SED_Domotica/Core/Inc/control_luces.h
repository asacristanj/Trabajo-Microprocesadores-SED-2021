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
void setLuces(int n){
	if(n==0){
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);

	}else{
		  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
	}
	  estado=n;
}

int getEstadoLuces(){
	return estado;
}

#endif /* INC_CONTROL_LUCES_H_ */
