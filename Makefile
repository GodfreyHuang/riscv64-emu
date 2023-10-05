CC = gcc

APP_NAME = main
APP_DIR = ./bin/*

INCLUDES_POS = -I ./includes
SRC_POS = $(shell find ./src -name '*.c')
APP_SRC_FILES = $(APP_NAME).c
SRC_FILES += $(APP_SRC_FILES)
SRC_FILES += $(SRC_POS)

make:
	$(CC) $(SRC_FILES) -o $(APP_DIR)$(APP_NAME) $(INCLUDES_POS)

clean:
	rm -f $(APP_DIR)