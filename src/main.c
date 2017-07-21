/*
 ============================================================================
 Project Name: uFR APDU Credit Card Reader Example
 Name        : main.c
 Author      : d-logic
 Version     :
 Copyright   : 2017.
 Description : uFR APDU Credit Card Reader Example in C, (C99)
 Dependencies: uFR firmware - min. version 3.9.39
 uFRCoder library - min. version 4.0.26
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <uFCoder.h>
#include "ini.h"
#include "uFR.h"
#include "utils.h"
//------------------------------------------------------------------------------
#define MAX_BUFF_LEN	256
//------------------------------------------------------------------------------
typedef enum E_MENU_ITEMS {
	TOP_MENU_LEVEL, APDU_MENU_LEVEL
} menu_levels_t;
menu_levels_t menu_level = TOP_MENU_LEVEL;

void menu(char key);
void usage(void);
UFR_STATUS NewCardInField(uint8_t sak, uint8_t *uid, uint8_t uid_size);
//------------------------------------------------------------------------------
int main(void) {
	char key;
	bool card_in_field = false;
	uint8_t old_sak = 0, old_uid_size = 0, old_uid[10];
	uint8_t sak, uid_size, uid[10];
	UFR_STATUS status;

	usage();
	printf(" --------------------------------------------------\n");
	printf("     Please wait while opening uFR NFC reader.\n");
	printf(" --------------------------------------------------\n");

	status = ReaderOpen();
	if (status != UFR_OK) {
		printf(" Error while opening device, status is: 0x%08X\n", status);
		getchar();
		return EXIT_FAILURE;
	}
	if (!CheckDependencies()) {
		ReaderClose();
		getchar();
		return EXIT_FAILURE;
	}
	status = ReaderReset();
	if (status != UFR_OK) {
		printf(" Error while opening device, status is: 0x%08X\n", status);
		getchar();
		return EXIT_FAILURE;
	}

	printf(" --------------------------------------------------\n");
	printf("        uFR NFC reader successfully opened.\n");
	printf(" --------------------------------------------------\n");

	do {
		while (!_kbhit()) {
			status = GetCardIdEx(&sak, uid, &uid_size);
			switch (status) {
			case UFR_OK:
				if (card_in_field) {
					if (old_sak != sak || old_uid_size != uid_size || memcmp(old_uid, uid, uid_size)) {
						old_sak = sak;
						old_uid_size = uid_size;
						memcpy(old_uid, uid, uid_size);
						NewCardInField(sak, uid, uid_size);
					}
				} else {
					old_sak = sak;
					old_uid_size = uid_size;
					memcpy(old_uid, uid, uid_size);
					NewCardInField(sak, uid, uid_size);
					card_in_field = true;
				}
				break;
			case UFR_NO_CARD:
				menu_level = TOP_MENU_LEVEL;
				card_in_field = false;
				status = UFR_OK;
				break;
			default:
				ReaderClose();
				printf(" Fatal error while trying to read card, status is: 0x%08X\n", status);
				getchar();
				return EXIT_FAILURE;
			}
			Sleep(500);
		}

		key = _getch();
		menu(key);
	} while (key != '\x1b');

	ReaderClose();
	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------------
void menu(char key) {

	switch (key) {
	case '1':
		if (menu_level == APDU_MENU_LEVEL)
			ReadSystemFileOnST_M24SR();
		break;

	case '2':
		if (menu_level == APDU_MENU_LEVEL)
			ReadNfcCC();
		break;

	case '3':
		if (menu_level == APDU_MENU_LEVEL)
			ReadNfcNdef();
		break;

	case '\x1b':
		break;

	default:
		usage();
		break;
	}
}
//------------------------------------------------------------------------------
void usage(void) {
	switch (menu_level) {
	case APDU_MENU_LEVEL:
		printf(" -------------------------------------------------------------------\n");
		printf(" ISO14443-4 tag detected, choose one of the supported APDU commands:\n");
		printf(" -------------------------------------------------------------------\n");
		break;

	default:
		printf(" +------------------------------------------------+\n"
				" |      uFR APDU Credit Card Reader Example      |\n"
				" |                 version "APP_VERSION"                    |\n"
				" +------------------------------------------------+\n");
		printf(" When You put ISO14443-4 tag in the reader field,\n"
				" You will be prompted for appropriate APDU to send.\n"
				"\n"
				"                              For exit, hit escape.\n");
		printf(" --------------------------------------------------\n");
		break;
	}
}
//------------------------------------------------------------------------------
UFR_STATUS NewCardInField(uint8_t sak, uint8_t *uid, uint8_t uid_size) {
	UFR_STATUS status;
	uint8_t dl_card_type;

	status = GetDlogicCardType(&dl_card_type);
	if (status != UFR_OK)
		return status;

	printf(" \a-------------------------------------------------------------------\n");
	printf(" Card type: %s, sak = 0x%02X, uid[%d] = ", GetDlTypeName(dl_card_type), sak, uid_size);
	print_hex_ln(uid, uid_size, ":");
	printf(" -------------------------------------------------------------------\n");

	if (dl_card_type == DL_UNKNOWN_ISO_14443_4) {
		menu_level = APDU_MENU_LEVEL;
		usage();
	} else if (menu_level == APDU_MENU_LEVEL) {
		menu_level = TOP_MENU_LEVEL;
	}

	return UFR_OK;
}
//------------------------------------------------------------------------------
void ReadSystemFileOnST_M24SR(void) {
	UFR_STATUS status;
	size_t response_len;
	uint8_t response[MAX_BUFF_LEN];
	char *resp = NULL;
	uint8_t sw[2];
	uint32_t le;
	uint16_t *sw16_ptr = (uint16_t *) &sw;

	printf(" -------------------------------------------------------------------\n");
	printf("                         Read file on tag\n");
	printf(" -------------------------------------------------------------------\n");
	status = SetISO14443_4_Mode();
	if (status != UFR_OK) {
		printf(" Error while switching into ISO 14443-4 mode, uFR status is: 0x%08X\n", status);
		return;
	}

	printf(" 1. Issuing \"NDEF Tag Application Select\" command:\n"
			"  [C] 00 A4 04 00 07 D2 76 00 00 85 01 01 00\n");
	response_len = MAX_BUFF_LEN;
	status = ApduCommand("00 A4 04 00 07 D2760000850101 00", &resp, sw);
	if (status != UFR_OK) {
		s_block_deselect(100);
		printf(" Error while executing APDU command, uFR status is: 0x%08X\n", status);
		return;
	} else {
		if (*sw16_ptr != 0x90) {
			s_block_deselect(100);
			printf(" Could not continue execution due to an APDU error.\n");
			printf(" [SW] ");
					print_hex_ln(sw, 2, " ");
			return;
		}
		response_len = strlen(resp);
		printf(" APDU command executed: response data length = %d\n", (int) response_len);
		printf("  [R] %s\n", resp);
		printf(" [SW] ");
		print_hex_ln(sw, 2, " ");
		printf(" -------------------------------------------------------------------\n");
	}
	s_block_deselect(100);
}
//------------------------------------------------------------------------------
