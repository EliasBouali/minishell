# =======================
#        Minishell
# =======================

SHELL := /bin/sh
.NOTPARALLEL:

NAME    := minishell
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -Iinclude
LDFLAGS :=
LDLIBS  :=

# --- Détection simple de readline (portable) ---
UNAME_S := $(shell uname -s)
RL_PKG_CFLAGS := $(shell pkg-config --cflags readline 2>/dev/null)
RL_PKG_LIBS   := $(shell pkg-config --libs   readline 2>/dev/null)

# 1) Si pkg-config fonctionne
ifneq ($(RL_PKG_LIBS),)
  CFLAGS  += $(RL_PKG_CFLAGS)
  LDFLAGS += $(filter -L%,$(RL_PKG_LIBS))
  LDLIBS  += $(filter-out -L%,$(RL_PKG_LIBS))
else
  # 2) Fallback simple
  ifeq ($(UNAME_S),Darwin)
    CFLAGS  += -I/opt/homebrew/opt/readline/include -I/usr/local/opt/readline/include
    LDFLAGS += -L/opt/homebrew/opt/readline/lib     -L/usr/local/opt/readline/lib
    LDLIBS  += -lreadline
  else
    LDLIBS  += -lreadline -lncurses
  endif
endif

# ===== Sources (pas de wildcard) =====
SRC_BUILTINS = \
	src/builtins/builtins.c \
	src/builtins/ft_cd.c \
	src/builtins/ft_echo.c \
	src/builtins/ft_env.c \
	src/builtins/ft_exit.c \
	src/builtins/ft_export.c \
	src/builtins/ft_pwd.c \
	src/builtins/ft_unset.c

SRC_ENV = \
	src/env/env_init.c \
	src/env/env_node_new.c \
	src/env/env_set.c \
	src/env/env_to_envp.c \
	src/env/env_unset.c \
	src/env/free_env.c \
	src/env/get_env.c

SRC_EXEC = \
	src/executor/exec.c \
	src/executor/heredoc.c \
	src/executor/pipes.c \
	src/executor/redirection.c

SRC_MAIN = \
	src/main/main.c \
	src/main/prompt.c

SRC_SIGNALS = \
	src/signals/signals.c

SRC_LEXER = \
	src/lexer/lexer.c \
	src/lexer/lexer_utils0.c \
	src/lexer/lexer_utils1.c \
	src/lexer/lexer_utils2.c \
	src/lexer/lexer_utils3.c \
	src/lexer/lexer_utils4.c \
	src/lexer/lexer_utils5.c \
	src/lexer/lexer_utils6.c

SRC_PARSER = \
	src/parser/parser.c \
	src/parser/parser_utils0.c \
	src/parser/parser_utils1.c \
	src/parser/parser_utils2.c \
	src/parser/print.c

SRC_LCHECK = \
	src/line_checker/line_check1.c \
	src/line_checker/line_check2.c

SRC_BRIDGE = \
	src/bridge/normalize_cmds.c

SRC_UTILS = \
	src/utils/exec_second_utils.c \
	src/utils/exec_third_utils.c \
	src/utils/exec_utils.c \
	src/utils/five_utils.c \
	src/utils/four_utils.c \
	src/utils/ft_split.c \
	src/utils/ft_strjoin.c \
	src/utils/heredoc_second_utils.c \
	src/utils/heredoc_utils.c \
	src/utils/pipes_four_utils.c \
	src/utils/pipes_second_utils.c \
	src/utils/pipes_third_utils.c \
	src/utils/pipes_utils.c \
	src/utils/second_utils.c \
	src/utils/three_utils.c \
	src/utils/utils.c

SRCS = \
  $(SRC_BUILTINS) $(SRC_ENV) $(SRC_EXEC) $(SRC_MAIN) $(SRC_SIGNALS) \
  $(SRC_LEXER) $(SRC_PARSER) $(SRC_LCHECK) $(SRC_BRIDGE) $(SRC_UTILS)

OBJS := $(SRCS:%.c=obj/%.o)

# ===== Progress bar (compatible mac & linux, sans `seq`) =====
PROGRESS_FILE := .build_progress
BAR_WIDTH     := 30
TOTAL_OBJS    := $(words $(OBJS))
ifeq ($(TOTAL_OBJS),0)
  TOTAL_OBJS := 1
endif

define PROGRESS_STEP
	@sh -c '\
		printf . >> "$(PROGRESS_FILE)"; \
		count=$$(wc -c < "$(PROGRESS_FILE)" 2>/dev/null || echo 0); \
		prc=$$((count * 100 / $(TOTAL_OBJS))); \
		bar_len=$(BAR_WIDTH); \
		filled=$$((prc * bar_len / 100)); \
		empty=$$((bar_len - filled)); \
		printf "\r⏳  Progression: ["; \
		if [ $$filled -gt 0 ]; then \
			printf "%*s" $$filled "" | tr " " "█"; \
		fi; \
		if [ $$empty -gt 0 ]; then \
			printf "%*s" $$empty " "; \
		fi; \
		printf "] %3d%%  (%d/%d)" $$prc $$count $(TOTAL_OBJS); \
	'
endef

# =======================
#        Règles
# =======================

.PHONY: all build clean fclean re

all:
	@$(MAKE) -q $(NAME) >/dev/null 2>&1; \
	case $$? in \
	  0) printf "\n🚫 Déjà compilé — rien à faire. 🙅‍♂️\n\n";; \
	  1) rm -f "$(PROGRESS_FILE)"; \
	     printf "\n🛠️  Compilation en cours... ⏳\n\n"; \
	     if $(MAKE) -s --no-print-directory build; then \
	       printf "\n\n✅ Compilation terminée ! 🎉😄\n\n"; \
	     else \
	       printf "\n\n❌ Échec de la compilation. 🔥\n"; exit 2; \
	     fi;; \
	  *) printf "\n❌ Problème de dépendances.\n"; exit 2;; \
	esac

build: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@
	@printf "\n🔗 Link: $@ OK\n"

obj/%.o: %.c
	@mkdir -p "$(dir $@)"
	@$(CC) $(CFLAGS) -c $< -o $@
	$(PROGRESS_STEP)

clean:
	@printf "\n🧹  Nettoyage en cours... 🧹\n"
	@rm -rf obj
	@rm -f "$(PROGRESS_FILE)"
	@printf "✨ Nettoyage fini ! ✨\n\n"

fclean: clean
	@printf "🧹  Nettoyage binaire...\n"
	@rm -f $(NAME)
	@printf "✨ Nettoyage complet terminé ! 😌✨\n\n"

re: fclean all
