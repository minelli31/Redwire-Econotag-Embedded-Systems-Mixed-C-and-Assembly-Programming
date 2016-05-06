/*****************************************************************************/
/*                                                                           */
/* Sistemas Empotrados                                                       */
/* El "hola mundo" en la Redwire EconoTAG en C                               */
/*                                                                           */
/*****************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include "system.h"

/*
 * Constantes relativas a la plataforma
 */

// definimos con gpios mas finamente
#define RED_LED gpio_pin_44
#define GREEN_LED gpio_pin_45

/*
 * Variables globales
 */
 
/* Guarda estado del led rojo */
uint8_t r_led = 1; 

/* Guarda estado del led verde */ 
uint8_t g_led = 1; 

/*****************************************************************************/

/*
 * Inicialización de los pines de E/S
 */
void gpio_init(void)
{
	/* Configuramos el GPIO44 y GPIO45 para que sean de salida */
	gpio_set_pin_dir_output(RED_LED);
	gpio_set_pin_dir_output(GREEN_LED);
}

/*****************************************************************************/

/*
 * Callback de recepción
 */

void rec_callback(void){
	char c;

	c = getchar();

	if (c == 'r')
		r_led ^= 1; 
 
	else if (c == 'g')
		g_led ^= 1; 

	else
		printf("error");

}





// funcion para manejar el delay
void pause(void){
	uint32_t i, delay = 0x10000;
	for (i = 0; i < delay; ++i);
}

/*
 * Programa principal
 */
int main ()
{
	gpio_init();
	uart_set_receive_callback(uart_1, rec_callback);

	while(1)
	{
		if(g_led)
			gpio_set_pin(GREEN_LED);
		
		if (r_led)
			gpio_set_pin(RED_LED);

		pause();

		gpio_clear_pin(GREEN_LED);

	    gpio_clear_pin(RED_LED);

	    pause();

	}


}

/*****************************************************************************/

