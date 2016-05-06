#
# Makefile del BSP para la Redwire EconoTAG
#

# Este makefile está escrito para una shell bash
SHELL = /bin/bash

#
# Paths y nombres de directorios
#

# Ruta al BSP
BSP_ROOT_DIR   = .

# Directorio para almacenar los ficheros objeto
OBJ_DIR        = $(BSP_ROOT_DIR)/obj

# Directorio de la toolchain de GNU
#TOOLS_PATH     = /fenix/depar/atc/se/toolchain
TOOLS_PATH     = /opt/econotag

#
# Herramientas y cadena de desarrollo
#

# Herramientas del sistema
MKDIR          = mkdir -p
RM             = rm -rf

# Cadena de desarrollo
TOOLS_PREFIX   = arm-econotag-eabi
CROSS_COMPILE  = $(TOOLS_PATH)/bin/$(TOOLS_PREFIX)-
AS             = $(CROSS_COMPILE)as
CC             = $(CROSS_COMPILE)gcc
AR             = $(CROSS_COMPILE)ar

# Flags
ASFLAGS        = -gstabs -mcpu=arm7tdmi -mfpu=softfpa
CFLAGS         = -c -g -Wall -mcpu=arm7tdmi
ARFLAGS        = -src

#
# Fuentes
#

# Todos los ficheros C del BSP
C_SRCS_TMP     = $(shell find $(BSP_ROOT_DIR) -name '*.c' -print)
C_SRCS         = $(C_SRCS_TMP:./%=%)

# Todos los ficheros en ensamblador del BSP
ASM_SRCS_TMP   = $(shell find $(BSP_ROOT_DIR) -name '*.s' -print)
ASM_SRCS       = $(ASM_SRCS_TMP:./%=%)
#
# Lista de ficheros objeto
#

OBJS           = $(addprefix $(OBJ_DIR)/, $(ASM_SRCS:.s=.o) $(C_SRCS:.c=.o))

#
# Incluimos el Makefile público del BSP
#

include bsp.mk

ASFLAGS        += $(BSP_ASFLAGS)
CFLAGS         += $(BSP_CFLAGS)

#
# Reglas de construcción
#

.PHONY: all
all: $(BSP_LIB)

$(BSP_LIB): $(OBJS)
	@echo "Generando la biblioteca del bsp ..."
	$(AR) $(ARFLAGS) $@ $^
	@echo

$(OBJ_DIR)/%.o : %.s
	@echo "Ensamblando $< ..."
	@$(MKDIR) $(@D)
	$(AS) $(ASFLAGS) $< -o $@
	@echo

$(OBJ_DIR)/%.o : %.c
	@echo "Compilando $< ..."
	@$(MKDIR) $(@D)
	$(CC) $(CFLAGS) $< -o $@
	@echo

.PHONY: clean
clean:
	@echo "Limpiando el BSP ..."
	@$(RM) $(BSP_LIB) $(OBJ_DIR) $(shell find $(BSP_ROOT_DIR) -name '*~' -print)

