# --- Variables ---
CC       = gcc
CFLAGS   = -O2 -Wall -Wextra
SRC_DIR  = src
INC_DIR  = include
OBJ_DIR  = obj
BIN      = c-batch-ci

SRC      = $(wildcard $(SRC_DIR)/*.c)
OBJ      = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TEST_SRC = $(wildcard tests/*.c)
TEST_BIN = test_runner

# Exclure main.c des tests
TEST_SRC_NO_MAIN := $(filter-out $(SRC_DIR)/main.c, $(SRC))

# --- Règle par défaut ---
all: $(BIN)

# --- Compilation du binaire principal ---
$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# --- Compilation des .o ---
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# --- Tests unitaires (cmocka) ---
test:
ifdef TEST_SRC
	$(CC) -Wall -Wextra -I$(INC_DIR) -o $(TEST_BIN) $(TEST_SRC) $(TEST_SRC_NO_MAIN) -lcmocka
	./$(TEST_BIN)
else
	@echo "Aucun test trouvé dans tests/"
endif

# --- Nettoyage ---
clean:
	rm -rf $(OBJ_DIR) $(BIN) $(TEST_BIN)

.PHONY: all clean test
