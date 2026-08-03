// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#pragma once

/* print a single character, i.e. add a character to the print buffer (FIFO) */
void pr_char(char c);

/* print a string */
void pr_string(const char* msg);

/* pop earliest character from the print buffer (FIFO) */
char pr_getch(void);

/* is there a character in the print buffer? */
unsigned char pr_empty(void);

/* print an integer in binary form, without any prefix */
void pr_binary(unsigned char value);

/* print an integer in hexadecimal form, without any prefix */
void pr_hex(unsigned char value);
