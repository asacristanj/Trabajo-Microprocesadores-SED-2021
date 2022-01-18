/*
 * control_clima.h
 *
 *  Created on: Jan 17, 2022
 *      Author: Raul
 */

#ifndef INC_CONTROL_CLIMA_H_
#define INC_CONTROL_CLIMA_H_
#include "main.h"

TIM_HandleTypeDef htim6;
uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
uint16_t SUM, RH, TEMP;
float Temperature = 0;
float Humidity = 0;
uint8_t Presence = 0;

int estado_clima; //0 todo apagado, 1 calef encendida y aire apagado, 2 calef apagada y aire encendido
int controldelclima=0; //0 manual, 1 auto
uint32_t tickstart_clima=0; counter_clima=0;

void setEstadoClima(int n){
	estado_clima=n;
	if(estado_clima==0){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);
	}else if(estado_clima==1){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);
	}else if(estado_clima==2){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);
	}
}

int getEstadoClima(){
	return estado_clima;
}

void cambiarEstadoClima(){
	controldelclima=0;

	if(estado_clima==0){
		setEstadoClima(1);
	}else if(estado_clima==1){
		setEstadoClima(2);
	}else if(estado_clima==2){
		setEstadoClima(0);
	}
}

void cambiarControlClima(){
	if(controldelclima==0){
		controldelclima=1;
	}else{
		controldelclima=0;
	}
}


void delay(uint16_t time) {
	__HAL_TIM_SET_COUNTER(&htim6, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim6)) < time)
		;
}

void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_PIN_2

void DHT11_Start(void) {
	Set_Pin_Output(DHT11_PORT, DHT11_PIN);  // set the pin as output
	HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, 0);   // pull the pin low
	delay(18000);   // wait for 18ms
	HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, 1);   // pull the pin high
	delay(20);   // wait for 20us
	Set_Pin_Input(DHT11_PORT, DHT11_PIN);    // set as input
}

uint8_t DHT11_Check_Response(void) {
	uint8_t Response = 0;
	delay(40);
	if (!(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN))) {
		delay(80);
		if ((HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)))
			Response = 1;
		else
			Response = -1; // 255
	}
	while ((HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)))
		;   // wait for the pin to go low

	return Response;
}

uint8_t DHT11_Read(void) {
	uint8_t i, j;
	for (j = 0; j < 8; j++) {
		while (!(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)))
			;   // wait for the pin to go high
		delay(40);   // wait for 40 us
		if (!(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)))   // if the pin is low
		{
			i &= ~(1 << (7 - j));   // write 0
		} else
			i |= (1 << (7 - j));  // if the pin is high, write 1
		while ((HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)))
			;  // wait for the pin to go low
	}
	return i;
}

void lectura_dht11() {
	DHT11_Start();
	Presence = DHT11_Check_Response();
	Rh_byte1 = DHT11_Read();
	Rh_byte2 = DHT11_Read();
	Temp_byte1 = DHT11_Read();
	Temp_byte2 = DHT11_Read();
	SUM = DHT11_Read();
	TEMP = Temp_byte1;
	RH = Rh_byte1;
	Temperature = (float) TEMP;
	Humidity = (float) RH;
}

void clima(){
	if(controldelclima==1 && counter_clima>1000){
		counter_clima=0;
		tickstart_clima=HAL_GetTick();
		lectura_dht11();
		if(Temperature>20){
			setEstadoClima(2);
		}else{
			setEstadoClima(1);
		}
	}else{
		counter_clima=HAL_GetTick()-tickstart_clima;
	}

}

#endif /* INC_CONTROL_CLIMA_H_ */
