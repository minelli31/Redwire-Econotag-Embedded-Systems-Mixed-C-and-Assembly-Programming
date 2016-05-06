/*
 * Sistemas operativos empotrados
 * Inicialización del HAL
 */

#include <fcntl.h>
#include <unistd.h>
#include "system.h"

/*****************************************************************************/

/**
 * Inicializa los vectores de excepción y el controlador de interrupciones.
 */
static void bsp_excep_init( void )
{
	/* Inicializamos los manejadores de excepción */
	excep_init();

	/* Inicializamos el controlador de interrupciones */
	itc_init ();
}

/*****************************************************************************/

/**
 * Inicializa los dispositivos del sistema.
 * Esta función se debe llamar después de  bsp_int_init().
 */
static void bsp_sys_init( void )
{
	/* Inicialización de las UARTs */
	uart_init(UART1_ID, UART1_BAUDRATE, UART1_NAME);
	uart_init(UART2_ID, UART2_BAUDRATE, UART2_NAME);
}

/*****************************************************************************/

/**
 * Redirección de la E/S estándar.
 * Esta función se llama por el BSP antes de llamar a main, y una vez que se
 * han inicializado los dispositivos, para fijar la E/S estándar del sistema,
 * que por defecto está asignada a /dev/null.
 * @param stdin_dev  Nombre del dispositivo que se usará para la entrada estándar
 * @param stdout_dev Nombre del dispositivo que se usará para la salida estándar
 * @param stderr_dev Nombre del dispositivo que se usará para el error estándar
 */
static void bsp_io_redirect(const char* stdin_dev, const char* stdout_dev, const char* stderr_dev)
{
	/* Redireccionamos la E/S estándar */
	redirect_fd (STDIN_FILENO, stdin_dev, O_RDONLY, 0777);
	redirect_fd (STDOUT_FILENO, stdout_dev, O_WRONLY, 0777);
	redirect_fd (STDERR_FILENO, stderr_dev, O_WRONLY, 0777);
}

/*****************************************************************************/

/**
 * Realiza la inicialización de la plataforma. Se llama desde el código de
 * arranque en ensamblador específico del procesador, crt0.s.
 * Aquí se termina la configuración del runtime de C y se configuran los
 * dispositivos y sistemas de archivos.
 */
void bsp_init (void)
{
	/* Inicializamos las excepciones */
	bsp_excep_init();

	/* Inicializamos los drivers de los dispositivos */
	bsp_sys_init();

	/*
	 * Redireccionamos la E/S estándar a los dispositivos apropiados, ahora que
	 * han sido inicializados.
	 */
	bsp_io_redirect(BSP_STDIN, BSP_STDOUT, BSP_STDERR);

}

/*****************************************************************************/
