/*
 * control_BT.h
 *
 *  Created on: Dec 23, 2021
 *      Author: Raul
 */

#ifndef INC_CONTROL_BT_H_
#define INC_CONTROL_BT_H_
#include"main.h"
#include "stdlib.h"
#include "string.h"
#include "TFT.h"

UART_HandleTypeDef huart6;
char rx_buffer[50], tx_buffer[50];


const char *t_menu[6] = { "LUCES", "PERSIANAS", "RIEGO", "CLIMA", "AUDIO",
		"SEGURIDAD" };
int opcion = 0;
void seleccion_menu_principal(){
	HAL_UART_Receive(&huart6, (uint8_t*)rx_buffer, 50, 500);
	if (rx_buffer[0]=='r') {
		if (opcion == 3) {
			opcion = 1;
		}else if (opcion == 6) {
			opcion = 4;
		}else{
			opcion++;
		}
		resaltar_boton(opcion);
		setFont(1);
		rellenar_botones(t_menu);
		rx_buffer[0]=0;
	} else if (rx_buffer[0]=='l') {
		if (opcion == 1) {
			opcion = 3;
		}else if (opcion == 4) {
			opcion = 6;
		}else if (opcion == 0) {
			opcion = 1;
		}else{
			opcion--;
		}
		resaltar_boton(opcion);
		setFont(1);
		rellenar_botones(t_menu);
		rx_buffer[0]=0;
	}
	else if (rx_buffer[0]=='u') {
				if (opcion == 1) {
					opcion = 4;
				}else if (opcion == 2) {
					opcion = 5;
				}else if (opcion == 3) {
					opcion = 6;
				}else if (opcion == 0) {
					opcion = 1;
				}else{
					opcion-=3;
				}
				resaltar_boton(opcion);
				setFont(1);
				rellenar_botones(t_menu);
				rx_buffer[0]=0;
			}
	else if (rx_buffer[0]=='d') {
				if (opcion == 4) {
					opcion = 1;
				}else if (opcion == 5) {
					opcion = 2;
				}else if (opcion == 0) {
					opcion = 1;
				}else if (opcion == 6) {
					opcion = 3;
				}else{
					opcion+=3;
				}
				resaltar_boton(opcion);
				setFont(1);
				rellenar_botones(t_menu);
				rx_buffer[0]=0;
			}
}


#endif /* INC_CONTROL_BT_H_ */
