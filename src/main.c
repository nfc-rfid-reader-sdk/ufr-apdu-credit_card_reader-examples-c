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
#include "emv.h"
#include "uFR.h"
#include "utils.h"
//------------------------------------------------------------------------------
#define RAW_RES_MAX_LEN		258
#define MAX_AID_LEN			16
//------------------------------------------------------------------------------
typedef enum E_MENU_ITEMS {
	TOP_MENU_LEVEL, APDU_MENU_LEVEL
} menu_levels_t;
menu_levels_t menu_level = TOP_MENU_LEVEL;
//------------------------------------------------------------------------------
void menu(char key);
void usage(void);
UFR_STATUS NewCardInField(uint8_t sak, uint8_t *uid, uint8_t uid_size);
void checkEmvPse(const char *df_name, const char *szTitlePse);
void checkEmvPse1(void);
void checkEmvPse2(void);
void tryEmvPseCardRead(const char *df_name, const char *szTitlePse);
void tryEmvPse1CardRead(void);
void tryEmvPse2CardRead(void);
//------------------------------------------------------------------------------
int main(void) {
	char kbd_key;
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

		kbd_key = _getch();
		menu(kbd_key);
	} while (kbd_key != '\x1b');

	ReaderClose();
	return EXIT_SUCCESS;
}
//------------------------------------------------------------------------------
void menu(char key) {

	switch (key) {
	case '1':
		if (menu_level == APDU_MENU_LEVEL)
			checkEmvPse1();
		break;

	case '2':
		if (menu_level == APDU_MENU_LEVEL)
			checkEmvPse2();
		break;

	case '3':
		if (menu_level == APDU_MENU_LEVEL)
			tryEmvPse1CardRead();
		break;

	case '4':
		if (menu_level == APDU_MENU_LEVEL)
			tryEmvPse2CardRead();
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
		printf("                   ISO14443-4 tag detected:\n");
		printf(" -------------------------------------------------------------------\n");
		printf("  (1) - Check if the card support Payment System Environment (PSE1)\n"
			   "  (2) - Check if the card support Payment System Environment (PSE2)\n"
			   "  (3) - Read and parse EMV card supporting PSE1\n"
			   "  (4) - Read and parse EMV card supporting PSE2\n");
		printf(" -------------------------------------------------------------------\n");
		break;

	default:
		printf(" +------------------------------------------------+\n"
				" |         APDU usage with uFR example            |\n"
				" |                 version "APP_VERSION"                    |\n"
				" +------------------------------------------------+\n");
		printf(" When You put ISO14443-4 tag in the reader field,\n"
				" You will be prompted for appropriate scenario.\n"
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
void checkEmvPse(const char *df_name, const char *szTitlePse)
{
	emv_tree_node_t *head = NULL, *tail = NULL, *temp = NULL;
	uint8_t r_apdu[RAW_RES_MAX_LEN];
	uint32_t Ne; // without SW
	uint8_t sw[2];
	uint8_t sfi, record, cnt;
	uint16_t *sw16_ptr = (uint16_t *) &sw;
	EMV_STATUS emv_status;
	UFR_STATUS status;

	printf(" ===================================================================\n");
	printf("  Checking if if the card support Payment System Environment (%s) \n", szTitlePse);
	printf(" -------------------------------------------------------------------\n");

	do {
		status = SetISO14443_4_Mode();
		if (status != UFR_OK) {
			printf(" Error while switching into ISO 14443-4 mode, uFR status is: 0x%08X\n", status);
			break;
		}

		printf(" 1. Issuing \"Select PSE\" command (\"%s\"):\n"
			   "  [C] 00 A4 04 00 ", df_name);
		print_hex((const uint8_t *)df_name, strlen(df_name), " ");
		printf(" 00\n");
		Ne = 256;
		status = APDUTransceive(0x00, 0xA4, 0x04, 0x00, (const uint8_t *)df_name, strlen(df_name), r_apdu, &Ne, 1, sw);
		if (status != UFR_OK) {
			printf(" Error while executing APDU command, uFR status is: 0x%08X\n", status);
			break;
		} else {
			if (*sw16_ptr != 0x90) {
				printf(" [SW] ");
				print_hex_ln(sw, 2, " ");
				printf(" Could not continue execution due to an APDU error.\n");
				break;
			}
			if (Ne) {
				printf(" APDU command executed: response data length = %d bytes\n", (int) Ne);
				printf("  [R] ");
				print_hex_ln(r_apdu, Ne, " ");
			}
			printf(" [SW] ");
			print_hex_ln(sw, 2, " ");
		}

		emv_status = newEmvTag(&head, r_apdu, Ne, false);
		if (emv_status) {
			printf(" EMV parsing error code: 0x%08X", emv_status);
			break;
		}

		emv_status = getSfi(head, &sfi);
		if (emv_status == EMV_OK) {

			// There is SFI:
			cnt = 2;
			record = 1;
			do {
				printf("\n %d. Issuing \"Read Record\" command (record = %d, sfi = %d):\n"
							   "  [C] 00 B2 %02X %02X 00\n", cnt, record, sfi, record, (sfi << 3) | 4);

				emv_status = emvReadRecord(r_apdu, &Ne, sfi, record, sw);
				if (emv_status == EMV_OK) {
					emv_status = newEmvTag(&temp, r_apdu, Ne, false);
					if (record == 1)
						head->next = tail = temp;
					else {
						tail->next = temp;
						tail = tail->next;
					}
					if (Ne) {
						printf(" APDU command executed: response data length = %d bytes\n", (int) Ne);
						printf("  [R] ");
						print_hex_ln(r_apdu, Ne, " ");
					}
					printf(" [SW] ");
					print_hex_ln(sw, 2, " ");
				} else {
					if (*sw16_ptr != 0x90) {
						printf(" [SW] ");
						print_hex_ln(sw, 2, " ");
						printf(" There is no records.\n");
					}
				}
				record++;
				cnt++;
			} while (emv_status == EMV_OK);
		}

		printf("\n-------------------------------------------------------------------\n");
		printEmvBranch(head, 0);
		printf(" -------------------------------------------------------------------\n");
		printf("          Card support Payment System Environment (%s)\n", szTitlePse);

	} while (0);

	printf(" ===================================================================\n");
	emvTreeCleanup(head);

	s_block_deselect(100);
}
//------------------------------------------------------------------------------
void checkEmvPse1(void) {

	checkEmvPse("1PAY.SYS.DDF01", "PSE1");
}
//------------------------------------------------------------------------------
void checkEmvPse2(void) {

	checkEmvPse("2PAY.SYS.DDF01", "PSE2");
}
//------------------------------------------------------------------------------
void tryEmvPseCardRead(const char *df_name, const char *szTitlePse) {
	EMV_STATUS emv_status;
	UFR_STATUS status;
	bool head_attached = false;
	emv_tree_node_t *head = NULL, *tail = NULL, *temp = NULL;
	afl_list_item_t *afl_list = NULL, *afl_list_item = NULL;
	uint8_t afl_list_count;
//	char *sz_hex_r_apdu;
	uint8_t r_apdu[RAW_RES_MAX_LEN];
	uint32_t Ne; // without SW
	uint8_t sw[2], aid[MAX_AID_LEN];
	uint8_t sfi, record, cnt = 1, aid_len;
	uint16_t *sw16_ptr = (uint16_t *) &sw;
	uint8_t *gpo_data_field = NULL;
	uint16_t gpo_data_field_size;

	printf(" ===================================================================\n");
	printf("            Read and parse EMV card supporting %s \n", szTitlePse);
	printf(" -------------------------------------------------------------------\n");

	do {
		status = SetISO14443_4_Mode();
		if (status != UFR_OK) {
			printf(" Error while switching into ISO 14443-4 mode, uFR status is: 0x%08X\n", status);
			break;
		}

		printf(" %d. Issuing \"Select PSE\" command (\"%s\"):\n"
			   "  [C] 00 A4 04 00 %02X ", cnt++, df_name, (unsigned)strlen(df_name));
		print_hex((uint8_t *)df_name, strlen(df_name), " ");
		printf(" 00\n");
		Ne = 256;
		status = APDUTransceive(0x00, 0xA4, 0x04, 0x00, (uint8_t *)df_name, strlen(df_name), r_apdu, &Ne, 1, sw);
		if (status != UFR_OK) {
			printf(" Error while executing APDU command, uFR status is: 0x%08X\n", status);
			break;
		} else {
			if (*sw16_ptr != 0x90) {
				printf(" [SW] ");
				print_hex_ln(sw, 2, " ");
				printf(" Could not continue execution due to an APDU error.\n");
				break;
			}
			if (Ne) {
				printf(" APDU command executed: response data length = %d bytes\n", (int) Ne);
				printf("  [R] ");
				print_hex_ln(r_apdu, Ne, " ");
			}
			printf(" [SW] ");
			print_hex_ln(sw, 2, " ");
		}

		emv_status = newEmvTag(&head, r_apdu, Ne, false);
		if (emv_status) {
			printf(" EMV parsing error code: 0x%08X", emv_status);
			break;
		}

		emv_status = getSfi(head, &sfi);
		if (emv_status == EMV_OK) {

			// There is SFI:
			record = 1;
			do {
				printf("\n %d. Issuing \"Read Record\" command (record = %d, sfi = %d):\n"
							   "  [C] 00 B2 %02X %02X 00\n", cnt, record, sfi, record, (sfi << 3) | 4);

				emv_status = emvReadRecord(r_apdu, &Ne, sfi, record, sw);
				if (emv_status == EMV_OK) {
					emv_status = newEmvTag(&temp, r_apdu, Ne, false);
					if (!head_attached) {
						head->next = tail = temp;
						head_attached = true;
					} else {
						tail->next = temp;
						tail = tail->next;
					}
					if (Ne) {
						printf(" APDU command executed: response data length = %d bytes\n", (int) Ne);
						printf("  [R] ");
						print_hex_ln(r_apdu, Ne, " ");
					}
					printf(" [SW] ");
					print_hex_ln(sw, 2, " ");
				} else {
					if (*sw16_ptr != 0x90) {
						printf(" [SW] ");
						print_hex_ln(sw, 2, " ");
						printf(" There is no records.\n");
					}
				}
				record++;
				cnt++;
			} while (emv_status == EMV_OK);
		}

		emv_status = getAid(head, aid, &aid_len);
		if (emv_status == EMV_OK) {
			printf("\n %d. Issuing \"Select the application\" command:\n"
					   "  [C] 00 A4 04 00 %02X ", cnt++, aid_len);
			print_hex(aid, aid_len, " ");
			printf(" 00\n");
			Ne = 256;
			status = APDUTransceive(0x00, 0xA4, 0x04, 0x00, aid, aid_len, r_apdu, &Ne, 1, sw);
			if (status != UFR_OK) {
				printf(" Error while executing APDU command, uFR status is: 0x%08X\n", status);
				break;
			} else {
				if (*sw16_ptr != 0x90) {
					printf(" [SW] ");
					print_hex_ln(sw, 2, " ");
					printf(" Could not continue execution due to an APDU error.\n");
					break;
				}
				if (Ne) {
					printf(" APDU command executed: response data length = %d bytes\n", (int) Ne);
					printf("  [R] ");
					print_hex_ln(r_apdu, Ne, " ");
				}
				printf(" [SW] ");
				print_hex_ln(sw, 2, " ");
			}

			emv_status = newEmvTag(&temp, r_apdu, Ne, false);
			if (emv_status) {
				printf(" EMV parsing error code: 0x%08X", emv_status);
				break;
			}

			if (!head_attached) {
				head->next = tail = temp;
				head_attached = true;
			} else {
				tail->next = temp;
				tail = tail->next;
			}

			// Ovde se mora, ako postoji PDOL, sračunati dužinu bajtova i to poslati u narednoj komandi.
			// Ako PDOL sadrži Terminal Capabilities Tag '9F33' onda se po mnogima mogu poslati sve nule.
			// Ako PDOL sadrži Terminal Transaction Qualifiers (TTQ) Tag '9F66' onda tu treba postaviti bar "28 00 00 00"
			//                 da ne bi kartica vratila grešku "69 85" = "Conditions of use not satisfied". Radi i sa
            //                 "20 00 00 00".
			// Posle ovoga pronaći Application File Locator (AFL) Tag '94' za dalje parsiranje - "sfi" & "record range".
			// Ako ne postoji PDOL, Onda se šalje GPO komanda u formi: "80 A8 00 00 02 83 00 00" nakon koje se očekuje
			// da kartica vrati Response Message Template Format 1 Tag '80' {Contains the data objects (without tags and lengths)
			//        returned by the ICC in response to a command}. U tom slučaju prva dva bajta u okviru vraćene binarne vrednosti
			//        Taga '80' predstavljaju AIP a ostatak AFL u grupama po 4 bajta koje treba parsirati isto kao Tag '94':
			//        - prvi bajt je SFI šiftovan 3 bita levo (za read record cmd samo treba još orovati sa 4)
			//        - drugi bajt je prvi record tog SFI-a
			//        - treći bajt je poslednji record tog SFI-a
			//        - broj zapisa koji učestvuju u "offline data authentication" tog SFI-a počevši od prvog record-a.
			// Ako kartica ne vrati Tag '80' onda treba očekivati da vrati tagove '82' za AIP i '94' za AFL.

			printf("\n %d. Formating GET PROCESSING OPTIONS instruction (checking PDOL).\n", cnt++);
			emv_status = formatGetProcessingOptionsDataField(temp, &gpo_data_field, &gpo_data_field_size);
			if (emv_status) {
				printf(" EMV parsing error code: 0x%08X", emv_status);
				break;
			}

			printf("\n %d. Issuing \"Get processing options\" command:\n"
				   "  [C] 80 A8 00 00 %02X ", cnt++, gpo_data_field_size);
			print_hex(gpo_data_field, gpo_data_field_size, " ");
			printf(" 00\n");
			Ne = 256;
			status = APDUTransceive(0x80, 0xA8, 0x00, 0x00, gpo_data_field, gpo_data_field_size, r_apdu, &Ne, 1, sw);
			if (status != UFR_OK) {
				printf(" Error while executing APDU command, uFR status is: 0x%08X\n", status);
				break;
			} else {
				if (*sw16_ptr != 0x90) {
					printf(" [SW] ");
					print_hex_ln(sw, 2, " ");
					printf(" Could not continue execution due to an APDU error.\n");
					break;
				}
				if (Ne) {
					printf(" APDU command executed: response data length = %d bytes\n", (int) Ne);
					printf("  [R] ");
					print_hex_ln(r_apdu, Ne, " ");
				}
				printf(" [SW] ");
				print_hex_ln(sw, 2, " ");
			}

/*			status = ApduCommand("80 A8 00 00 15 83 13 28 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00", &sz_hex_r_apdu, sw);
			if (status != UFR_OK) {
				printf(" Error while executing APDU command, uFR status is: 0x%08X\n", status);
				break;
			} else {
				Ne = strlen(sz_hex_r_apdu) / 2;
				if (Ne) {
					printf(" APDU command executed: response data length = %d bytes\n", (int) Ne);
					printf("  [R] %s\n", sz_hex_r_apdu);
				}
				printf(" [SW] ");
				print_hex_ln(sw, 2, " ");
				if (*sw16_ptr != 0x90) {
					printf(" Could not continue execution due to an APDU error.\n");
				} else {
					hex2bin(r_apdu, sz_hex_r_apdu);
					emv_status = newEmvTag(&temp, r_apdu, Ne, false);
					...CHECK status...
					tail->next = temp;
					tail = tail->next;
				}
			}
*/
			emv_status = newEmvTag(&temp, r_apdu, Ne, false);
			if (emv_status) {
				printf(" EMV parsing error code: 0x%08X", emv_status);
				break;
			}
			tail->next = temp;
			tail = tail->next;

			emv_status = getAfl(temp, &afl_list, &afl_list_count);
			if (emv_status == EMV_ERR_TAG_NOT_FOUND)
				emv_status = getAflFromResponseMessageTemplateFormat1(temp, &afl_list, &afl_list_count);
			if (emv_status) {
				printf(" EMV parsing error code: 0x%08X", emv_status);
				break;
			}

			afl_list_item = afl_list;
			while (afl_list_item) {
				for (int r = afl_list_item->record_first; r <= afl_list_item->record_last; r++) {
					printf("\n %d. Issuing \"Read Record\" command (record = %d, sfi = %d):\n"
								   "  [C] 00 B2 %02X %02X 00\n", cnt,
								   r, afl_list_item->sfi, r, (afl_list_item->sfi << 3) | 4);

					emv_status = emvReadRecord(r_apdu, &Ne, afl_list_item->sfi, r, sw);
					if (emv_status == EMV_OK) {
						emv_status = newEmvTag(&temp, r_apdu, Ne, false);
						if (emv_status == EMV_OK) {
							tail->next = temp;
							tail = tail->next;
						}
						if (Ne) {
							printf(" APDU command executed: response data length = %d bytes\n", (int) Ne);
							printf("  [R] ");
							print_hex_ln(r_apdu, Ne, " ");
						}
						printf(" [SW] ");
						print_hex_ln(sw, 2, " ");
					} else {
						if (*sw16_ptr != 0x90) {
							printf(" [SW] ");
							print_hex_ln(sw, 2, " ");
						}
					}
					cnt++;
				}
				afl_list_item = afl_list_item->next;
			}
//			if (outer_exit)
//				break;

		}
		printf("\n-------------------------------------------------------------------\n");
	} while (0);

	printEmvBranch(head, 0);

	printf(" ===================================================================\n");

	if (afl_list)
		emvAflListCleanup(afl_list);
	if (gpo_data_field)
		free(gpo_data_field);
	if (head)
		emvTreeCleanup(head);

	s_block_deselect(100);
}
//------------------------------------------------------------------------------
void tryEmvPse1CardRead(void) {

	tryEmvPseCardRead("1PAY.SYS.DDF01", "PSE1");
}
//------------------------------------------------------------------------------
void tryEmvPse2CardRead(void) {

	tryEmvPseCardRead("2PAY.SYS.DDF01", "PSE2");
}
//------------------------------------------------------------------------------
