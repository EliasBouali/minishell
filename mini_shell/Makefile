# ==========================
# Minishell — Makefile (Intel macOS + Linux)
# ==========================

NAME    := minishell

# Folders
INCDIR     := include
SRCDIR     := src
OBJDIR     := obj

MAINDIR    := $(SRCDIR)/main
EXECDIR    := $(SRCDIR)/executor
UTILSDIR   := $(SRCDIR)/utils
SIGNALSDIR := $(SRCDIR)/signals
BUILTINSDIR:= $(SRCDIR)/builtins
ENVDIR     := $(SRCDIR)/env

# ---- Sources
SRCS := \
  $(MAINDIR)/main.c \
  $(MAINDIR)/prompt.c \
  $(EXECDIR)/exec.c \
  $(EXECDIR)/heredoc.c \
  $(EXECDIR)/redirection.c \
  $(EXECDIR)/pipes.c \
  $(SIGNALSDIR)/signals.c \
  $(UTILSDIR)/utils.c \
  $(UTILSDIR)/second_utils.c \
  $(UTILSDIR)/three_utils.c \
  $(UTILSDIR)/pipes_utils.c \
  $(UTILSDIR)/pipes_second_utils.c \
	$(UTILSDIR)/pipes_third_utils.c \
	$(UTILSDIR)/pipes_four_utils.c \
	$(UTILSDIR)/exec_utils.c \
	$(UTILSDIR)/exec_second_utils.c \
  $(UTILSDIR)/exec_third_utils.c \
	$(UTILSDIR)/heredoc_utils.c \
	$(UTILSDIR)/heredoc_second_utils.c \
  $(UTILSDIR)/ft_split.c \
  $(UTILSDIR)/ft_strjoin.c \
  $(BUILTINSDIR)/builtins.c \
  $(BUILTINSDIR)/ft_echo.c \
  $(BUILTINSDIR)/ft_pwd.c \
  $(BUILTINSDIR)/ft_env.c \
  $(BUILTINSDIR)/ft_cd.c \
  $(BUILTINSDIR)/ft_export.c \
  $(BUILTINSDIR)/ft_exit.c \
  $(BUILTINSDIR)/ft_unset.c \
  $(ENVDIR)/env_init.c \
  $(ENVDIR)/env_node_new.c \
  $(ENVDIR)/env_set.c \
  $(ENVDIR)/env_to_envp.c \
  $(ENVDIR)/env_unset.c \
  $(ENVDIR)/free_env.c \
  $(ENVDIR)/get_env.c

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
else                       # Linux (42)
  RL_INC ?=
  RL_LIB ?= -lreadline -lncurses
endif

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(RL_INC) $(OBJS) $(RL_LIB) -o $@

# Build objects (et crée les sous-dossiers de obj/)
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(RL_INC) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

# ---- Optionnel : debug (binaire séparé) ----
debug: CFLAGS += -g
debug: fclean $(NAME)

# ---- Optionnel : norminette ----
norm:
	norminette $(SRCDIR) $(INCDIR) | grep -v "OK!" || true

.PHONY: all clean fclean re debug norm
