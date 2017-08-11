/*
 * emv.h
 *
 *  Created on: 25.07.2017.
 *      Author: d-logic
 */

#ifndef EMV_H_
#define EMV_H_
//------------------------------------------------------------------------------
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
//------------------------------------------------------------------------------
#define MAX_TAG_LEN_BYTES		3
#define TAB_STEP				3
//==============================================================================
typedef enum E_TAG_TYPE {
	STR = 10,
	LANGUAGE_CODE_PAIRS,
	BCD_4BY4,
	DEC_UINT8,
	DEC_UINT16,
	DEC_UINT32,
	ISO3166_COUNTRY,
	ISO4217_CURRENCY,
	DATE_YMD,
	BIN_OR_STR,
	BIN,
	//-------------------
	TL_LIST,
	NODE,
	//-------------------
} tag_type_t;
//------------------------------------------------------------------------------
typedef enum E_EMV_STATUS {
	EMV_OK,
	SYS_ERR_OUT_OF_MEMORY,
	EMV_ERR_WRONG_INPUT_DATA,
	EMV_ERR_MAX_TAG_LEN_BYTES_EXCEEDED,
	EMV_ERR_TAG_NOT_FOUND,
	EMV_ERR_TAG_WRONG_SIZE,
	EMV_ERR_IN_CARD_READER,
	EMV_ERR_READING_RECORD,
	EMV_ERR_PDOL_IS_EMPTY
} EMV_STATUS;
//==============================================================================
typedef uint32_t emv_tag_t;
typedef uint8_t emv_tag_index_t;
//------------------------------------------------------------------------------
typedef struct emv_tags_s {
	emv_tag_t tag;
	char *description;
	tag_type_t tag_type;
	uint8_t tag_id_len;
} emv_tags_t;
//------------------------------------------------------------------------------
typedef struct emv_tree_node_s emv_tree_node_t;
struct emv_tree_node_s {
	emv_tag_t tag;
	uint8_t tag_bytes;
	char *description;
	tag_type_t tag_type;
	bool is_node_type;
	uint8_t *value;
	uint32_t value_len;
	emv_tree_node_t *tl_list_format;
	emv_tree_node_t *next;
	emv_tree_node_t *subnode;
};
//------------------------------------------------------------------------------
typedef struct afl_list_item_s afl_list_item_t;
struct afl_list_item_s {
	uint8_t sfi;
	uint8_t record_first;
	uint8_t record_last;
	uint8_t record_num_offline_auth;
	afl_list_item_t *next;
};
//------------------------------------------------------------------------------
// Public function prototypes:
EMV_STATUS getSfi(emv_tree_node_t *tag_node, uint8_t *sfi);
EMV_STATUS getAid(emv_tree_node_t *tag_node, uint8_t *aid, uint8_t *aid_len);
EMV_STATUS getAfl(emv_tree_node_t *tag_node, afl_list_item_t **afl_list_item, uint8_t *afl_list_count);
EMV_STATUS getAflFromResponseMessageTemplateFormat1(emv_tree_node_t *tag_node, afl_list_item_t **afl_list_item, uint8_t *afl_list_count);
EMV_STATUS getPdol(emv_tree_node_t *tag_node, emv_tree_node_t **pdol);
// formatGetProcessingOptionsDataField()
// free() heap gpo_data_field point to when not needed any more...
EMV_STATUS formatGetProcessingOptionsDataField(emv_tree_node_t *tag_node, uint8_t **gpo_data_field, uint16_t *gpo_data_field_size);
void printEmvBranch(emv_tree_node_t *tag_node, int tabulator);
EMV_STATUS newEmvTag(emv_tree_node_t **head, uint8_t *input, int32_t input_bytes_left, bool is_list_format);
void emvTreeCleanup(emv_tree_node_t *head);
void emvAflListCleanup(afl_list_item_t *head);

// emvReadRecord()
// uint8_t *r_apdu - minimum of the 256 bytes have to be allocated before call
EMV_STATUS emvReadRecord(uint8_t *r_apdu, uint32_t *Ne, uint8_t sfi, uint8_t record, uint8_t sw[2]);
//==============================================================================

#endif /* EMV_H_ */
