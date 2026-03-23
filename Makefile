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

NAME    := lexer
ARGS    ?= regression.d
SRC_DIR := src
INC_DIR := inc
OBJ_DIR := obj
BIN_DIR := bin
CC      := gcc
CFLAGS  := -Wall -I$(INC_DIR)
DFLAGS  := -Wextra -g -DDEBUG -DWARNING
LDFLAGS := 
LEX     := flex
LEX_SRC := $(SRC_DIR)/lex.yy.c
LEX_OBJ := $(OBJ_DIR)/lex.yy.o
SRCS    := $(shell find $(SRC_DIR) -type f -name "*.c")
OBJS    := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS)) $(LEX_OBJ)

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

release: $(LEX_SRC) $(BIN_DIR)/$(NAME)

$(BIN_DIR)/$(NAME): $(LEX_SRC) $(SRCS)
	@$(MKDIR_P) $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS)

debug: $(LEX_SRC) $(BIN_DIR)/$(NAME)_debug

$(BIN_DIR)/$(NAME)_debug: $(LEX_OBJ) $(OBJS)
	@$(MKDIR_P) $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR_P) $(OBJ_DIR)
	$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

#---<flex>---
$(LEX_SRC): $(SRC_DIR)/lexico.l # Generación del .c desde el .l
	$(LEX) -o $@ $<

$(LEX_OBJ): $(LEX_SRC)          # Compilación del .c generado
	@$(MKDIR_P) $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#---<ejecucion>---

runrelease: release
	@$(BIN_DIR)/$(NAME) $(ARGS)

rundebug: debug
	@$(BIN_DIR)/$(NAME)_debug $(ARGS)

valgrind: debug
	@valgrind $(BIN_DIR)/$(NAME)_debug $(ARGS)

gdb: debug
	@gdb -ex "run $(ARGS)" --args $(BIN_DIR)/$(NAME)_debug $(ARGS)

#---<limpieza>---

clean:
	$(RM) $(OBJ) $(BIN_DIR)/$(NAME) $(BIN_DIR)/$(NAME)_debug

cleanall:
	$(RM_RF) $(BIN_DIR) $(OBJ_DIR)

#  ╔══════╗
#  ║ SPEC ║
#  ╚══════╝ 

.PHONY: help clean cleanall runrelease rundebug valgrind gdb
