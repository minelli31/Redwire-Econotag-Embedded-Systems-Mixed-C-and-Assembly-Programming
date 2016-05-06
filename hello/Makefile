#
# Makefile de la aplicación para la Redwire EconoTAG
#

# Este makefile está escrito para una shell bash
SHELL = /bin/bash

#
# Paths y nombres de directorios
#

# Directorio de la toolchain de GNU
TOOLS_PATH     = /opt/econotag

# Directorio para las herramientas adicionales
EXTRA_TOOLS_PATH = ../tools

#
# Plataforma
#

# Detalles de la plataforma
SRAM_BASE = 0x00400000
SERIAL_PORT = /dev/ttyUSB1
BAUDRATE = 115200

#
# Herramientas y cadena de desarrollo
#

# Herramientas del sistema
MKDIR          = mkdir -p
RM             = rm -rf
#TERMINAL       = xterm -e "picocom -b $(BAUDRATE) $(SERIAL_PORT)"
#TERMINAL       = xterm -e "minicom -b $(BAUDRATE) -D $(SERIAL_PORT)"
#TERMINAL       = gtkterm -s $(BAUDRATE) -p $(SERIAL_PORT)
TERMINAL       = putty -serial -sercfg $(BAUDRATE) $(SERIAL_PORT)


# Cadena de desarrollo
TOOLS_PREFIX   = arm-econotag-eabi
CROSS_COMPILE  = $(TOOLS_PATH)/bin/$(TOOLS_PREFIX)-
AS             = $(CROSS_COMPILE)as
CC             = $(CROSS_COMPILE)gcc
LD             = $(CROSS_COMPILE)ld
OBJCOPY        = $(CROSS_COMPILE)objcopy
OPENOCD        = $(TOOLS_PATH)/bin/openocd

# Herramientas adicionales

MC1322X_LOAD   = $(EXTRA_TOOLS_PATH)/bin/mc1322x-load
FLASHER        = $(EXTRA_TOOLS_PATH)/flasher_redbee-econotag.bin
BBMC           = $(EXTRA_TOOLS_PATH)/bin/bbmc


# Flags
ASFLAGS        = -gstabs -mcpu=arm7tdmi -mfpu=softfpa
CFLAGS         = -c -g -Wall -mcpu=arm7tdmi
LDFLAGS        = -nostartfiles -Teconotag.ld

#
# Fuentes
#

# Aplicación
PROGNAME = hello
OBJ      = $(PROGNAME).o
ELF      = $(PROGNAME).elf
BIN      = $(PROGNAME).bin

#
# Reglas de construcción
#

.PHONY: all
all: $(ELF) $(BIN)

$(ELF) : $(OBJ) $(BSP_ROOT_DIR)/$(BSP_LIB) $(BSP_LINKER_SCRIPT)
	@echo "Enlazando $@ ..."
	$(LD) $(LDFLAGS) $< -o $@ $(LIBS)
	@echo

$(BIN) : $(ELF)
	@echo "Generando $@ ..."
	$(OBJCOPY) -O binary $< $@
	@echo

%.o : %.c
	@echo "Compilando $@ ..."
	$(CC) $(CFLAGS) $< -o $@
	@echo

%.o : %.s
	@echo "Ensamblando $@ ..."
	$(AS) $(ASFLAGS) $< -o $@
	@echo

#
# Reglas para gestionar la plataforma
#

# Ejecución
.PHONY: halt
halt: check-openocd
	@echo "Deteniendo el procesador ..."
	@echo -e "halt" | nc -i 1 localhost 4444 > /dev/null

# Ejecución vía OpenOCD
.PHONY: run
run: $(BIN) check-openocd
	@echo "Ejecutando el programa ..."
	@echo -e "soft_reset_halt\n load_image $< $(SRAM_BASE)\n resume $(SRAM_BASE)" | nc -i 1 localhost 4444  > /dev/null

# Ejecución vía mc1322x-load.pl
$(SERIAL_PORT):
	@echo "Conecta la placa!"
	@false

$(MC1322X_LOAD): $(EXTRA_TOOLS_PATH)/mc1322x-load
	@echo "Construyendo mc1322x_load ..."
	@make -C $< install 

$(BBMC): $(EXTRA_TOOLS_PATH)/bbmc
	@echo "Construyendo bbmc ..."
	@make -C $< install 

.PHONY: run2
run2: $(BIN) $(MC1322X_LOAD) $(SERIAL_PORT)
	@echo "Ejecutando el programa ..."
	@$(MC1322X_LOAD) -f $(BIN) -t $(SERIAL_PORT)

# Grabación de la imagen en la flash
.PHONY: flash
flash: $(BIN) $(MC1322X_LOAD) $(FLASHER) $(SERIAL_PORT)
	@echo "Grabando la imagen en la flash de la placa ..."
	@$(MC1322X_LOAD) -f $(FLASHER) -s $(BIN) -t $(SERIAL_PORT)

# Borrado de la flash de la placa
.PHONY: erase
erase: $(BIN) $(BBMC) $(SERIAL_PORT)
	@echo "Borrando la flash de la placa ..."
	@$(BBMC) -l redbee-econotag erase

# Terminal serie
.PHONY: term
term:  $(SERIAL_PORT)
	@echo "Abriendo terminal serie ..."
	@$(TERMINAL) &

# Depuración
.PHONY: openocd
openocd:
	@echo "Lanzando openocd ..."
	@xterm -e "$(OPENOCD) -f interface/ftdi/redbee-econotag.cfg -f board/redbee.cfg" &
	@sleep 1

.PHONY: check-openocd
check-openocd:
	@if [ ! `pgrep openocd` ]; then make -s openocd; fi

.PHONY: openocd-term
openocd-term: check-openocd
	@echo "Abriendo terminal openocd ..."
	@xterm -e "telnet localhost 4444" &

# Limpieza
.PHONY: clean
clean:
	@echo "Limpiando la aplicación ..."
	@$(RM) $(BIN) $(ELF) $(OBJ) *~

