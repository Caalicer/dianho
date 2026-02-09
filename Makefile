#╔════════════════════════════════════════════════════════════════════════════╗#
#║                                                                            ║#
#║       ███╗   ███╗ █████╗ ██╗  ██╗███████╗███████╗██╗██╗     ███████╗       ║#
#║       ████╗ ████║██╔══██╗██║ ██╔╝██╔════╝██╔════╝██║██║     ██╔════╝       ║#
#║       ██╔████╔██║███████║█████╔╝ █████╗  █████╗  ██║██║     █████╗         ║#
#║       ██║╚██╔╝██║██╔══██║██╔═██╗ ██╔══╝  ██╔══╝  ██║██║     ██╔══╝         ║#
#║       ██║ ╚═╝ ██║██║  ██║██║  ██╗███████╗██║     ██║███████╗███████╗       ║#
#║       ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝╚═╝     ╚═╝╚══════╝╚══════╝       ║#
#║                                                                            ║#
#╚════════════════════════════════════════════════════════════════════════════╝#

#  ╔═════════════╗
#╔═╣ INGRIDIENTS ╠══════════════════════╗
#║ ╚═════════════╝                      ║
#║ NAME      target name                ║
#║ SRC_DIR   source directory           ║
#║ INC_DIR   include directory          ║
#║ OBJ_DIR   target object directory    ║
#║ BIN_DIR   target binary directory    ║
#║ CC        compiler                   ║
#║ CFLAGS    compiler flags             ║
#║ DFLAGS    compiler debug extra files ║
#║ LDFFLAGS  compiler link flags        ║
#║ SRCS      source files               ║
#║ OBJS      object files               ║
#╚══════════════════════════════════════╝

NAME    := programa
SRC_DIR := src
INC_DIR := inc
OBJ_DIR := obj
BIN_DIR := bin
CC      := gcc
CFLAGS  := -Wall -I$(INC_DIR)
DFLAGS  := -Wextra -g -DDEBUG
LDFLAGS := 
SRCS    := $(shell find $(SRC_DIR) -type f -name "*.c")
OBJS    := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

#  ╔══════════╗
#╔═╣ UTENSILS ╠═════════════════════════╗
#║ ╚══════════╝                         ║
#║  MKDIR_P  create directory           ║
#║  RM       force remove               ║
#║  RM_RF    recursive force remove     ║
#╚══════════════════════════════════════╝

MKDIR_P  := mkdir -p
RM       := rm -f
RM_RF    := rm -rf

#  ╔═════════╗
#╔═╣ RECIPES ╠════════════════════════════════════╗
#║ ╚═════════╝                                    ║
#║ help           default: print usage            ║
#║ release        release goal                    ║
#║ $(NAME)        compilation .c -> target        ║
#║ debug          debug goal                      ║
#║ $(NAME)_debug  link .o -> target               ║
#║ %.o            compilation .c -> .o            ║
#║ runrelease     run release target              ║
#║ rundebug       run debug target                ║
#║ valgrind       run debug target with valgrind  ║ 
#║ gdb            run debug target with gdb       ║
#║ clean          remove target and .o            ║
#║ cleanall       remove all (dirs)               ║
#╚════════════════════════════════════════════════╝

help:
	@echo "Opciones disponibles:"
	@echo "\t release    - Versión final sin archivos intermedios"
	@echo "\t debug      - Versión de depuración (con archivos intermedios)"
	@echo "\t runrelease - Ejecuta la versión final"
	@echo "\t rundebug   - Ejecuta la versión de depuración"
	@echo "\t valgrind   - Ejecuta con valgrind la versión de depuración"
	@echo "\t gdb        - Ejecuta con gdb la versión de depuración"
	@echo "\t clean      - Limpieza de archivos generados"
	@echo "\t cleanall   - Limpieza completa de directoriso generados"
	@echo "\t help       - Guia de uso del Makefile"

#---<compilacion>---

release: $(BIN_DIR)/$(NAME)

$(BIN_DIR)/$(NAME): $(SRCS)
	@$(MKDIR_P) $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS)

debug: $(BIN_DIR)/$(NAME)_debug

$(BIN_DIR)/$(NAME)_debug: $(OBJS)
	@$(MKDIR_P) $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR_P) $(OBJ_DIR)
	$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

#---<ejecucion>---

runrelease: release
	@$(BIN_DIR)/$(NAME)

rundebug: debug
	@$(BIN_DIR)/$(NAME)_debug

valgrind: debug
	@valgrind $(BIN_DIR)/$(NAME)_debug

gdb: debug
	@gdb $(BIN_DIR)/$(NAME)_debug

#---<limpieza>---

clean:
	$(RM) $(OBJ) $(BIN_DIR)/$(NAME) $(BIN_DIR)/$(NAME)_debug

cleanall:
	$(RM_RF) $(BIN_DIR) $(OBJ_DIR)

#  ╔══════╗
#  ║ SPEC ║
#  ╚══════╝ 

.PHONY: help clean cleanall runrelease rundebug valgrind gdb
