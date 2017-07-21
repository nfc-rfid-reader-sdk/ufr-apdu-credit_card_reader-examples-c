/*
 * uFR.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <uFCoder.h>
#include "ini.h"
#include "uFR.h"

//------------------------------------------------------------------------------
size_t hex2bin(uint8_t *dst, const char *src) {
	size_t dst_len = 0;
	char s_tmp[3];

	s_tmp[2] = '\0';

	while (*src) {
		while (((char)*src < '0' || (char)*src > '9')
				&& ((char)*src < 'a' || (char)*src > 'f')
				&& ((char)*src < 'A' || (char)*src > 'F'))
			src++; // skip delimiters, white spaces, etc.

		s_tmp[0] = (char) *src++;

		// Must be pair of the hex digits:
		if (!(*src))
			break;

		// And again, must be pair of the hex digits:
		if (((char)*src < '0' || (char)*src > '9')
				&& ((char)*src < 'a' || (char)*src > 'f')
				&& ((char)*src < 'A' || (char)*src > 'F'))
			break;

		s_tmp[1] = (char) *src++;

		*dst++ = strtoul(s_tmp, NULL, 16);
		dst_len++;
	}

	return dst_len;
}
//------------------------------------------------------------------------------
UFR_STATUS ApduCommand(sz_ptr apdu, char **response, uint8_t *sw) {
	UFR_STATUS status;

	status = APDUHexStrTransceive(apdu, response);
	if (status)
		return status;

	if (strlen(*response) % 2)
		return UFR_PARAMETERS_ERROR;

	if (strlen(*response) >= 4) {
		hex2bin(sw, *response + strlen(*response) - 4);
		*(*response + strlen(*response) - 4) = '\0';
	}
	else
		return UFR_PARAMETERS_ERROR;

	return UFR_OK;
}
//------------------------------------------------------------------------------
bool CheckDependencies(void) {
#if defined(EXIT_ON_WRONG_FW_DEPENDENCY) || defined(EXIT_ON_WRONG_LIB_DEPENDENCY)
	uint8_t version_major, version_minor, build;
	bool wrong_version = false;
#endif
	UFR_STATUS status;

#ifdef EXIT_ON_WRONG_LIB_DEPENDENCY
#endif
#ifdef EXIT_ON_WRONG_FW_DEPENDENCY
	wrong_version = false;
	status = GetReaderFirmwareVersion(&version_major, &version_minor);
	if (status != UFR_OK) {
		printf(" Error while checking device version, status is: 0x%08X\n", status);
		return false;
	}
	status = GetBuildNumber(&build);
	if (status != UFR_OK) {
		printf(" Error while checking device version, status is: 0x%08X\n", status);
		return false;
	}
	if (version_major < MIN_DEPEND_FW_VER_MAJOR) {
		wrong_version = true;
	} else if (version_major == MIN_DEPEND_FW_VER_MAJOR && version_minor < MIN_DEPEND_FW_VER_MINOR) {
		wrong_version = true;
	} else if (version_minor == MIN_DEPEND_FW_VER_MINOR && build < MIN_DEPEND_FW_VER_BUILD) {
		wrong_version = true;
	}

	if (wrong_version) {
		printf("Wrong uFR NFC reader firmware version (%d.%d.%d).\n"
				"Please update uFR firmware to at last %d.%d.%d version.\n", version_major, version_minor, build,
		MIN_DEPEND_FW_VER_MAJOR, MIN_DEPEND_FW_VER_MINOR, MIN_DEPEND_FW_VER_BUILD);
		return false;
	}
#endif
	return true;
}
//------------------------------------------------------------------------------
sz_ptr GetDlTypeName(uint8_t dl_type_code) {

	switch (dl_type_code) {
	case DL_MIFARE_ULTRALIGHT:
		return "DL_MIFARE_ULTRALIGHT";
	case DL_MIFARE_ULTRALIGHT_EV1_11:
		return "DL_MIFARE_ULTRALIGHT_EV1_11";
	case DL_MIFARE_ULTRALIGHT_EV1_21:
		return "DL_MIFARE_ULTRALIGHT_EV1_21";
	case DL_MIFARE_ULTRALIGHT_C:
		return "DL_MIFARE_ULTRALIGHT_C";
	case DL_NTAG_203:
		return "DL_NTAG_203";
	case DL_NTAG_210:
		return "DL_NTAG_210";
	case DL_NTAG_212:
		return "DL_NTAG_212";
	case DL_NTAG_213:
		return "DL_NTAG_213";
	case DL_NTAG_215:
		return "DL_NTAG_215";
	case DL_NTAG_216:
		return "DL_NTAG_216";
	case DL_MIKRON_MIK640D:
		return "DL_MIKRON_MIK640D";
	case DL_MIFARE_MINI:
		return "DL_MIFARE_MINI";
	case DL_MIFARE_CLASSIC_1K:
		return "DL_MIFARE_CLASSIC_1K";
	case DL_MIFARE_CLASSIC_4K:
		return "DL_MIFARE_CLASSIC_4K";
	case DL_MIFARE_PLUS_S_2K:
		return "DL_MIFARE_PLUS_S_2K";
	case DL_MIFARE_PLUS_S_4K:
		return "DL_MIFARE_PLUS_S_4K";
	case DL_MIFARE_PLUS_X_2K:
		return "DL_MIFARE_PLUS_X_2K";
	case DL_MIFARE_PLUS_X_4K:
		return "DL_MIFARE_PLUS_X_4K";
	case DL_MIFARE_DESFIRE:
		return "DL_MIFARE_DESFIRE";
	case DL_MIFARE_DESFIRE_EV1_2K:
		return "DL_MIFARE_DESFIRE_EV1_2K";
	case DL_MIFARE_DESFIRE_EV1_4K:
		return "DL_MIFARE_DESFIRE_EV1_4K";
	case DL_MIFARE_DESFIRE_EV1_8K:
		return "DL_MIFARE_DESFIRE_EV1_8K";
	case DL_UNKNOWN_ISO_14443_4:
		return "DL_GENERIC_ISO_14443_4";
	case DL_IMEI_UID:
		return "DL_IMEI_UID";
	}
	return "UNSUPPORTED CARD";
}
//------------------------------------------------------------------------------
