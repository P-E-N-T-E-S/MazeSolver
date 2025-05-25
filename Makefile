CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = maze_solver
SRC = main.c function.c

define MAZE_CMD
#################\n\
#S    #   #     #\n\
##### ### # ### #\n\
#   #   # # # # #\n\
# # ### # # # # #\n\
# # # # #     # #\n\
# # # # # ##### #\n\
# #   # # # #   #\n\
# ##### # # # ###\n\
#       #   #   #\n\
# ######### ### #\n\
# #           # #\n\
# ############# #\n\
#             # #\n\
############# # #\n\
#              E#\n\
#################
endef

export MAZE_CMD

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

run: $(TARGET)
	@printf "%b" "$$MAZE_CMD" | xargs -0 ./$(TARGET)

clean:
	rm -f $(TARGET) saida.txt

.PHONY: all clean run