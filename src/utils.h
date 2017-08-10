/*
 * utils.h
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdbool.h>

#define DEFAULT_LINE_LEN		60

void print_ln_len(char symbol, uint8_t cnt);
void print_ln(char symbol);
void print_hex(const uint8_t *data, uint32_t len, const char *delimiter);
void print_hex_ln(const uint8_t *data, uint32_t len, const char *delimiter);
int bin_bcd_to_i(const uint8_t *bin, uint32_t len);
bool isByteArrayPrintable(const uint8_t *s, uint32_t len);
size_t hex2bin(uint8_t *dst, const char *src);

#endif /* UTILS_H_ */
