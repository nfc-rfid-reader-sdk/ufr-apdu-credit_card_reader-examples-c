/*
 * iso3166.h
 *
 *  Created on: 10.08.2017.
 *      Author: Zborac
 */

#ifndef ISO3166_H_
#define ISO3166_H_

#include <inttypes.h>

//==============================================================================
typedef struct iso3166_country_code_s {
	uint16_t num_code;
	char *alpha2_code;
	char *alpha3_code;
	char *country;
} iso3166_country_code_t;
//------------------------------------------------------------------------------
extern iso3166_country_code_t iso3166_country_codes[];
//------------------------------------------------------------------------------
int findCountryIndexByNumCode(uint16_t num_code);
//==============================================================================


#endif /* ISO3166_H_ */
