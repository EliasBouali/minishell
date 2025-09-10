# ===== Project =====
NAME      := minishell
CC        := gcc
CFLAGS    := -Wall -Wextra -Werror -Iinclude -I/usr/local/opt/readline/include
LDFLAGS   := -L/usr/local/opt/readline/lib
LDLIBS    := -lreadline

# ===== Sources =====
SRC_BUILTINS := $(wildcard src/builtins/*.c)
SRC_ENV      := $(wildcard src/env/*.c)
SRC_EXEC     := $(wildcard src/executor/*.c)
SRC_MAIN     := $(wildcard src/main/*.c)
SRC_SIGNALS  := $(wildcard src/signals/*.c)
SRC_UTILS    := $(wildcard src/utils/*.c)

# Nouveaux dossiers
SRC_LEXER    := $(wildcard src/lexer/*.c)
SRC_PARSER   := $(wildcard src/parser/*.c)
SRC_LCHECK   := $(wildcard src/line_checker/*.c)
SRC_BRIDGE   := $(wildcard src/bridge/*.c)

SRCS := $(SRC_BUILTINS) $(SRC_ENV) $(SRC_EXEC) $(SRC_MAIN) $(SRC_SIGNALS) \
        $(SRC_UTILS) $(SRC_LEXER) $(SRC_PARSER) $(SRC_LCHECK) $(SRC_BRIDGE)

OBJS := $(SRCS:%.c=obj/%.o)

# ===== Rules =====
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

