// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#include "print.h"

#define PR_BUF_SIZE 128
#define RING_INC(x) (x = (x + 1) % PR_BUF_SIZE)

char pr_buf[PR_BUF_SIZE];

unsigned char write_index;
unsigned char read_index;

void pr_char(char c)
{
	pr_buf[write_index] = c;
	RING_INC(write_index);
}

void pr_string(const char* msg)
{
	while (msg[0] != '\0') {
		pr_buf[write_index] = msg[0];
		RING_INC(write_index);
		msg++;
	}
}

char pr_getch(void)
{
	char result = pr_buf[read_index];

	RING_INC(read_index);
	return result;
}

unsigned char pr_empty(void)
{
	return read_index == write_index;
}

char hex_digits[] = "0123456789ABCDEF";

void pr_hex(unsigned char value)
{
	unsigned char low = hex_digits[value & 0xF];
	unsigned char high = hex_digits[(value >> 4) & 0xF];

	pr_char(high);
	pr_char(low);
}

void pr_binary(unsigned char value)
{
	char buf[9];

	buf[8] = '\0';
	/* unusual condition because of unsigned overflow */
	for (unsigned char i = 7; i < 8; i--) {
		buf[i] = '0' + (value & 0x1);
		value >>= 1;
	}
	pr_string(buf);
}
