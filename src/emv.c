/*
 * emv.c
 *
 *  Created on: 25.07.2017.
 *      Author: d-logic
 */

#include <stdio.h>
#include <stdlib.h>
#include <uFCoder.h>
#include "uFR.h"
#include "utils.h"
#include "iso3166.h"
#include "iso4217.h"
#include "emv.h"
//------------------------------------------------------------------------------
emv_tags_t emv_tags[] = {
		{0x9f01, "Acquirer Identifier", BIN, 2},
		{0x9f02, "Amount, Authorised (Numeric)", BIN, 2},
		{0x9f03, "Amount, Other (Numeric)", BIN, 2},
		{0x9f04, "Amount, Other (Binary)", BIN, 2},
		{0x9f05, "Application Discretionary Data", BIN, 2},
		{0x9f06, "Application Identifier (AID) - terminal", BIN, 2},
		{0x9f07, "Application Usage Control", BIN, 2},
		{0x9f08, "Application Version Number", BIN, 2},
		{0x9f09, "Application Version Number", BIN, 2},
		{0x9f0b, "Cardholder Name Extended", BIN, 2},
		{0xbf0c, "FCI Issuer Discretionary Data", NODE, 2},
		{0x9f0d, "Issuer Action Code - Default", BIN, 2},
		{0x9f0e, "Issuer Action Code - Denial", BIN, 2},
		{0x9f0f, "Issuer Action Code - Online", BIN, 2},
		{0x9f10, "Issuer Application Data", BIN, 2},
		{0x9f11, "Issuer Code Table Index", BIN, 2},
		{0x9f12, "Application Preferred Name", BIN, 2},
		{0x9f13, "Last Online Application Transaction Counter (ATC) Register", BIN, 2},
		{0x9f14, "Lower Consecutive Offline Limit", BIN, 2},
		{0x9f15, "Merchant Category Code", BIN, 2},
		{0x9f16, "Merchant Identifier", BIN, 2},
		{0x9f17, "Personal Identification Number (PIN) Try Counter", BIN, 2},
		{0x9f18, "Issuer Script Identifier", BIN, 2},
		{0x9f1a, "Terminal Country Code", ISO3166_COUNTRY, 2},
		{0x9f1b, "Terminal Floor Limit", BIN, 2},
		{0x9f1c, "Terminal Identification", BIN, 2},
		{0x9f1d, "Terminal Risk Management Data", BIN, 2},
		{0x9f1e, "Interface Device (IFD) Serial Number", BIN, 2},
		{0x9f1f, "Track 1 Discretionary Data", BIN, 2},
		{0x5f20, "Cardholder Name", STR, 2},
		{0x9f21, "Transaction Time", BIN, 2},
		{0x9f22, "Certification Authority Public Key Index", BIN, 2},
		{0x9f23, "Upper Consecutive Offline Limit", BIN, 2},
		{0x5f24, "Application Expiration Date", DATE_YMD, 2},
		{0x5f25, "Application Effective Date", DATE_YMD, 2},
		{0x9f26, "Application Cryptogram", BIN, 2},
		{0x9f27, "Cryptogram Information Data", BIN, 2},
		{0x5f28, "Issuer Country Code", ISO3166_COUNTRY, 2},
		{0x5f2a, "Transaction Currency Code", ISO4217_CURRENCY, 2},
		{0x5f2d, "Language Preference", LANGUAGE_CODE_PAIRS, 2},
		{0x9f2e, "Integrated Circuit Card (ICC) PIN Encipherment Public Key Exponent", BIN, 2},
		{0x9f2f, "Integrated Circuit Card (ICC) PIN Encipherment Public Key Remainder", BIN, 2},
		{0x5f30, "Service Code", BIN, 2},
		{0x9f32, "Issuer Public Key Exponent", BIN, 2},
		{0x9f33, "Terminal Capabilities", BIN, 2},
		{0x5f34, "Application PAN Sequence Number (PSN)", BIN, 2},
		{0x9f35, "Terminal Type", BIN, 2},
		{0x5f36, "Transaction Currency Exponent", BIN, 2},
		{0x9f37, "Unpredictable Number", BIN, 2},
		{0x9f38, "Processing Options Data Object List (PDOL)", TL_LIST, 2},
		{0x9f34, "Cardholder Verification Method (CVM) Results", BIN, 2},
		{0x9f3a, "Amount, Reference Currency", BIN, 2},
		{0x9f3b, "Application Reference Currency", BIN, 2},
		{0x9f3c, "Transaction Reference Currency Code", BIN, 2},
		{0x9f3d, "Transaction Reference Currency Exponent", BIN, 2},
		{0x9f40, "Additional Terminal Capabilities", BIN, 2},
		{0x9f41, "Transaction Sequence Counter", BIN, 2},
		{0x9f43, "Application Reference Currency Exponent", BIN, 2},
		{0x9f44, "Application Currency Exponent", BIN, 2},
		{0x9f2d, "Integrated Circuit Card (ICC) PIN Encipherment Public Key Certificate", BIN, 2},
		{0x9f46, "Integrated Circuit Card (ICC) Public Key Certificate", BIN, 2},
		{0x9f47, "Integrated Circuit Card (ICC) Public Key Exponent", BIN, 2},
		{0x9f48, "Integrated Circuit Card (ICC) Public Key Remainder", BIN, 2},
		{0x9f49, "Dynamic Data Authentication Data Object List (DDOL)", BIN, 2},
		{0x9f4a, "Static Data Authentication Tag List", BIN, 2},
		{0x9f4b, "Signed Dynamic Application Data", BIN, 2},
		{0x9f4c, "ICC Dynamic Number", BIN, 2},
		{0x9f4d, "Log Entry", BIN, 2},
		{0x9f4e, "Merchant Name and Location", BIN, 2},
		{0x9f51, "Application Currency Code", BIN, 2},
		{0x9f52, "Card Verification Results (CVR)", BIN, 2},
		{0x5f53, "International Bank Account Number (IBAN)", BIN, 2},
		{0x5f54, "Bank Identifier Code (BIC)", BIN, 2},
		{0x5f55, "Issuer Country Code (alpha2 format)", BIN, 2},
		{0x5f56, "Issuer Country Code (alpha3 format)", BIN, 2},
		{0x9f58, "Lower Consecutive Offline Limit (Card Check)", BIN, 2},
		{0x9f59, "Upper Consecutive Offline Limit (Card Check)", BIN, 2},
		{0x9f5c, "Cumulative Total Transaction Amount Upper Limit", BIN, 2},
		{0x9f72, "Consecutive Transaction Limit (International - Country)", BIN, 2},
		{0x9f65, "Track 2 Bit Map for CVC3", BIN, 2},
		{0x9f66, "Track 2 Bit Map for UN and ATC", BIN, 2},
		{0x9f68, "Mag Stripe CVM List", BIN, 2},
		{0x9f69, "Unpredictable Number Data Object List (UDOL)", BIN, 2},
		{0x9f6b, "Track 2 Data", BIN, 2},
		{0x9f6c, "Mag Stripe Application Version Number (Card)", BIN, 2},
		{0x9f6e, "Third Party Data", BIN, 2},
		{0x9f74, "VLP Issuer Authorization Code", BIN, 2},
		{0x9f75, "Cumulative Total Transaction Amount Limit - Dual Currency", BIN, 2},
		{0x9f76, "Secondary Application Currency Code", ISO4217_CURRENCY, 2},
		{0x9f7d, "Unknown Tag", BIN, 2},
		{0x9f7f, "Card Production Life Cycle (CPLC) History File Identifiers", BIN, 2},
		{0x9f45, "Data Authentication Code", BIN, 2},
		{0x9f57, "Issuer Country Code", BIN, 2},
		{0x9f39, "Point-of-Service (POS) Entry Mode", BIN, 2},
		{0x9f73, "Currency Conversion Factor", BIN, 2},
		{0x9f42, "Application Currency Code", ISO4217_CURRENCY, 2},
		{0x9f56, "Issuer Authentication Indicator", BIN, 2},
		{0x9f20, "Track 2 Discretionary Data", BIN, 2},
		{0xdf01, "Reference PIN", BIN, 2},
		{0x9f36, "Application Transaction Counter (ATC)", BIN, 2},
		{0x9f4f, "Log Format", TL_LIST, 2},
		{0x5f50, "Issuer URL", BIN, 2},
		{0x9f5a, "Issuer URL2", BIN, 2},
		{0x9f53, "Consecutive Transaction Limit (International)", BIN, 2},
		{0x9f54, "Cumulative Total Transaction Amount Limit", BIN, 2},
		{0x9f55, "Geographic Indicator", BIN, 2},
		{0x42, "Issuer Identification Number (IIN)", BIN, 1},
		{0x4f, "Application Identifier (AID)", BIN, 1},
		{0x50, "Application Label", STR, 1},
		{0x57, "Track 2 Equivalent Data", BIN, 1},
		{0x5a, "Application Primary Account Number (PAN)", BCD_4BY4, 1},
		{0x61, "Application Template", NODE, 1},
		{0x6f, "File Control Information (FCI) Template", NODE, 1},
		{0x70, "Response Message Template / AEF Data Template", NODE, 1},
		{0x71, "Issuer Script Template 1", BIN, 1},
		{0x72, "Issuer Script Template 2", BIN, 1},
		{0x73, "Directory Discretionary Template", BIN, 1},
		{0x77, "Response Message Template Format 2", NODE, 1},
		{0x80, "Response Message Template Format 1", BIN, 1},
		{0x81, "Amount, Authorised (Binary)", BIN, 1},
		{0x82, "Application Interchange Profile", BIN, 1},
		{0x83, "Command Template", BIN, 1},
		{0x84, "Dedicated File (DF) Name", BIN_OR_STR, 1},
		{0x86, "Issuer Script Command", BIN, 1},
		{0x87, "Application Priority Indicator", BIN, 1},
		{0x88, "Short File Identifier (SFI)", BIN, 1},
		{0x89, "Authorisation Code", BIN, 1},
		{0x8a, "Authorisation Response Code", BIN, 1},
		{0x8c, "Card Risk Management Data Object List 1 (CDOL1)", BIN, 1},
		{0x8d, "Card Risk Management Data Object List 2 (CDOL2)", BIN, 1},
		{0x8e, "Cardholder Verification Method (CVM) List", BIN, 1},
		{0x8f, "Certification Authority Public Key Index", BIN, 1},
		{0x90, "Issuer Public Key Certificate", BIN, 1},
		{0x91, "Issuer Authentication Data", BIN, 1},
		{0x92, "Issuer Public Key Remainder", BIN, 1},
		{0x93, "Signed Static Application Data", BIN, 1},
		{0x94, "Application File Locator (AFL)", BIN, 1},
		{0x95, "Terminal Verification Results", BIN, 1},
		{0x97, "Transaction Certificate Data Object List (TDOL)", BIN, 1},
		{0x98, "Transaction Certificate (TC) Hash Value", BIN, 1},
		{0x99, "Transaction Personal Identification Number (PIN) Data", BIN, 1},
		{0x9a, "Transaction Date", DATE_YMD, 1},
		{0x9b, "Transaction Status Information", BIN, 1},
		{0x9c, "Transaction Type", BIN, 1},
		{0x9d, "Directory Definition File (DDF) Name", BIN, 1},
		{0xa5, "File Control Information (FCI) Proprietary Template", NODE, 1},
		{0, "UNKNOWN TAG", BIN, 0}
};

