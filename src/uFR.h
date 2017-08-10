/*
 * uFR.h
 */

#ifndef UFR_H_
#define UFR_H_

#include "inttypes.h"
#include "stdbool.h"
#include "ini.h"

#define MIN_APDU_HEX_STR_LEN	5
//------------------------------------------------------------------------------
UFR_STATUS ApduCommand(const char *apdu, char **response, uint8_t *sw);
bool CheckDependencies(void);
const char *GetDlTypeName(uint8_t dl_type_code);
//------------------------------------------------------------------------------

#endif /* UFR_H_ */
