# SPDX-License-Identifier: 0BSD
# Copyright (C) 2026 Grigoriy Loboda

SOURCES := 	hal.h 			\
		hal.c			\
		main.c			\
		puzzle.c		\
		puzzle.h

CLANG_FORMAT := clang-format
CLANG_FORMAT_OPTIONS := --Werror
CLANG_FORMAT_FIX_OPTIONS := $(CLANG_FORMAT_OPTIONS) -i
CLANG_FORMAT_CHECK_OPTIONS := $(CLANG_FORMAT_OPTIONS) --dry-run
CLANG_FORMAT_SOURCES := $(SOURCES)

PHONY += clang-format-check
clang-format-check:
	$(CLANG_FORMAT) $(CLANG_FORMAT_CHECK_OPTIONS) $(CLANG_FORMAT_SOURCES)

PHONY += clang-format-fix
clang-format-fix:
	$(CLANG_FORMAT) $(CLANG_FORMAT_FIX_OPTIONS) $(CLANG_FORMAT_SOURCES)

PHONY += help
help:
	@echo "  Syntax: make [TARGET]... [VARIABLE=value]..."
	@echo
	@echo "  clang-format-check - use clang-format to check for styling violations"
	@echo "  clang-format-fix   - use clang-format to fix styling violations"
	@echo "  help               - display this help message"

.PHONY: PHOMY
