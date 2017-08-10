/*
 * iso4217.h
 *
 *  Created on: 09.08.2017.
 *      Author: d-logic
 */

#ifndef ISO4217_H_
#define ISO4217_H_

#include <inttypes.h>

//==============================================================================
typedef struct iso4217_currency_code_s {
	uint16_t num_code;
	char *alpha_code;
	char *currency;
} iso4217_currency_code_t;
//------------------------------------------------------------------------------
extern iso4217_currency_code_t iso4217_currency_codes[];
//------------------------------------------------------------------------------
int findCurrencyIndexByNumCode(uint16_t num_code);
//==============================================================================

#endif /* ISO4217_H_ */
