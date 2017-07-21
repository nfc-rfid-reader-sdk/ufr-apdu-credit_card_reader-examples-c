/*
 * utils.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "ini.h"
#include "utils.h"

//------------------------------------------------------------------------------
void print_ln_len(char symbol, uint8_t cnt) {

	for (int i = 0; i < cnt; i++)
		printf("%c", symbol);

	printf("\n");
}
//------------------------------------------------------------------------------
inline void print_ln(char symbol) {

	print_ln_len(symbol, DEFAULT_LINE_LEN);
}
//------------------------------------------------------------------------------
void print_hex(const uint8_t *data, uint32_t len, const char *delimiter) {

	for (int i = 0; i < len; i++) {
		printf("%02X", data[i]);
		if ((delimiter != NULL) && (i < (len - 1)))
			printf("%c", *delimiter);
	}
}
//------------------------------------------------------------------------------
void print_hex_ln(const uint8_t *data, uint32_t len, const char *delimiter) {

	print_hex(data, len, delimiter);
	printf("\n");
}
//==============================================================================