char * months[] = {"", "jan.", "feb.", "mar.", "apr.", "may ", "jun.", "jul.", "aug.", "sep.", "oct.", "nov.", "dec."};
//------------------------------------------------------------------------------
emv_tag_index_t findEmvTagIndex(emv_tag_t tag)
{
	emv_tag_index_t i = 0;

	do {
		if (emv_tags[i].tag == tag)
			break;
		i++;
	} while (emv_tags[i].tag_id_len != 0);

	return i;
}
//------------------------------------------------------------------------------
void print_tab(int tabulator)
{
	for (int i = 0; i < tabulator; i++)
		printf(" ");
}
//------------------------------------------------------------------------------
void printEmvNode(emv_tree_node_t *tag_node, int tabulator)
{
	char *s;
	int temp;
    bool end_ln = true;

	print_tab(tabulator);
	printf("<> tag=");
	switch (tag_node->tag_bytes) {
	case 1:
		printf("%02X", tag_node->tag);
		break;
	case 2:
		printf("%04X", tag_node->tag);
		break;
	case 3:
		printf("%06X", tag_node->tag);
		break;
	default:
		printf("%08X", tag_node->tag);
		break;
	}
	printf(" length=%d\n", tag_node->value_len);
	print_tab(tabulator);
	printf("   desc: %s\n", tag_node->description);

	// print value:
	if (tag_node->value) {
		print_tab(tabulator);
		printf("   value: ");
		switch (tag_node->tag_type) {
		case STR:
			printf("\"%s\"", (char *) tag_node->value);
			break;
		case LANGUAGE_CODE_PAIRS:
			if (!tag_node->value_len || tag_node->value_len % 2) {
				print_hex(tag_node->value, tag_node->value_len, NULL);
			} else {
				temp = 0;
				while (temp < tag_node->value_len) {
					s = malloc(3);
					if (s != NULL) {
						memcpy(s, tag_node->value + temp, 2);
						s[2] = '\0';
						printf("%s", s);
						free(s);
					} else {
						print_hex(tag_node->value, tag_node->value_len, NULL);
						break;
					}
					temp += 2;
					if (temp < tag_node->value_len) {
						printf(", ");
					}
				}
			}
			break;
		case BCD_4BY4:
			if (tag_node->value_len == 8)
			{
				print_hex(tag_node->value, 2, NULL);
				printf("-");
				print_hex(tag_node->value + 2, 2, NULL);
				printf("-");
				print_hex(tag_node->value + 4, 2, NULL);
				printf("-");
				print_hex(tag_node->value + 6, 2, NULL);
			}
			else
				printf("%d", (int)*((uint8_t *)tag_node->value));
			break;
		case DEC_UINT8:
			printf("%d", (int)*((uint8_t *)tag_node->value));
			break;
		case DEC_UINT16:
			printf("%d", (int)*((uint16_t *)tag_node->value));
			break;
		case DEC_UINT32:
			printf("%d", (int)*((uint32_t *)tag_node->value));
			break;
        case ISO3166_COUNTRY:
			temp = findCountryIndexByNumCode(bin_bcd_to_i(tag_node->value, 2));
			printf("%s (%s)", iso3166_country_codes[temp].alpha2_code, iso3166_country_codes[temp].country);
			break;
		case ISO4217_CURRENCY:
			temp = findCurrencyIndexByNumCode(bin_bcd_to_i(tag_node->value, 2));
			printf("%s (%s)", iso4217_currency_codes[temp].alpha_code, iso4217_currency_codes[temp].currency);
			break;
		case DATE_YMD:
			if (tag_node->value_len == 3)
			{
				print_hex(tag_node->value + 2, 1, NULL);
				printf(". %s 20", months[bin_bcd_to_i(tag_node->value + 1, 1)]);
				print_hex(tag_node->value, 1, NULL);
			}
			else
				print_hex(tag_node->value, tag_node->value_len, NULL);
			break;
		case BIN_OR_STR:
			if (isByteArrayPrintable(tag_node->value, tag_node->value_len)) {
				s = malloc(tag_node->value_len + 1);
				if (s != NULL) {
					memcpy(s, tag_node->value, tag_node->value_len);
					s[tag_node->value_len] = '\0';
					printf("\"%s\"", s);
					free(s);
				} else {
					print_hex(tag_node->value, tag_node->value_len, NULL);
				}
			} else
				print_hex(tag_node->value, tag_node->value_len, NULL);
			break;
		case TL_LIST:
			print_hex_ln(tag_node->value, tag_node->value_len, NULL);
			print_tab(tabulator + TAB_STEP);
			printf("parsed list (from value):\n");
			printEmvBranch(tag_node->tl_list_format, tabulator + 2 * TAB_STEP);
			end_ln = false;
			break;
		case BIN:
			print_hex(tag_node->value, tag_node->value_len, NULL);
			break;
		default:
			break;
		}

		if (end_ln)
			printf("\n");
	}
}
//------------------------------------------------------------------------------
void printEmvBranch(emv_tree_node_t *tag_node, int tabulator)
{

	while (tag_node)
	{
		printEmvNode(tag_node, tabulator);
		if (tag_node->subnode != NULL) {
			printEmvBranch(tag_node->subnode, tabulator + TAB_STEP);
		}
		tag_node = tag_node->next;
	}
}
//------------------------------------------------------------------------------
EMV_STATUS getSfi(emv_tree_node_t *tag_node, uint8_t *sfi) {

	if (!tag_node)
		return EMV_ERR_TAG_NOT_FOUND;

	if (tag_node->tag == 0x88) {
		if (tag_node->value_len == 1) {
			*sfi = tag_node->value[0];
			return EMV_OK;
		} else {
			return EMV_ERR_TAG_WRONG_SIZE;
		}
	} else {
		if (tag_node->subnode) {
			return getSfi(tag_node->subnode, sfi);
		} else {
			return getSfi(tag_node->next, sfi);
		}
	}
}
//------------------------------------------------------------------------------
EMV_STATUS getAID(emv_tree_node_t *tag_node, uint8_t *aid, uint8_t *aid_len)
{
	if (!tag_node)
		return EMV_ERR_TAG_NOT_FOUND;

	if (tag_node->tag == 0x4F) {
		if (tag_node->value_len < 17) {
			memcpy(aid, tag_node->value, tag_node->value_len);
			*aid_len = (uint8_t)tag_node->value_len;
			return EMV_OK;
		} else {
			return EMV_ERR_TAG_WRONG_SIZE;
		}
	} else {
		if (tag_node->subnode) {
			return getAID(tag_node->subnode, aid, aid_len);
		} else {
			return getAID(tag_node->next, aid, aid_len);
		}
	}
}
//------------------------------------------------------------------------------
EMV_STATUS parseEmvTag(uint8_t *tag_ptr, emv_tag_t *tag, uint8_t **tag_val, int32_t *tag_len, int32_t *tag_len_len, int32_t *tag_val_len)
{
	// Tag parsing rules: there is at least 2 bytes if (MSB & 0x1F) == 0x1F
	//                    and if (2nd_byte & 0x80) == 0x80 there is third byte:
	*tag = *tag_ptr++;
	*tag_len = 1;
	if ((*tag & 0x1F) == 0x1F) {
		*tag <<= 8;
		*tag |= *tag_ptr;
		(*tag_len)++;
		if ((*tag_ptr++ & 0x80) == 0x80) {
			*tag <<= 8;
			*tag |= *tag_ptr++;
			(*tag_len)++;
		}
	}

	// Length:
	*tag_len_len = 1;
	*tag_val_len = *tag_ptr;
	if ((*tag_ptr & 0x80) == 0x80) {
		*tag_len_len += *tag_ptr & 0x7F;
	}

	if (*tag_len_len > MAX_TAG_LEN_BYTES)
		return EMV_ERR_MAX_TAG_LEN_BYTES_EXCEEDED;

	tag_ptr++;
	if (*tag_len_len > 1) {
		*tag_val_len = 0;
		for (int i = *tag_len_len - 1 ; i > 0; i--) {
			*tag_val_len |= *tag_ptr++ << ((i - 1) * 8);
		}
	}

	*tag_val = tag_ptr;

	return EMV_OK;
}
//------------------------------------------------------------------------------
EMV_STATUS newEmvTag(emv_tree_node_t **head, uint8_t *input, int32_t input_bytes_left, bool is_list_format)
{
	emv_tree_node_t *p;
	emv_tag_index_t tag_index = 0;
	emv_tag_t tag = 0;
	uint8_t *tag_val;
	int32_t tag_len = 0, tag_len_len = 0, tag_val_len = 0;
	int32_t temp;
	bool is_node_type;
	EMV_STATUS status;

	status = parseEmvTag(input, &tag, &tag_val, &tag_len, &tag_len_len, &tag_val_len);
	if (status != EMV_OK) {
		return status;
	}

	tag_index = findEmvTagIndex(tag);
	is_node_type = (emv_tags[tag_index].tag_type == NODE);

	temp = tag_len + tag_len_len;
	if (!is_node_type && !is_list_format) {
		temp += tag_val_len;
	}

	input_bytes_left -= temp;
	input += temp;

	p = malloc(sizeof(emv_tree_node_t));
	if (p == NULL) {
		return SYS_ERR_OUT_OF_MEMORY;
	} else {
		*head = p;
	}
	p->is_node_type = is_node_type;
	p->tag = tag;
	p->tag_bytes = tag_len;
	p->tag_type = emv_tags[tag_index].tag_type;
	p->description = emv_tags[tag_index].description; // pointer to constant in "code segment"
	p->tl_list_format = NULL;
	p->subnode = NULL;
	p->next = NULL;
	p->value = NULL;
	p->value_len = tag_val_len;

	if (!(p->is_node_type) && !is_list_format && tag_val_len) {
		if (p->tag_type == STR)
			temp = tag_val_len + 1;
		else
			temp = tag_val_len;

		p->value = malloc(temp);
		if (p->value == NULL) {
			return SYS_ERR_OUT_OF_MEMORY;
		}

		p->value_len = tag_val_len;
		memcpy(p->value, tag_val, tag_val_len);
		if (p->tag_type == STR)
			((char *)(p->value))[tag_val_len] = '\0';
	}

	if (p->tag_type == TL_LIST) {
		status = newEmvTag(&(p->tl_list_format), p->value, p->value_len, true);
	}

	if ((input_bytes_left < 0) || (is_node_type && (input_bytes_left != tag_val_len)))
		return EMV_ERR_WRONG_INPUT_DATA;
	else if (input_bytes_left > 0)
	{
		// Parsing not finished:
		if (p->is_node_type) {
			status = newEmvTag(&(p->subnode), input, input_bytes_left, false);
		} else {
			status = newEmvTag(&(p->next), input, input_bytes_left, is_list_format);
		}
		if (status != EMV_OK) {
			return status;
		}
	}

	return EMV_OK;
}
//------------------------------------------------------------------------------
void emvTreeCleanup(emv_tree_node_t *head) {
	emv_tree_node_t *temp;

	while (head) {
		if (head->subnode) {
			emvTreeCleanup(head->subnode);
		}
		if (head->value)
			free(head->value);
		temp = head->next;
		free(head);
		head = temp;
	}
}
//------------------------------------------------------------------------------
// uint8_t *r_apdu - minimum of the 256 bytes have to be allocated before call
EMV_STATUS emvReadRecord(uint8_t *r_apdu, uint32_t *Ne, uint8_t sfi, uint8_t record, uint8_t sw[2]) {
	UFR_STATUS status;
	uint16_t *sw16_ptr = (uint16_t *) sw;

	sfi <<= 3;
	sfi |= 4;

	*Ne = 256;
	status = APDUTransceive(0x00, 0xB2, record, sfi, NULL, 0, r_apdu, Ne, 1, sw);
	if (status != UFR_OK)
		return EMV_ERR_IN_CARD_READER;

	if (sw[0] == 0x6C) {
		*Ne = sw[1];
		status = APDUTransceive(0x00, 0xB2, record, sfi, NULL, 0, r_apdu, Ne, 1, sw);
		if (status != UFR_OK)
			return EMV_ERR_IN_CARD_READER;
	} else if (*sw16_ptr == 0x8262)
		*sw16_ptr = 0x90;

	if (*sw16_ptr != 0x90)
		return EMV_ERR_READING_RECORD;

	return EMV_OK;
}
//------------------------------------------------------------------------------
