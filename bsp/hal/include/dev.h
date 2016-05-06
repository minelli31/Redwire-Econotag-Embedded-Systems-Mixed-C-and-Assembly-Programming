/*
 * Sistemas operativos empotrados
 * Manejo de dispositivos
 */

#ifndef __DEV_H__
#define __DEV_H__

#include <sys/stat.h>
#include "system.h"

/*****************************************************************************/

/**
 * Estructura para almacenar las funciones de gestión de cada dispositivo
 */
typedef struct
{
	const char  *name;										/* Nombre del dispositivo */
	uint32_t id;											/* Identificador del dispositivo */
															/* Por defecto es cero. Se usa para */
															/* diferenciar a varios dispositivos */
															/* del mismo tipo */
	int (*open)(uint32_t id, int flags, mode_t mode);		/* Función open */
	int (*close)(uint32_t id);								/* Función close */
	ssize_t (*read)(uint32_t id, char *buf, size_t count);	/* Función read */
	ssize_t (*write)(uint32_t id, char *buf, size_t count);	/* Función write */
	off_t (*lseek)(uint32_t id, off_t offset, int whence);	/* Función lseek */
	int (*fstat)(uint32_t id, struct stat *buf);			/* Función fstat */
	int (*isatty)(uint32_t id);								/* Función isatty */
} bsp_dev_t;

/*****************************************************************************/

/**
 * Estructura de un descriptor de fichero
 */
typedef struct
{
	bsp_dev_t* dev;        /* Puntero a la estructura gestión del dispositivo */
	int        flags;      /* Flags de apertura/creación del fichero */
} bsp_fd_t;

/*****************************************************************************/

/**
 * Registro de un dispositivo en el sistema.
 * @param name		Nombre del dispositivo
 * @param id		Identificador del dispositivo
 * @param open		Función de apertura del dispositivo
 * @param close		Función close del dispositivo
 * @param read		Función read del dispositivo
 * @param write		Función write del dispositivo
 * @param lseek		Función lseek del dispositivo
 * @param fstat		Función fsat del dispositivo
 * @param isatty	Función isatty del dispositivo
 * @return 			El numero de dispositivo asignado o -1 en caso de error
 */
int32_t bsp_register_dev (const char  *name,
		uint32_t id,
		int (*open)(uint32_t id, int flags, mode_t mode),
		int (*close)(uint32_t id),
		ssize_t (*read)(uint32_t id, char *buf, size_t count),
		ssize_t (*write)(uint32_t id, char *buf, size_t count),
		off_t (*lseek)(uint32_t id, off_t offset, int whence),
		int (*fstat)(uint32_t id, struct stat *buf),
		int (*isatty)(uint32_t id));

/*****************************************************************************/

/**
 * Busca un dispositivo en el sistema
 * @param pathname   Nombre del dispositivo
 */
bsp_dev_t * find_dev (const char *pathname);

/*****************************************************************************/

/**
 * Retorna el puntero del dispositivo asociado al descriptor de un fichero
 * @param fd   El descriptor
 */
inline bsp_dev_t* get_dev (uint32_t fd);

/*****************************************************************************/

/**
 * Retorna los flags de apertura de un fichero
 * @param fd   El descriptor
 */
inline int get_flags (uint32_t fd);

/*****************************************************************************/

/**
 * Asigna un nuevo descriptor de fichero a un dispositivo
 * @param dev	El dispositivo
 * @param flags	Modo de acceso seleccionado en su apertura
 * @return 		El numero de descriptor o -1 en caso de error. La condición de error
 * 				se indica en la variable global errno.
 */
int32_t get_fd(bsp_dev_t *dev, int flags);

/*****************************************************************************/

/**
 * Liberación de un descriptor de fichero. Los descriptores de los ficheros
 * asignados a la E/S estándar (0, 1 y 2) no pueden ser liberados.
 * @param fd Número del descriptor
 */
void release_fd (uint32_t fd);

/*****************************************************************************/

/**
 * Abre un dispositivo y lo asigna al descriptor de fichero especificado en vez
 * de crear una nueva entrada en la tabla de descriptores de fichero.
 * @param fd    Descriptor de fichero al que se redireccionará el dispositivo
 *              una vez abierto
 * @param name  Nombre del dispositivo
 * @param flags Configuración
 * @param mode  Modo de apertura
 */
void redirect_fd(uint32_t fd, const char* name, int flags, mode_t mode);

/*****************************************************************************/

#endif /* __DEV_H__ */

