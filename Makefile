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
#║ --- target ---                       ║
#║ NAME      target name                ║
#║ ARGS      arguments to target        ║
#║                                      ║
#║ --- directories ---                  ║
#║ SRC_DIR   source directory           ║
#║ INC_DIR   include directory          ║
#║ OBJ_DIR   target object directory    ║
#║ BIN_DIR   target binary directory    ║
#║                                      ║
#║ --- tools ---                        ║
#║ CC        compiler                   ║
#║ LEX       lexical analyzer generator ║
#║                                      ║
#║ --- flags ---                        ║
#║ CFLAGS    compiler flags             ║
#║ DFLAGS    compiler debug extra files ║
#║ LDFLAGS   compiler link flags        ║
#║                                      ║
#║ --- files ---                        ║
#║ L_IN      flex input file            ║
#║ L_TARGET  flex source files          ║
#║ SRCS      source files               ║
#║ OBJS      object files               ║
#║ GEN_SRCS  generated source files     ║
#║ GEN_OBJS  generated object files     ║
#╚══════════════════════════════════════╝

NAME      := dianho
ARGS      ?= regression.d
 
SRC_DIR   := src
INC_DIR   := inc
OBJ_DIR   := obj
BIN_DIR   := bin

CC        := gcc
LEX       := flex

CFLAGS    := -Wall -I$(INC_DIR)
DFLAGS    := -Wextra -g -DDEBUG -DWARNING
LDFLAGS   := 

L_IN	  := $(SRC_DIR)/lexico.l
L_TARGET  := $(SRC_DIR)/lex.yy.c

SRCS      := $(shell find $(SRC_DIR) -type f -name "*.c")
OBJS      := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
GEN_SRCS  := $(L_TARGET)
GEN_OBJS  := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(GEN_SRCS))

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
#║ $(GEN_SRCS)    generación de código            ║
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

$(BIN_DIR)/$(NAME): $(SRCS) $(GEN_SRCS)
	@$(MKDIR_P) $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

debug: $(BIN_DIR)/$(NAME)_debug

$(BIN_DIR)/$(NAME)_debug: $(OBJS) $(GEN_OBJS)
	@$(MKDIR_P) $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR_P) $(OBJ_DIR)
	$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

#---<generacion>---
$(GEN_SRCS): $(L_IN)
	$(LEX) -o $(L_TARGET) $(L_IN)

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
	$(RM_RF) $(BIN_DIR) $(OBJ_DIR) $(L_TARGET)

#  ╔══════╗
#  ║ SPEC ║
#  ╚══════╝ 

.PHONY: help clean cleanall runrelease rundebug valgrind gdb
