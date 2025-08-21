# ==========================
# Minishell — Makefile (Intel macOS + Linux)
# ==========================

NAME    := minishell

# Folders
INCDIR   := include
SRCDIR   := src
OBJDIR   := obj

MAINDIR    := $(SRCDIR)/main
EXECDIR    := $(SRCDIR)/executor
UTILSDIR   := $(SRCDIR)/utils
SIGNALSDIR := $(SRCDIR)/signals
BUILTINSDIR:= $(SRCDIR)/builtins

# ---- Sources
# NOTE: point d'entrée actuel : src/executor/main.c
SRCS := \
  $(EXECDIR)/main.c \
  $(MAINDIR)/prompt.c \
  $(EXECDIR)/exec.c \
  $(SIGNALSDIR)/signals.c \
  $(UTILSDIR)/utils.c \
  $(UTILSDIR)/second_utils.c \
  $(UTILSDIR)/ft_split.c \
  $(UTILSDIR)/ft_strjoin.c \
  $(BUILTINSDIR)/builtins.c \
  $(BUILTINSDIR)/ft_echo.c \
	$(BUILTINSDIR)/ft_pwd.c \
	$(BUILTINSDIR)/ft_env.c \
	$(BUILTINSDIR)/ft_cd.c \
	$(BUILTINSDIR)/ft_export.c \
	$(BUILTINSDIR)/ft_exit.c \

OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Toolchain
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -I$(INCDIR)

# ---- Readline (auto macOS/Linux) ----
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)   # macOS Intel (Homebrew /usr/local par défaut)
  BREW_PREFIX ?= /usr/local
  RL_INC ?= -I$(BREW_PREFIX)/opt/readline/include
  RL_LIB ?= -L$(BREW_PREFIX)/opt/readline/lib -lreadline -lncurses
else                       # Linux
  RL_INC ?=
  RL_LIB ?= -lreadline -lncurses
endif

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(RL_LIB) -o $@

# Build objects (et crée les sous-dossiers de obj/)
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(RL_INC) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
