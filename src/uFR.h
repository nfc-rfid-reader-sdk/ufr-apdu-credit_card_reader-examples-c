/*
 * uFR.h
 */

#ifndef UFR_H_
#define UFR_H_

#include "ini.h"

//------------------------------------------------------------------------------
typedef const char * sz_ptr;
//------------------------------------------------------------------------------
UFR_STATUS ApduCommand(sz_ptr apdu, char **response, uint8_t *sw);
size_t hex2bin(uint8_t *dst, const char *src);
bool CheckDependencies(void);
sz_ptr GetDlTypeName(uint8_t dl_type_code);
//------------------------------------------------------------------------------

#endif /* UFR_H_ */
