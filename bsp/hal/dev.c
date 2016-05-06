/*
 * Sistemas operativos empotrados
 * Manejo de dispositivos
 */

#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include "system.h"

/*****************************************************************************/

/**
 * Lista de dispositivos registrados.
 * El máximo número de dispositivos se define en "system.h"
 */
static bsp_dev_t bsp_dev_list[BSP_MAX_DEV] =
{
		/* La primera entrada de la tabla es /dev/null */
		{
				"/dev/null",	/* Nombre del dispositivo */
				0,				/* Identificador */
				NULL,			/* Función open por defecto */
				NULL,			/* Función close por defecto */
				NULL,			/* Función read por defecto */
				NULL,			/* Función write por defecto */
				NULL,			/* Función lseek por defecto */
				NULL,			/* Función fstat por defecto */
				NULL			/* Función isatty por defecto */
		}
		/* El resto del array se inicializa a cero */
};


/*****************************************************************************/

/**
 * Indice de la siguiente entrada libre en la tabla de dispositivos.
 */
static uint32_t bsp_next_dev = 1;

/*****************************************************************************/

/**
 * Lista de descriptores de fichero abiertos. Las primeras tres entradas se
 * reservan para E/S estándar, asignada por defecto a /dev/null.
 * El máximo número de descriptores abiertos simultáneamente se define en
 * "system.h"
 */
static bsp_fd_t bsp_fd_list[BSP_MAX_FD] =
{
		{ bsp_dev_list, 0 },    /* Entrada estándar (STDIN) -> /dev/null  */
		{ bsp_dev_list, 0 },    /* Salida estándar  (STDOUT) -> /dev/null */
		{ bsp_dev_list, 0 }     /* Error estándar   (STDERR) -> /dev/null */
		/* El resto de entradas se inicializan a cero */
};

/*****************************************************************************/

/**
 * Registro de un dispositivo en el sistema.
 * @param name		Nombre del dispositivo
 * @param id		Dirección base de los registros de gestión del dispositivo
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
		int (*isatty)(uint32_t id))
{
	int32_t index = -1;
	if (bsp_next_dev < BSP_MAX_DEV)
	{
		index = bsp_next_dev;
		bsp_next_dev++;

		bsp_dev_list[index].name = name;
		bsp_dev_list[index].id = id;
		bsp_dev_list[index].open = open;
		bsp_dev_list[index].close = close;
		bsp_dev_list[index].read = read;
		bsp_dev_list[index].write = write;
		bsp_dev_list[index].lseek = lseek;
		bsp_dev_list[index].fstat = fstat;
		bsp_dev_list[index].isatty = isatty;
	}

	return index;
}

/*****************************************************************************/

/**
 * Busca un dispositivo en el sistema
 * @param pathname   Nombre del dispositivo
 */
bsp_dev_t * find_dev (const char *pathname)
{
	uint32_t i;
	uint32_t len = strlen(pathname) + 1;

	/* Usamos memcmp() en vez de strcmp() para reducir el tamaño del ejecutable */
	for (i = 0 ; i < bsp_next_dev ; i++)
		if (!memcmp (bsp_dev_list[i].name, pathname, len))
			return &bsp_dev_list[i];

	return NULL;
}

/*****************************************************************************/

/**
 * Retorna el puntero del dispositivo asociado al descriptor de un fichero
 * @param fd   El descriptor
 */
inline bsp_dev_t* get_dev (uint32_t fd)
{
	return bsp_fd_list[fd].dev;
}

/*****************************************************************************/

/**
 * Retorna los flags de apertura de un fichero
 * @param fd   El descriptor
 */
inline int get_flags (uint32_t fd)
{
	return bsp_fd_list[fd].flags;
}

/*****************************************************************************/

/**
 * Asigna un nuevo descriptor de fichero a un dispositivo
 * @param dev	El dispositivo
 * @param flags	Modo de acceso seleccionado en su apertura
 * @return 		El numero de descriptor o -1 en caso de error. La condición de error
 * 				se indica en la variable global errno.
 */
int32_t get_fd(bsp_dev_t *dev, int flags)
{
	int32_t i;

	for (i = 0; i < BSP_MAX_FD; i++)
	{
		/* Si la entrada de la tabla está vacía, le asignamos un nuevo descriptor */
		if (bsp_fd_list[i].dev == NULL)
		{
			bsp_fd_list[i].dev = dev;
			bsp_fd_list[i].flags = flags;
			return i;
		}
	}

	/* Se ha alcanzado el máximo número de ficheros abiertos */
	errno = ENFILE;
	return -1;
}

/*****************************************************************************/

/**
 * Liberación de un descriptor de fichero. Los descriptores de los ficheros
 * asignados a la E/S estándar (0, 1 y 2) no pueden ser liberados.
 * @param fd Número del descriptor
 */
void release_fd (uint32_t fd)
{
	if (fd > 2)
	{
		bsp_fd_list[fd].dev   = NULL;
		bsp_fd_list[fd].flags = 0;
	}
}

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
void redirect_fd(uint32_t fd, const char* name, int flags, mode_t mode)
{
	int temp;

	temp = open (name, flags, mode);

	if (temp >= 0)
	{
		bsp_fd_list[fd].dev   = bsp_fd_list[temp].dev;
		bsp_fd_list[fd].flags = bsp_fd_list[temp].flags;

		release_fd (temp);
	}
} 

/*****************************************************************************/
