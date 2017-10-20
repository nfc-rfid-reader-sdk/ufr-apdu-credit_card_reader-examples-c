/*
 * uFCoder.h
 *
 * library version: 4.0.30
 *
 * Created on:  2009-01-14
 * Last edited: 2017-08-15
 *
 * Author: D-Logic
 */

#ifndef uFCoder_H_
#define uFCoder_H_

#include <stdint.h>
#include <stddef.h>

////////////////////////////////////////////////////////////////////
/**
 * Type for representing null terminated char array ( aka C-String )
 * Array is always one byte longer ( for null character ) then string
 * Memory space for array must be allocated before use.
 */
typedef const char * c_string;
////////////////////////////////////////////////////////////////////

#ifdef _WIN32
// WINDOWS
#	if defined(DL_CREATE_STATIC_LIB) || defined(DL_USE_STATIC_LIB)
#		define DL_API
#	else
#		ifndef DL_uFC_EXPORTS
#			define DL_API /* __declspec(dllimport) */ __stdcall
#		else
#			define DL_API __declspec(dllexport) __stdcall
#		endif
#	endif
#else
// Linux & OS X
#	define DL_API
#endif // _WIN32

#if defined(DL_uFC_EXPORTS) || defined(DL_CREATE_STATIC_LIB)
	typedef struct S_UFR_HANDLE * UFR_HANDLE;
#else
	typedef void * UFR_HANDLE;
#endif

// MIFARE CLASSIC type id's:
#define MIFARE_CLASSIC_1k               0x08
#define MF1ICS50                        0x08
#define SLE66R35                        0x88 // Infineon = Mifare Classic 1k
#define MIFARE_CLASSIC_4k               0x18
#define MF1ICS70                        0x18
#define MIFARE_CLASSIC_MINI             0x09
#define MF1ICS20                        0x09

//DLOGIC CARD TYPE
#define TAG_UNKNOWN						0
#define DL_MIFARE_ULTRALIGHT			0x01
#define DL_MIFARE_ULTRALIGHT_EV1_11		0x02
#define DL_MIFARE_ULTRALIGHT_EV1_21		0x03
#define DL_MIFARE_ULTRALIGHT_C			0x04
#define DL_NTAG_203						0x05
#define DL_NTAG_210						0x06
#define DL_NTAG_212						0x07
#define DL_NTAG_213						0x08
#define DL_NTAG_215						0x09
#define DL_NTAG_216						0x0A
#define DL_MIKRON_MIK640D				0x0B
#define NFC_T2T_GENERIC					0x0C

#define DL_MIFARE_MINI					0x20
#define	DL_MIFARE_CLASSIC_1K			0x21
#define DL_MIFARE_CLASSIC_4K			0x22
#define DL_MIFARE_PLUS_S_2K				0x23
#define DL_MIFARE_PLUS_S_4K				0x24
#define DL_MIFARE_PLUS_X_2K				0x25
#define DL_MIFARE_PLUS_X_4K				0x26
#define DL_MIFARE_DESFIRE				0x27
#define DL_MIFARE_DESFIRE_EV1_2K		0x28
#define DL_MIFARE_DESFIRE_EV1_4K		0x29
#define DL_MIFARE_DESFIRE_EV1_8K		0x2A
#define DL_MIFARE_DESFIRE_EV2_2K		0x2B
#define DL_MIFARE_DESFIRE_EV2_4K		0x2C
#define DL_MIFARE_DESFIRE_EV2_8K		0x2D

#define DL_UNKNOWN_ISO_14443_4			0x40
#define DL_IMEI_UID						0x80

// ST Product ID-s:
#define M24SR02							0x82
#define M24SR02_AUTOMOTIVE				0x8A
#define M24SR04							0x86
#define M24SR04_AUTOMOTIVE				0x8E
#define M24SR16							0x85
#define M24SR16_AUTOMOTIVE				0x8D
#define M24SR64							0x84
#define M24SR64_AUTOMOTIVE				0x8C

// MIFARE CLASSIC Authentication Modes:
enum MIFARE_AUTHENTICATION
{
	MIFARE_AUTHENT1A = 0x60,
	MIFARE_AUTHENT1B = 0x61,
};

enum ADDRESS_MODE
{
	ADDRESS_MODE_BLOCK = 0,
	ADDRESS_MODE_SECTOR,
};

#define MAX_UID_LEN		10
#define ECC_SIG_LEN		32

// API Status Codes Type:
typedef enum UFCODER_ERROR_CODES
{
	UFR_OK = 0x00,
	UFR_COMMUNICATION_ERROR = 0x01,
	UFR_CHKSUM_ERROR = 0x02,
	UFR_READING_ERROR = 0x03,
	UFR_WRITING_ERROR = 0x04,
	UFR_BUFFER_OVERFLOW = 0x05,
	UFR_MAX_ADDRESS_EXCEEDED = 0x06,
	UFR_MAX_KEY_INDEX_EXCEEDED = 0x07,
	UFR_NO_CARD = 0x08,
	UFR_COMMAND_NOT_SUPPORTED = 0x09,
	UFR_FORBIDEN_DIRECT_WRITE_IN_SECTOR_TRAILER = 0x0A,
	UFR_ADDRESSED_BLOCK_IS_NOT_SECTOR_TRAILER = 0x0B,
	UFR_WRONG_ADDRESS_MODE = 0x0C,
	UFR_WRONG_ACCESS_BITS_VALUES = 0x0D,
	UFR_AUTH_ERROR = 0x0E,
	UFR_PARAMETERS_ERROR = 0x0F, // ToDo, point 5.
	UFR_MAX_SIZE_EXCEEDED = 0x10,
	UFR_UNSUPPORTED_CARD_TYPE,

	UFR_WRITE_VERIFICATION_ERROR = 0x70,
	UFR_BUFFER_SIZE_EXCEEDED = 0x71,
	UFR_VALUE_BLOCK_INVALID = 0x72,
	UFR_VALUE_BLOCK_ADDR_INVALID = 0x73,
	UFR_VALUE_BLOCK_MANIPULATION_ERROR = 0x74,
	UFR_WRONG_UI_MODE = 0x75,
	UFR_KEYS_LOCKED = 0x76,
	UFR_KEYS_UNLOCKED = 0x77,
	UFR_WRONG_PASSWORD = 0x78,
	UFR_CAN_NOT_LOCK_DEVICE = 0x79,
	UFR_CAN_NOT_UNLOCK_DEVICE = 0x7A,
	UFR_DEVICE_EEPROM_BUSY = 0x7B,
	UFR_RTC_SET_ERROR = 0x7C,
	UFR_TAG_UNKNOWN = 0x7D,

	UFR_COMMUNICATION_BREAK = 0x50,
	UFR_NO_MEMORY_ERROR = 0x51,
	UFR_CAN_NOT_OPEN_READER = 0x52,
	UFR_READER_NOT_SUPPORTED = 0x53,
	UFR_READER_OPENING_ERROR = 0x54,
	UFR_READER_PORT_NOT_OPENED = 0x55,
	UFR_CANT_CLOSE_READER_PORT = 0x56,

	UFR_TIMEOUT_ERR = 0x90,

	UFR_FT_STATUS_ERROR_1 = 0xA0,
	UFR_FT_STATUS_ERROR_2 = 0xA1,
	UFR_FT_STATUS_ERROR_3 = 0xA2,
	UFR_FT_STATUS_ERROR_4 = 0xA3,
	UFR_FT_STATUS_ERROR_5 = 0xA4,
	UFR_FT_STATUS_ERROR_6 = 0xA5,
	UFR_FT_STATUS_ERROR_7 = 0xA6,
	UFR_FT_STATUS_ERROR_8 = 0xA7,
	UFR_FT_STATUS_ERROR_9 = 0xA8,

	//NDEF error codes
	UFR_WRONG_NDEF_CARD_FORMAT = 0x80,
	UFR_NDEF_MESSAGE_NOT_FOUND = 0x81,
	UFR_NDEF_UNSUPPORTED_CARD_TYPE = 0x82,
	UFR_NDEF_CARD_FORMAT_ERROR = 0x83,
	UFR_MAD_NOT_ENABLED = 0x84,
	UFR_MAD_VERSION_NOT_SUPPORTED = 0x85,

	// multiple units - return from the functions with ReaderList_ prefix in name
	UFR_DEVICE_WRONG_HANDLE = 0x100,
	UFR_DEVICE_INDEX_OUT_OF_BOUND,
	UFR_DEVICE_ALREADY_OPENED,
	UFR_DEVICE_ALREADY_CLOSED,
	UFR_DEVICE_IS_NOT_CONNECTED,

	// Originality Check Error Codes:
	UFR_NOT_NXP_GENUINE = 0x200,
	UFR_OPEN_SSL_DYNAMIC_LIB_FAILED,
	UFR_OPEN_SSL_DYNAMIC_LIB_NOT_FOUND,

	UFR_NOT_IMPLEMENTED = 0x1000,
	UFR_COMMAND_FAILED,

	MAX_UFR_STATUS = 0x7FFFFFFF
} UFR_STATUS;

//DESFIRE Card Status Error Codes
#define READER_ERROR			2999
#define NO_CARD_DETECTED		3000
#define CARD_OPERATION_OK		3001
#define WRONG_KEY_TYPE			3002
#define KEY_AUTH_ERROR			3003
#define CARD_CRYPTO_ERROR		3004
#define READER_CARD_COMM_ERROR	3005
#define PC_READER_COMM_ERROR	3006
#define COMMIT_TRANSACTION_NO_REPLY	3007
#define COMMIT_TRANSACTION_ERROR		3008

#define	DESFIRE_CARD_NO_CHANGES					0x0C0C
#define	DESFIRE_CARD_OUT_OF_EEPROM_ERROR		0x0C0E
#define	DESFIRE_CARD_ILLEGAL_COMMAND_CODE		0x0C1C
#define	DESFIRE_CARD_INTEGRITY_ERROR			0x0C1E
#define	DESFIRE_CARD_NO_SUCH_KEY				0x0C40
#define	DESFIRE_CARD_LENGTH_ERROR				0x0C7E
#define	DESFIRE_CARD_PERMISSION_DENIED			0x0C9D
#define	DESFIRE_CARD_PARAMETER_ERROR			0x0C9E
#define	DESFIRE_CARD_APPLICATION_NOT_FOUND		0x0CA0
#define	DESFIRE_CARD_APPL_INTEGRITY_ERROR		0x0CA1
#define	DESFIRE_CARD_AUTHENTICATION_ERROR		0x0CAE
#define	DESFIRE_CARD_ADDITIONAL_FRAME			0x0CAF
#define	DESFIRE_CARD_BOUNDARY_ERROR				0x0CBE
#define	DESFIRE_CARD_PICC_INTEGRITY_ERROR		0x0CC1
#define	DESFIRE_CARD_COMMAND_ABORTED			0x0CCA
#define	DESFIRE_CARD_PICC_DISABLED_ERROR		0x0CCD
#define	DESFIRE_CARD_COUNT_ERROR				0x0CCE
#define	DESFIRE_CARD_DUPLICATE_ERROR			0x0CDE
#define	DESFIRE_CARD_EEPROM_ERROR_DES			0x0CEE
#define	DESFIRE_CARD_FILE_NOT_FOUND				0x0CF0
#define	DESFIRE_CARD_FILE_INTEGRITY_ERROR		0x0CF1

//DESFIRE key settings values
#define DESFIRE_KEY_SET_CREATE_WITH_AUTH_SET_CHANGE_KEY_CHANGE				0x09
#define DESFIRE_KEY_SET_CREATE_WITHOUT_AUTH_SET_CHANGE_KEY_CHANGE			0x0F
#define DESFIRE_KEY_SET_CREATE_WITH_AUTH_SET_NOT_CHANGE_KEY_CHANGE			0x01
#define DESFIRE_KEY_SET_CREATE_WITHOUT_AUTH_SET_NOT_CHANGE_KEY_CHANGE		0x07
#define DESFIRE_KEY_SET_CREATE_WITH_AUTH_SET_CHANGE_KEY_NOT_CHANGE			0x08
#define DESFIRE_KEY_SET_CREATE_WITHOUT_AUTH_SET_CHANGE_KEY_NOT_CHANGE		0x0E
#define DESFIRE_KEY_SET_CREATE_WITH_AUTH_SET_NOT_CHANGE_KEY_NOT_CHANGE		0x00
#define DESFIRE_KEY_SET_CREATE_WITHOUT_AUTH_SET_NOT_CHANGE_KEY_NOT_CHANGE	0x06


#ifdef __cplusplus
extern "C"
{
#endif

//--------------------------------------------------------------------------------------------------

UFR_STATUS DL_API ReaderOpen(void);

/**
 * Useful for speed up opening for non uFR basic reader type
 *
 * @param reader_type : 0 : auto > same as call ReaderOpen()
 *                      1 : uFR type (1 Mbps)
 *                      2 : uFR RS232 type (115200 bps)
 *                      3 : XRC type (250 Kbps)
 * @return
 */
UFR_STATUS DL_API ReaderOpenByType(uint32_t reader_type);

/**
 * ReaderOpenEx() is a function for opening port with
 *
 * @param reader_type : 0 : auto > same as call ReaderOpen()
 *                      1 : uFR type (1 Mbps)
 *                      2 : uFR RS232 type (115200 bps)
 *                      3 : XRC type (250 Kbps)
 * @param port_name : serial port name, identifier, like
 *                      "COM3" on Window or
 *                      "/dev/ttyS0" on Linux or
 *                      "/dev/tty.serial1" on OS X
 *                      or if you select FTDI
 *                      "UN123456" if Reader have integrated FTDI interface
 * @param port_interface : type of communication interfaces
 *                      0 : auto - first try FTDI than serial if no port_name defined
 *                      1 : try serial / virtual COM port / interfaces
 *                      2 : try only FTDI communication interfaces
 * @param arg : for future purpose
 * @return
 */
UFR_STATUS DL_API ReaderOpenEx(uint32_t reader_type,
							   c_string port_name,
		                       uint32_t port_interface,
							   void *arg);

UFR_STATUS DL_API ReaderReset(void);
UFR_STATUS DL_API ReaderClose(void);
UFR_STATUS DL_API ReaderSoftRestart(void);
UFR_STATUS DL_API GetReaderType(uint32_t *lpulReaderType);
UFR_STATUS DL_API GetReaderSerialNumber(uint32_t *lpulSerialNumber);

UFR_STATUS DL_API ReaderStillConnected(uint32_t *connected);

UFR_STATUS DL_API ReaderKeyWrite(const uint8_t *aucKey, uint8_t ucKeyIndex);
UFR_STATUS DL_API ReaderKeysLock(const uint8_t *password);
UFR_STATUS DL_API ReaderKeysUnlock(const uint8_t *password);
UFR_STATUS DL_API ReaderUISignal(uint8_t light_signal_mode,
                                     uint8_t beep_signal_mode);
UFR_STATUS DL_API ReadUserData(uint8_t *aucData);
UFR_STATUS DL_API WriteUserData(const uint8_t *aucData);
UFR_STATUS DL_API GetCardId(uint8_t *lpucCardType, uint32_t *lpulCardSerial);
UFR_STATUS DL_API BlockRead(uint8_t *data,
                                uint8_t block_address,
                                uint8_t auth_mode,
                                uint8_t key_index);
UFR_STATUS DL_API BlockWrite(const uint8_t *data,
                                 uint8_t block_address,
                                 uint8_t auth_mode,
                                 uint8_t key_index);
UFR_STATUS DL_API BlockInSectorRead(uint8_t *data,
                                        uint8_t sector_address,
                                        uint8_t block_in_sector_address,
                                        uint8_t auth_mode,
                                        uint8_t key_index);
UFR_STATUS DL_API BlockInSectorWrite(const uint8_t *data,
                                         uint8_t sector_address,
                                         uint8_t block_in_sector_address,
                                         uint8_t auth_mode,
                                         uint8_t key_index);
UFR_STATUS DL_API LinearRead(uint8_t *aucData,
                                 uint16_t usLinearAddress,
                                 uint16_t usDataLength,
                                 uint16_t *lpusBytesReturned,
                                 uint8_t ucKeyMode,
                                 uint8_t ucReaderKeyIndex);
UFR_STATUS DL_API LinRowRead(uint8_t *aucData,
                                 uint16_t usLinearAddress,
                                 uint16_t usDataLength,
                                 uint16_t *lpusBytesReturned,
                                 uint8_t ucKeyMode,
                                 uint8_t ucReaderKeyIndex);
UFR_STATUS DL_API LinearWrite(const uint8_t *aucData,
                                  uint16_t usLinearAddress,
                                  uint16_t usDataLength,
                                  uint16_t *lpusBytesWritten,
                                  uint8_t ucKeyMode,
                                  uint8_t ucReaderKeyIndex);
UFR_STATUS DL_API LinearFormatCard(const uint8_t *new_key_A,
                                       uint8_t blocks_access_bits,
                                       uint8_t sector_trailers_access_bits,
                                       uint8_t sector_trailers_byte9,
                                       const uint8_t *new_key_B,
                                       uint8_t *lpucSectorsFormatted,
                                       uint8_t auth_mode,
                                       uint8_t key_index);
UFR_STATUS DL_API SectorTrailerWrite(uint8_t addressing_mode,
                                         uint8_t address,
                                         const uint8_t *new_key_A,
                                         uint8_t block0_access_bits,
                                         uint8_t block1_access_bits,
                                         uint8_t block2_access_bits,
                                         uint8_t sector_trailer_access_bits,
                                         uint8_t sector_trailer_byte9,
                                         const uint8_t *new_key_B,
                                         uint8_t auth_mode,
                                         uint8_t key_index);
UFR_STATUS DL_API SectorTrailerWriteUnsafe(uint8_t addressing_mode,
                                               uint8_t address,
                                               const uint8_t *sector_trailer,
                                               uint8_t auth_mode,
                                               uint8_t key_index);
UFR_STATUS DL_API ValueBlockRead(int32_t *value,
                                     uint8_t *value_addr,
                                     uint8_t block_address,
                                     uint8_t auth_mode,
                                     uint8_t key_index);
UFR_STATUS DL_API ValueBlockInSectorRead(int32_t *value,
                                             uint8_t *value_addr,
                                             uint8_t sector_address,
                                             uint8_t block_in_sector_address,
                                             uint8_t auth_mode,
                                             uint8_t key_index);
UFR_STATUS DL_API ValueBlockWrite(int32_t value,
                                      uint8_t value_addr,
                                      uint8_t block_address,
                                      uint8_t auth_mode,
                                      uint8_t key_index);
UFR_STATUS DL_API ValueBlockInSectorWrite(int32_t value,
                                              uint8_t value_addr,
                                              uint8_t sector_address,
                                              uint8_t block_in_sector_address,
                                              uint8_t auth_mode,
                                              uint8_t key_index);
UFR_STATUS DL_API ValueBlockIncrement(int32_t increment_value,
                                          uint8_t block_address,
                                          uint8_t auth_mode,
                                          uint8_t key_index);
UFR_STATUS DL_API ValueBlockInSectorIncrement(int32_t increment_value,
                                                  uint8_t sector_address,
                                                  uint8_t block_in_sector_address,
                                                  uint8_t auth_mode,
                                                  uint8_t key_index);
UFR_STATUS DL_API ValueBlockDecrement(int32_t decrement_value,
                                          uint8_t block_address,
                                          uint8_t auth_mode,
                                          uint8_t key_index);
UFR_STATUS DL_API ValueBlockInSectorDecrement(int32_t decrement_value,
                                                  uint8_t sector_address,
                                                  uint8_t block_in_sector_address,
                                                  uint8_t auth_mode,
                                                  uint8_t key_index);

UFR_STATUS DL_API BlockRead_AKM1(uint8_t *data,
                                    uint8_t block_address,
                                    uint8_t auth_mode);
UFR_STATUS DL_API BlockWrite_AKM1(const uint8_t *data,
                                 uint8_t block_address,
                                 uint8_t auth_mode);
UFR_STATUS DL_API BlockInSectorRead_AKM1(uint8_t *data,
                                        uint8_t sector_address,
                                        uint8_t block_in_sector_address,
                                        uint8_t auth_mode);
UFR_STATUS DL_API BlockInSectorWrite_AKM1(const uint8_t *data,
                                         uint8_t sector_address,
                                         uint8_t block_in_sector_address,
                                         uint8_t auth_mode);
UFR_STATUS DL_API LinearRead_AKM1(uint8_t *data,
                                 uint16_t linear_address,
                                 uint16_t length,
                                 uint16_t *bytes_returned,
                                 uint8_t auth_mode);
UFR_STATUS DL_API LinRowRead_AKM1(uint8_t *data,
                                 uint16_t linear_address,
                                 uint16_t length,
                                 uint16_t *bytes_returned,
                                 uint8_t auth_mode);
UFR_STATUS DL_API LinearWrite_AKM1(const uint8_t *data,
                                  uint16_t linear_address,
                                  uint16_t length,
                                  uint16_t *bytes_written,
                                  uint8_t auth_mode);
UFR_STATUS DL_API LinearFormatCard_AKM1(const uint8_t *new_key_A,
                                       uint8_t blocks_access_bits,
                                       uint8_t sector_trailers_access_bits,
                                       uint8_t sector_trailers_byte9,
                                       const uint8_t *new_key_B,
                                       uint8_t *lpucSectorsFormatted,
                                       uint8_t auth_mode);
UFR_STATUS DL_API SectorTrailerWrite_AKM1(uint8_t addressing_mode,
                                         uint8_t address,
                                         const uint8_t *new_key_A,
                                         uint8_t block0_access_bits,
                                         uint8_t block1_access_bits,
                                         uint8_t block2_access_bits,
                                         uint8_t sector_trailer_access_bits,
                                         uint8_t sector_trailer_byte9,
                                         const uint8_t *new_key_B,
                                         uint8_t auth_mode);
UFR_STATUS DL_API SectorTrailerWriteUnsafe_AKM1(uint8_t addressing_mode,
                                               uint8_t address,
                                               const uint8_t *sector_trailer,
                                               uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockRead_AKM1(int32_t *value,
                                     uint8_t *value_addr,
                                     uint8_t block_address,
                                     uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockInSectorRead_AKM1(int32_t *value,
                                             uint8_t *value_addr,
                                             uint8_t sector_address,
                                             uint8_t block_in_sector_address,
                                             uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockWrite_AKM1(int32_t value,
                                      uint8_t value_addr,
                                      uint8_t block_address,
                                      uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockInSectorWrite_AKM1(int32_t value,
                                              uint8_t value_addr,
                                              uint8_t sector_address,
                                              uint8_t block_in_sector_address,
                                              uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockIncrement_AKM1(int32_t increment_value,
                                          uint8_t block_address,
                                          uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockInSectorIncrement_AKM1(int32_t increment_value,
                                                  uint8_t sector_address,
                                                  uint8_t block_in_sector_address,
                                                  uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockDecrement_AKM1(int32_t decrement_value,
                                          uint8_t block_address,
                                          uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockInSectorDecrement_AKM1(int32_t decrement_value,
                                                  uint8_t sector_address,
                                                  uint8_t block_in_sector_address,
                                                  uint8_t auth_mode);

UFR_STATUS DL_API BlockRead_AKM2(uint8_t *data,
                                    uint8_t block_address,
                                    uint8_t auth_mode);
UFR_STATUS DL_API BlockWrite_AKM2(const uint8_t *data,
                                 uint8_t block_address,
                                 uint8_t auth_mode);
UFR_STATUS DL_API BlockInSectorRead_AKM2(uint8_t *data,
                                        uint8_t sector_address,
                                        uint8_t block_in_sector_address,
                                        uint8_t auth_mode);
UFR_STATUS DL_API BlockInSectorWrite_AKM2(const uint8_t *data,
                                         uint8_t sector_address,
                                         uint8_t block_in_sector_address,
                                         uint8_t auth_mode);
UFR_STATUS DL_API LinearRead_AKM2(uint8_t *data,
                                 uint16_t linear_address,
                                 uint16_t length,
                                 uint16_t *bytes_returned,
                                 uint8_t auth_mode);
UFR_STATUS DL_API LinRowRead_AKM2(uint8_t *data,
                                 uint16_t linear_address,
                                 uint16_t length,
                                 uint16_t *bytes_returned,
                                 uint8_t auth_mode);
UFR_STATUS DL_API LinearWrite_AKM2(const uint8_t *data,
                                  uint16_t linear_address,
                                  uint16_t length,
                                  uint16_t *bytes_written,
                                  uint8_t auth_mode);
UFR_STATUS DL_API LinearFormatCard_AKM2(const uint8_t *new_key_A,
                                       uint8_t blocks_access_bits,
                                       uint8_t sector_trailers_access_bits,
                                       uint8_t sector_trailers_byte9,
                                       const uint8_t *new_key_B,
                                       uint8_t *lpucSectorsFormatted,
                                       uint8_t auth_mode);
UFR_STATUS DL_API SectorTrailerWrite_AKM2(uint8_t addressing_mode,
                                         uint8_t address,
                                         const uint8_t *new_key_A,
                                         uint8_t block0_access_bits,
                                         uint8_t block1_access_bits,
                                         uint8_t block2_access_bits,
                                         uint8_t sector_trailer_access_bits,
                                         uint8_t sector_trailer_byte9,
                                         const uint8_t *new_key_B,
                                         uint8_t auth_mode);
UFR_STATUS DL_API SectorTrailerWriteUnsafe_AKM2(uint8_t addressing_mode,
                                               uint8_t address,
                                               const uint8_t *sector_trailer,
                                               uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockRead_AKM2(int32_t *value,
                                     uint8_t *value_addr,
                                     uint8_t block_address,
                                     uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockInSectorRead_AKM2(int32_t *value,
                                             uint8_t *value_addr,
                                             uint8_t sector_address,
                                             uint8_t block_in_sector_address,
                                             uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockWrite_AKM2(int32_t value,
                                      uint8_t value_addr,
                                      uint8_t block_address,
                                      uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockInSectorWrite_AKM2(int32_t value,
                                              uint8_t value_addr,
                                              uint8_t sector_address,
                                              uint8_t block_in_sector_address,
                                              uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockIncrement_AKM2(int32_t increment_value,
                                          uint8_t block_address,
                                          uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockInSectorIncrement_AKM2(int32_t increment_value,
                                                  uint8_t sector_address,
                                                  uint8_t block_in_sector_address,
                                                  uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockDecrement_AKM2(int32_t decrement_value,
                                          uint8_t block_address,
                                          uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockInSectorDecrement_AKM2(int32_t decrement_value,
                                                  uint8_t sector_address,
                                                  uint8_t block_in_sector_address,
                                                  uint8_t auth_mode);

UFR_STATUS DL_API BlockRead_PK(uint8_t *data,
                                   uint8_t block_address,
                                   uint8_t auth_mode,
                                   const uint8_t *key);
UFR_STATUS DL_API BlockWrite_PK(const uint8_t *data,
                                    uint8_t block_address,
                                    uint8_t auth_mode,
                                    const uint8_t *key);
UFR_STATUS DL_API BlockInSectorRead_PK(uint8_t *data,
                                           uint8_t sector_address,
                                           uint8_t block_in_sector_address,
                                           uint8_t auth_mode,
                                           const uint8_t *key);
UFR_STATUS DL_API BlockInSectorWrite_PK(const uint8_t *data,
                                            uint8_t sector_address,
                                            uint8_t block_in_sector_address,
                                            uint8_t auth_mode,
                                            const uint8_t *key);
UFR_STATUS DL_API LinearRead_PK(uint8_t *data,
                                    uint16_t linear_address,
                                    uint16_t length,
                                    uint16_t *bytes_returned,
                                    uint8_t auth_mode,
                                    const uint8_t *key);
UFR_STATUS DL_API LinRowRead_PK(uint8_t *data,
                                    uint16_t linear_address,
                                    uint16_t length,
                                    uint16_t *bytes_returned,
                                    uint8_t auth_mode,
                                    const uint8_t *key);
UFR_STATUS DL_API LinearWrite_PK(const uint8_t *data,
                                     uint16_t linear_address,
                                     uint16_t length,
                                     uint16_t *bytes_written,
                                     uint8_t auth_mode,
                                     const uint8_t *key);
UFR_STATUS DL_API LinearFormatCard_PK(const uint8_t *new_key_A,
                                          uint8_t blocks_access_bits,
                                          uint8_t sector_trailers_access_bits,
                                          uint8_t sector_trailers_byte9,
                                          const uint8_t *new_key_B,
                                          uint8_t *lpucSectorsFormatted,
                                          uint8_t auth_mode,
                                          const uint8_t *key);
UFR_STATUS DL_API SectorTrailerWrite_PK(uint8_t addressing_mode,
                                            uint8_t address,
                                            const uint8_t *new_key_A,
                                            uint8_t block0_access_bits,
                                            uint8_t block1_access_bits,
                                            uint8_t block2_access_bits,
                                            uint8_t sector_trailer_access_bits,
                                            uint8_t sector_trailer_byte9,
                                            const uint8_t *new_key_B,
                                            uint8_t auth_mode,
                                            const uint8_t *key);
UFR_STATUS DL_API SectorTrailerWriteUnsafe_PK(uint8_t addressing_mode,
                                                  uint8_t address,
                                                  const uint8_t *sector_trailer,
                                                  uint8_t auth_mode,
                                                  const uint8_t *key);
UFR_STATUS DL_API ValueBlockRead_PK(int32_t *value,
                                        uint8_t *value_addr,
                                        uint8_t block_address,
                                        uint8_t auth_mode,
                                        const uint8_t *key);
UFR_STATUS DL_API ValueBlockInSectorRead_PK(int32_t *value,
                                                uint8_t *value_addr,
                                                uint8_t sector_address,
                                                uint8_t block_in_sector_address,
                                                uint8_t auth_mode,
                                                const uint8_t *key);
UFR_STATUS DL_API ValueBlockWrite_PK(int32_t value,
                                         uint8_t value_addr,
                                         uint8_t block_address,
                                         uint8_t auth_mode,
                                         const uint8_t *key);
UFR_STATUS DL_API ValueBlockInSectorWrite_PK(int32_t value,
                                                 uint8_t value_addr,
                                                 uint8_t sector_address,
                                                 uint8_t block_in_sector_address,
                                                 uint8_t auth_mode,
                                                 const uint8_t *key);
UFR_STATUS DL_API ValueBlockIncrement_PK(int32_t increment_value,
                                             uint8_t block_address,
                                             uint8_t auth_mode,
                                             const uint8_t *key);
UFR_STATUS DL_API ValueBlockInSectorIncrement_PK(int32_t increment_value,
                                                     uint8_t sector_address,
                                                     uint8_t block_in_sector_address,
                                                     uint8_t auth_mode,
                                                     const uint8_t *key);
UFR_STATUS DL_API ValueBlockDecrement_PK(int32_t decrement_value,
                                             uint8_t block_address,
                                             uint8_t auth_mode,
                                             const uint8_t *key);
UFR_STATUS DL_API ValueBlockInSectorDecrement_PK(int32_t decrement_value,
                                                     uint8_t sector_address,
                                                     uint8_t block_in_sector_address,
                                                     uint8_t auth_mode,
                                                     const uint8_t *key);
UFR_STATUS DL_API GetReaderHardwareVersion(uint8_t *version_major,
                                               uint8_t *version_minor);
UFR_STATUS DL_API GetReaderFirmwareVersion(uint8_t *version_major,
                                               uint8_t *version_minor);

// New commands (for RTC & I2C EEPROM):
UFR_STATUS DL_API GetReaderTime(uint8_t *time);
UFR_STATUS DL_API SetReaderTime(uint8_t *password, uint8_t *time);
UFR_STATUS DL_API ChangeReaderPassword(uint8_t *old_password, uint8_t *new_password);
UFR_STATUS DL_API ReaderEepromWrite(uint8_t *data, uint32_t address, uint32_t size, uint8_t *password);
UFR_STATUS DL_API ReaderEepromRead(uint8_t *data, uint32_t address, uint32_t size);
UFR_STATUS DL_API ChangeReaderJobId(uint8_t *job_id, uint8_t *new_password);

UFR_STATUS DL_API GetReaderSerialDescription(uint8_t pSerialDescription[8]);
UFR_STATUS DL_API SetReaderSerialDescription(const uint8_t pSerialDescription[8]);

// New since version 2.0:
UFR_STATUS DL_API GetBuildNumber(uint8_t *build);
UFR_STATUS DL_API GetCardIdEx(uint8_t *lpucSak, uint8_t *aucUid,
		uint8_t *lpucUidSize);
UFR_STATUS DL_API GetLastCardIdEx(uint8_t *lpucSak, uint8_t *aucUid,
		uint8_t *lpucUidSize);
//------------------------------------------------------------------------------
//Multi-card (anti collision) mode:
//------------------------------------------------------------------------------
UFR_STATUS DL_API EnableAntiCollision(void);
UFR_STATUS DL_API DisableAntiCollision(void);
UFR_STATUS DL_API EnumCards(uint8_t *lpucCardsNumber, uint8_t *lpucUidListSize); // Card pointer is on the first card in list
UFR_STATUS DL_API ListCards(uint8_t *aucUidList, uint8_t ucUidListSize); // Before calling this function you must call EnumCards() first.
UFR_STATUS DL_API SelectCard(const uint8_t *aucUid, uint8_t ucUidSize, uint8_t *lpucSelctedCardType);
UFR_STATUS DL_API DeslectCard(void);
UFR_STATUS DL_API GetAntiCollisionStatus(int8_t *lpcIsAntiCollEnabled, int8_t *lpcIsAnyCardSelected);
//------------------------------------------------------------------------------
UFR_STATUS DL_API GetDlogicCardType(uint8_t *lpucCardType);
UFR_STATUS DL_API GetNfcT2TVersion(uint8_t lpucVersionResponse[8]);
UFR_STATUS DL_API GetCardSize(uint32_t *lpulLinearSize, uint32_t *lpulRawSize);

// uFCoder PRO MODE
UFR_STATUS DL_API GetReaderProMode(uint32_t *pReaderProMode, uint32_t *pReaderProConfig);
UFR_STATUS DL_API SetReaderProMode(const uint32_t ReaderProMode);

// QR barcode crypt algorithm
// initialization. with TB serial like 'TB123456'
UFR_STATUS DL_API CardEncryption_Initialize(const uint8_t *TBSerialString, uint16_t job_number);

// You must define 25 bytes array in memory for CardSerialString[]
UFR_STATUS DL_API CardEncryption_GetNextEncryptedCard(const uint32_t from_timestamp, const uint32_t to_timestamp,
		uint8_t CardSerialString[25]);

UFR_STATUS DL_API CardEncryption_GetActualCardSN(uint32_t *ActualCard_SN, uint32_t *ActualCard_SN_LOG);
UFR_STATUS DL_API CardEncryption_GetJobSN(uint32_t *JobSN);
UFR_STATUS DL_API CardEncryption_GetSalterSN(uint8_t SalterSN[8], uint8_t * magicByte);

UFR_STATUS DL_API read_ndef_record(uint8_t message_nr, uint8_t record_nr, uint8_t *tnf, uint8_t *type_record, uint8_t *type_length, uint8_t *id, uint8_t *id_length,
									   uint8_t *payload, uint32_t *payload_length);
UFR_STATUS DL_API write_ndef_record(uint8_t message_nr, uint8_t *tnf, uint8_t *type_record, uint8_t *type_length, uint8_t *id, uint8_t *id_length,
		   uint8_t *payload, uint32_t *payload_length, uint8_t *card_formated);
UFR_STATUS DL_API write_ndef_record_mirroring(uint8_t message_nr,
		          uint8_t *tnf, uint8_t *type_record, uint8_t *type_length, uint8_t *id,
		          uint8_t *id_length, uint8_t *payload, uint32_t *payload_length,
		          uint8_t *card_formated,
		          int use_uid_ascii_mirror, int use_counter_ascii_mirror, uint32_t payload_mirroring_pos);
UFR_STATUS DL_API get_ndef_record_count(uint8_t *ndef_message_cnt, uint8_t *ndef_record_cnt, uint8_t *ndef_record_array, uint8_t *empty_ndef_message_cnt);
UFR_STATUS DL_API erase_last_ndef_record(uint8_t message_nr);
UFR_STATUS DL_API erase_all_ndef_records(uint8_t message_nr);
UFR_STATUS DL_API ndef_card_initialization(void);
//---------------------------------------------------------------------
// Card emulation:
//---------------------------------------------------------------------
UFR_STATUS DL_API WriteEmulationNdef(uint8_t tnf,
		uint8_t* type_record,
		uint8_t type_length,
		uint8_t* id,
		uint8_t id_length,
		uint8_t* payload,
		uint8_t payload_length);
UFR_STATUS DL_API WriteEmulationNdefWithAAR(
		uint8_t tnf,
		uint8_t *type_record,
		uint8_t type_length,
		uint8_t *id,
		uint8_t id_length,
		uint8_t *payload,
		uint8_t payload_length,
		uint8_t *aar,
		uint8_t aar_length);
UFR_STATUS DL_API TagEmulationStart(void);
UFR_STATUS DL_API TagEmulationStop(void);
UFR_STATUS DL_API CombinedModeEmulationStart(void);
UFR_STATUS DL_API AdHocEmulationStart(void);
UFR_STATUS DL_API AdHocEmulationStop(void);
UFR_STATUS DL_API GetAdHocEmulationParams(uint8_t *ThresholdMinLevel, uint8_t *ThresholdCollLevel,
											uint8_t *RFLevelAmp, uint8_t *RxGain, uint8_t *RFLevel);
UFR_STATUS DL_API SetAdHocEmulationParams(uint8_t ThresholdMinLevel, uint8_t ThresholdCollLevel,
											uint8_t RFLevelAmp, uint8_t RxGain, uint8_t RFLevel);
UFR_STATUS DL_API GetExternalFieldState(uint8_t *is_field_present);
UFR_STATUS DL_API EnterShareRamCommMode(void);
UFR_STATUS DL_API ExitShareRamCommMode(void);
UFR_STATUS DL_API WriteShareRam(uint8_t *ram_data, uint8_t addr, uint8_t data_len);
UFR_STATUS DL_API ReadShareRam(uint8_t *ram_data, uint8_t addr, uint8_t data_len);

//------------------------------------------------------------------------------

// GetNfcT2TVersion() returns 8 bytes (see T2T documentation):
typedef struct t2t_version_struct {
	uint8_t header;
	uint8_t vendor_id;
	uint8_t product_type;
	uint8_t product_subtype;
	uint8_t major_product_version;
	uint8_t minor_product_version;
	uint8_t storage_size;
	uint8_t protocol_type;
} t2t_version_t;

// NfcT2TSafeConvertVersion() returns converts version_record that returned from GetNfcT2TVersion()
// or GetNfcT2TVersionM(). Conversion is "alignment safe"
// (you don't need to pay attention on structure byte alignment):
void DL_API NfcT2TSafeConvertVersion(t2t_version_t *version, const uint8_t *version_record);
UFR_STATUS DL_API ReadECCSignature(uint8_t lpucECCSignature[ECC_SIG_LEN],
		uint8_t lpucUid[MAX_UID_LEN], uint8_t *lpucUidLen, uint8_t *lpucDlogicCardType);

//------------------------------------------------------------------------------
UFR_STATUS DL_API ReadCounter(uint8_t counter_address, uint32_t *value);
UFR_STATUS DL_API IncrementCounter(uint8_t counter_address, uint32_t inc_value);
UFR_STATUS DL_API ReadNFCCounter(uint32_t *value); // Same as ReadCounter(2, &value);
UFR_STATUS DL_API ReadNFCCounterPwdAuth_RK(uint32_t *value, uint8_t reader_key_index);
UFR_STATUS DL_API ReadNFCCounterPwdAuth_PK(uint32_t *value, const uint8_t *key);

//------------------------------------------------------------------------------

UFR_STATUS DL_API SetAsyncCardIdSendConfig(uint8_t send_enable,
		uint8_t prefix_enable, uint8_t prefix, uint8_t suffix,
		uint8_t send_removed_enable, uint32_t async_baud_rate);

UFR_STATUS DL_API SetAsyncCardIdSendConfigEx(uint8_t send_enable,
		uint8_t prefix_enable, uint8_t prefix, uint8_t suffix,
		uint8_t send_removed_enable, uint8_t reverse_byte_order,
		uint8_t decimal_representation, uint32_t async_baud_rate);

UFR_STATUS DL_API GetAsyncCardIdSendConfig(uint8_t *send_enable,
		uint8_t *prefix_enable, uint8_t *prefix, uint8_t *suffix,
		uint8_t *send_removed_enable, uint32_t *async_baud_rate);

UFR_STATUS DL_API GetAsyncCardIdSendConfigEx(uint8_t *send_enable,
		uint8_t *prefix_enable, uint8_t *prefix, uint8_t *suffix,
		uint8_t *send_removed_enable, uint8_t *reverse_byte_order,
		uint8_t *decimal_representation, uint32_t *async_baud_rate);

/*****************************************************************************
 * AIS FUNCTIONS
 *****************************************************************************
 */

/**
 * ais_get_card_number() read assigned serial number from AIS cards
 *
 * @param card_number ( pointer to reserved 32 bit integer )  AIS card serial number
 * @return UFR_STATUS execution status
 */
UFR_STATUS DL_API ais_get_card_number(uint32_t *card_number);

UFR_STATUS DL_API ais_set_right_record(uint8_t record_number, uint16_t first_reader_nr, uint16_t last_reader_nr,
		uint8_t start_hour, uint8_t start_minute, uint8_t end_hour, uint8_t end_minute, uint8_t *days);
UFR_STATUS DL_API ais_get_right_record(uint8_t record_number, uint16_t *first_reader_nr, uint16_t *last_reader_nr,
		uint8_t *start_hour, uint8_t *start_minute, uint8_t *end_hour, uint8_t *end_minute, uint8_t *days);
UFR_STATUS DL_API ais_erase_right_record(uint8_t record_number);
UFR_STATUS DL_API ais_set_validate_record(uint8_t begin_year, uint8_t begin_month, uint8_t begin_day, uint8_t begin_hour, uint8_t begin_minute,
		uint8_t end_year, uint8_t end_month, uint8_t end_day, uint8_t end_hour, uint8_t end_minute);
UFR_STATUS DL_API ais_get_validate_record(uint8_t *begin_year, uint8_t *begin_month, uint8_t *begin_day, uint8_t *begin_hour, uint8_t *begin_minute,
		uint8_t *end_year, uint8_t *end_month, uint8_t *end_day, uint8_t *end_hour, uint8_t *end_minute);
UFR_STATUS DL_API ais_set_card_type(uint8_t card_type);
UFR_STATUS DL_API ais_get_card_type(uint8_t *card_type);
UFR_STATUS DL_API ais_set_card_daily_duration(uint16_t duration);
UFR_STATUS DL_API ais_get_card_daily_duration(uint16_t *duration);
UFR_STATUS DL_API ais_set_card_total_duration(uint32_t duration);
UFR_STATUS DL_API ais_get_card_total_duration(uint32_t *duration);

// swimming pool **************************************************************
/**
 * ais_get_credit_and_period_validity() > get information about credit and period of validity
 *
 * @param credit ( pointer to integer ) :
 * @param begin_year ( pointer to integer ) :
 * @param begin_month ( pointer to integer ) :
 * @param begin_day ( pointer to integer ) :
 * @param begin_hour ( pointer to integer ) :
 * @param begin_minute ( pointer to integer ) :
 * @param end_year ( pointer to integer ) :
 * @param end_month ( pointer to integer ) :
 * @param end_day ( pointer to integer ) :
 * @param end_hour ( pointer to integer ) :
 * @param end_minute ( pointer to integer ) :
 * @return
 */
UFR_STATUS DL_API ais_get_credit_and_period_validity(int32_t *credit,
		uint32_t *begin_year, uint32_t *begin_month, uint32_t *begin_day,
		uint32_t *begin_hour,
		uint32_t *begin_minute, //
		uint32_t *end_year, uint32_t *end_month, uint32_t *end_day,
		uint32_t *end_hour, uint32_t *end_minute);

/**
 * ais_set_credit_and_period_validity() > set credit and period of validity
 *
 * @param credit ( integer ) :
 * @param begin_year ( integer ) :
 * @param begin_month ( integer ) :
 * @param begin_day ( integer ) :
 * @param begin_hour ( integer ) :
 * @param begin_minute ( integer ) :
 * @param end_year ( integer ) :
 * @param end_month ( integer ) :
 * @param end_day ( integer ) :
 * @param end_hour ( integer ) :
 * @param end_minute ( integer ) :
 * @return
 */
UFR_STATUS DL_API ais_set_credit_and_period_validity(int32_t credit,
		uint32_t begin_year, uint32_t begin_month, uint32_t begin_day,
		uint32_t begin_hour,
		uint32_t begin_minute, //
		uint32_t end_year, uint32_t end_month, uint32_t end_day,
		uint32_t end_hour, uint32_t end_minute);

UFR_STATUS DL_API ais_set_right_type_record(uint8_t record_number,
		uint8_t right_record_type, uint8_t *right_data);

UFR_STATUS DL_API ais_get_right_type_record(uint8_t record_number,
		uint8_t *right_record_type, uint8_t *right_data);

UFR_STATUS DL_API ais_set_right_record_type_max_daily_counter(uint8_t record_number,
		uint16_t first_reader_nr, uint16_t last_reader_nr, uint8_t start_hour,
		uint8_t start_minute, uint8_t end_hour, uint8_t end_minute,
		uint8_t *days, uint8_t max_daily_counter);

UFR_STATUS DL_API ais_get_right_record_type_max_daily_counter(uint8_t record_number,
		uint16_t *first_reader_nr, uint16_t *last_reader_nr,
		uint8_t *start_hour, uint8_t *start_minute, uint8_t *end_hour,
		uint8_t *end_minute, uint8_t *days, uint8_t *max_daily_counter);

//=============================================================================

UFR_STATUS DL_API UfrXrcLockOn(uint16_t pulse_duration);
UFR_STATUS DL_API UfrXrcRelayState(uint8_t state);
UFR_STATUS DL_API UfrXrcGetIoState(uint8_t *intercom, uint8_t *door, uint8_t *relay_state);
UFR_STATUS DL_API UfrRedLightControl(uint8_t light_status);

UFR_STATUS DL_API UfrSetBadSelectCardNrMax(uint8_t bad_select_nr_max);
UFR_STATUS DL_API UfrGetBadSelectCardNrMax(uint8_t *bad_select_nr_max);

UFR_STATUS DL_API UfrEnterSleepMode(void);
UFR_STATUS DL_API UfrLeaveSleepMode(void);
UFR_STATUS DL_API AutoSleepSet(uint8_t seconds_wait);
UFR_STATUS DL_API AutoSleepGet(uint8_t *seconds_wait);

UFR_STATUS DL_API SetSpeedPermanently(unsigned char tx_speed, unsigned char rx_speed);
UFR_STATUS DL_API GetSpeedParameters(unsigned char *tx_speed, unsigned char *rx_speed);
UFR_STATUS DL_API SetDisplayData(uint8_t *display_data, uint8_t data_length);
UFR_STATUS DL_API SetSpeakerFrequency(uint16_t frequency);
UFR_STATUS DL_API SetDisplayIntensity(uint8_t intensity);
UFR_STATUS DL_API GetDisplayIntensity(uint8_t *intensity);
// DESFIRE functions **************************************************************

/**
 *
 * @param chaining
 * @param timeout
 * @param block_length
 * @param snd_data_array
 * @param rcv_length
 * @param rcv_data_array
 * @param ufr_status
 * @return
 */
UFR_STATUS DL_API SetISO14443_4_Mode(void);
UFR_STATUS DL_API uFR_i_block_transceive(uint8_t chaining, uint8_t timeout,
		uint8_t block_length, uint8_t *snd_data_array, size_t *rcv_length,
		uint8_t *rcv_data_array, uint32_t *ufr_status);

UFR_STATUS DL_API uFR_APDU_Transceive(uint8_t cls, uint8_t ins, uint8_t p1, uint8_t p2,
		uint8_t *data_out, uint8_t data_out_len, uint8_t *data_in, uint32_t max_data_in_len, uint32_t *response_len,
		uint8_t send_le, uint8_t *apdu_status);

UFR_STATUS DL_API APDUHexStrTransceive(const char *c_apdu, char **r_apdu);
UFR_STATUS DL_API APDUPlainTransceive(const uint8_t *c_apdu, uint32_t c_apdu_len, uint8_t *r_apdu, uint32_t *r_apdu_len);
UFR_STATUS DL_API APDUTransceive(uint8_t cls, uint8_t ins, uint8_t p1, uint8_t p2,
		const uint8_t *data_out, uint32_t Nc, uint8_t *data_in, uint32_t *Ne,
		uint8_t send_le, uint8_t *apdu_status);
UFR_STATUS DL_API i_block_trans_rcv_chain(uint8_t chaining,	uint8_t timeout,
		uint8_t block_length, uint8_t *snd_data_array,
		uint8_t *rcv_length, uint8_t *rcv_data_array, uint8_t *rcv_chained, uint32_t *ufr_status);
UFR_STATUS DL_API r_block_transceive(uint8_t ack, uint8_t timeout,
		uint8_t *rcv_length, uint8_t *rcv_data_array, uint8_t *rcv_chained, uint32_t *ufr_status);
UFR_STATUS DL_API s_block_deselect(uint8_t timeout);

UFR_STATUS DL_API DES_to_AES_key_type(void);

UFR_STATUS DL_API AES_to_DES_key_type(void);

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

UFR_STATUS DL_API uFR_int_GetDesfireUid(uint8_t aes_key_nr, uint32_t aid,
		uint8_t aid_key_nr, uint8_t *card_uid, uint8_t *card_uid_len,
		uint16_t *card_status, uint16_t *exec_time);
UFR_STATUS DL_API uFR_int_GetDesfireUid_PK(uint8_t *aes_key_ext, uint32_t aid,
		uint8_t aid_key_nr, uint8_t *card_uid, uint8_t *card_uid_len,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireFreeMem(uint32_t *free_mem_byte, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireFormatCard(uint8_t aes_key_nr, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireFormatCard_PK(uint8_t *aes_key_ext, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireCreateStdDataFile(uint8_t aes_key_nr, uint32_t aid, uint8_t file_id, uint32_t file_size,
		uint8_t read_key_no, uint8_t write_key_no, uint8_t read_write_key_no, uint8_t change_key_no,
		uint8_t communication_settings, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireCreateStdDataFile_PK(uint8_t *aes_key_ext, uint32_t aid, uint8_t file_id, uint32_t file_size,
		uint8_t read_key_no, uint8_t write_key_no, uint8_t read_write_key_no, uint8_t change_key_no,
		uint8_t communication_settings, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireCreateStdDataFile_no_auth(uint32_t aid, uint8_t file_id, uint32_t file_size,
		uint8_t read_key_no, uint8_t write_key_no, uint8_t read_write_key_no, uint8_t change_key_no,
		uint8_t communication_settings, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireDeleteFile(uint8_t aes_key_nr, uint32_t aid, uint8_t file_id,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireDeleteFile_PK(uint8_t *aes_key_ext, uint32_t aid, uint8_t file_id,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireDeleteFile_no_auth(uint32_t aid, uint8_t file_id,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireCreateAesApplication(uint8_t aes_key_nr, uint32_t aid,
		uint8_t setting, uint8_t max_key_no, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireCreateAesApplication_PK(uint8_t *aes_key_ext, uint32_t aid,
		uint8_t setting, uint8_t max_key_no, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireCreateAesApplication_no_auth(uint32_t aid,
		uint8_t setting, uint8_t max_key_no, uint16_t *card_status,	uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireDeleteApplication(uint8_t aes_key_nr,	uint32_t aid,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireDeleteApplication_PK(uint8_t *aes_key_ext, uint32_t aid,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireSetConfiguration(uint8_t aes_key_nr, uint8_t random_uid, uint8_t format_disable,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireSetConfiguration_PK(uint8_t *aes_key_ext, uint8_t random_uid, uint8_t format_disable,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireGetKeySettings(uint8_t aes_key_nr, uint32_t aid,
		uint8_t *setting, uint8_t *max_key_no,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireGetKeySettings_PK(uint8_t *aes_key_ext, uint32_t aid,
		uint8_t *setting, uint8_t *max_key_no,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireChangeKeySettings(uint8_t aes_key_nr,	uint32_t aid,
		uint8_t setting, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireChangeKeySettings_PK(uint8_t *aes_key_ext, uint32_t aid,
		uint8_t setting, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireChangeAesKey(uint8_t aes_key_nr, uint32_t aid, uint8_t aid_key_no_auth,
		uint8_t new_aes_key[16], uint8_t aid_key_no, uint8_t old_aes_key[16],
		uint16_t *card_status, uint16_t *exec_time);

// all keys are in reader
UFR_STATUS DL_API uFR_int_DesfireChangeAesKey_A(uint8_t aes_key_nr, uint32_t aid, uint8_t aid_key_no_auth,
		uint8_t new_aes_key_nr, uint8_t aid_key_no, uint8_t old_aes_key_nr,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireChangeAesKey_PK(uint8_t *aes_key_ext, uint32_t aid, uint8_t aid_key_no_auth,
		uint8_t new_aes_key[16], uint8_t aid_key_no, uint8_t old_aes_key[16],
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_SetParam(uint8_t aes_key[16], uint8_t key_no,
		uint32_t aid_nr, uint8_t file_id);

UFR_STATUS DL_API uFR_int_DesfireWriteAesKey(uint8_t aes_key_no, uint8_t *aes_key);

//---------------------------------------------------------------------------

UFR_STATUS DL_API uFR_int_DesfireReadStdDataFile(uint8_t aes_key_nr, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint16_t offset, uint16_t data_length,
		uint8_t communication_settings, uint8_t *data,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireReadStdDataFile_PK(uint8_t *aes_key_ext, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint16_t offset, uint16_t data_length,
		uint8_t communication_settings, uint8_t *data,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireReadStdDataFile_no_auth(uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint16_t offset, uint16_t data_length,
		uint8_t communication_settings, uint8_t *data,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireWriteStdDataFile(uint8_t aes_key_nr, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint16_t offset, uint16_t data_length,
		uint8_t communication_settings, uint8_t *data,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireWriteStdDataFile_PK(uint8_t *aes_key_ext, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint16_t offset, uint16_t data_length,
		uint8_t communication_settings, uint8_t *data,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireWriteStdDataFile_no_auth(uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint16_t offset, uint16_t data_length,
		uint8_t communication_settings, uint8_t *data,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireCreateValueFile(uint8_t aes_key_nr, uint32_t aid, uint8_t file_id,
		int32_t lower_limit, int32_t upper_limit, int32_t value, uint8_t limited_credit_enabled,
		uint8_t read_key_no, uint8_t write_key_no, uint8_t read_write_key_no, uint8_t change_key_no,
		uint8_t communication_settings, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireCreateValueFile_PK(uint8_t *aes_key_ext, uint32_t aid, uint8_t file_id,
		int32_t lower_limit, int32_t upper_limit, int32_t value, uint8_t limited_credit_enabled,
		uint8_t read_key_no, uint8_t write_key_no, uint8_t read_write_key_no, uint8_t change_key_no,
		uint8_t communication_settings, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireCreateValueFile_no_auth(uint32_t aid, uint8_t file_id,
		int32_t lower_limit, int32_t upper_limit, int32_t value, uint8_t limited_credit_enabled,
		uint8_t read_key_no, uint8_t write_key_no, uint8_t read_write_key_no, uint8_t change_key_no,
		uint8_t communication_settings, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireReadValueFile(uint8_t aes_key_nr, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint8_t communication_settings, int32_t *value,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireReadValueFile_PK(uint8_t *aes_key_ext, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint8_t communication_settings, int32_t *value,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireReadValueFile_no_auth(uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint8_t communication_settings, int32_t *value,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireIncreaseValueFile(uint8_t aes_key_nr, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint8_t communication_settings, uint32_t value,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireIncreaseValueFile_PK(uint8_t *aes_key_ext, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint8_t communication_settings, uint32_t value,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireIncreaseValueFile_no_auth(uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint8_t communication_settings, uint32_t value,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireDecreaseValueFile(uint8_t aes_key_nr, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint8_t communication_settings, uint32_t value,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireDecreaseValueFile_PK(uint8_t *aes_key_ext, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint8_t communication_settings, uint32_t value,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireDecreaseValueFile_no_auth(uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint8_t communication_settings, uint32_t value,
		uint16_t *card_status, uint16_t *exec_time);



////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////

UFR_STATUS DL_API EE_Password_Change(const uint8_t old_password[8], const uint8_t new_password[8]);

UFR_STATUS DL_API EE_Lock(const uint8_t password[8], uint32_t lock);

UFR_STATUS DL_API EE_Write(uint32_t address, uint32_t size, void *data);


UFR_STATUS DL_API EE_Read(uint32_t address, uint32_t size, void *data);

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// XXX: Support for multiple readers with same DLL
//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################
//#################  M U L T I   R E A D E R   S U P P O R T  #################
//#################  M U L T I   R E A D E R   S U P P O R T  #################
//#################  M U L T I   R E A D E R   S U P P O R T  #################
//#################  M U L T I   R E A D E R   S U P P O R T  #################
//#################  M U L T I   R E A D E R   S U P P O R T  #################
//#################  M U L T I   R E A D E R   S U P P O R T  #################
//#################  M U L T I   R E A D E R   S U P P O R T  #################
//#################  M U L T I   R E A D E R   S U P P O R T  #################
//#################  M U L T I   R E A D E R   S U P P O R T  #################
//#################  M U L T I   R E A D E R   S U P P O R T  #################
//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################

//--------------------------------------------------------------------------------------------------
/**
 * This is the main function of the multi-reader support.
 * ReaderList_UpdateAndGetCount() scan all communication ports for compatible devices.
 * Function probes opened readers if still connected, if not close and mark them handles for deletion.
 * If some device is disconnected from system this function should remove its handle.
 *
 * @param NumberOfDevices
 * @return status of execution
 */
UFR_STATUS DL_API ReaderList_UpdateAndGetCount(int32_t * NumberOfDevices);

UFR_STATUS DL_API ReaderList_GetSerialByIndex(int32_t DeviceIndex, uint32_t *lpulSerialNumber);
UFR_STATUS DL_API ReaderList_GetSerialDescriptionByIndex(int32_t DeviceIndex, uint8_t pSerialDescription[8]);

UFR_STATUS DL_API ReaderList_GetTypeByIndex(int32_t DeviceIndex, uint32_t *lpulReaderType);

UFR_STATUS DL_API ReaderList_GetFTDISerialByIndex(int32_t DeviceIndex, char ** Device_Serial);
UFR_STATUS DL_API ReaderList_GetFTDIDescriptionByIndex(int32_t DeviceIndex, char ** Device_Description);

UFR_STATUS DL_API ReaderList_OpenByIndex(const int32_t DeviceIndex, UFR_HANDLE *hndUFR);

// not implemented
//UFR_STATUS DL_API ReaderList_OpenBySerial(const char Device_SN[16], UFR_HANDLE *hndUFR);


/**
 * Function for getting all relevant information about connected readers.
 *
 * Eg. If you have tree connected readers, detected by ReaderList_UpdateAndGetCount(),
 * you should call this function tree times.
 *
 * @param DeviceHandle
 * @param DeviceSerialNumber
 * @param DeviceType
 * @param DeviceFWver
 * @param DeviceCommID
 * @param DeviceCommSpeed
 * @param DeviceCommFTDISerial
 * @param DeviceCommFTDIDescription
 * @param DeviceIsOpened
 * @param DeviceStatus
 * @return
 */
UFR_STATUS DL_API ReaderList_GetInformation( //
		UFR_HANDLE *DeviceHandle, //// assigned Handle
		c_string *DeviceSerialNumber, //// device serial number
		int *DeviceType, //// device type - device identification in AIS database
		int *DeviceFWver, //// version of firmware
		int *DeviceCommID, //// device identification number (master)
		int *DeviceCommSpeed, //// communication speed
		c_string *DeviceCommFTDISerial, //// FTDI COM port identification
		c_string *DeviceCommFTDIDescription, //// FTDI COM port description
		int *DeviceIsOpened, //// is Device opened
		int *DeviceStatus //// actual device status
);

/**
 * If the handle exists in the list of opened devices,
 * function would try to close communication port and destroy the handle.
 * ( ReaderList_UpdateAndGetCount() will do that automatically in next execution)
 *
 * Good when you identify that the reader is no longer connected, and want to release the handle.
 *
 * @param DeviceHandle the handle that will be destroyed
 * @return
 */
UFR_STATUS DL_API ReaderList_Destroy(UFR_HANDLE DeviceHandle);

//--------------------------------------------------------------------------------------------------

// open first/next Reader and return handle - better to use ReaderList_OpenByIndex()
UFR_STATUS DL_API ReaderOpenM(UFR_HANDLE *hndUFR);
UFR_STATUS DL_API ReaderResetM(UFR_HANDLE hndUFR);
UFR_STATUS DL_API ReaderCloseM(UFR_HANDLE hndUFR);
UFR_STATUS DL_API ReaderSoftRestartM(UFR_HANDLE hndUFR);
UFR_STATUS DL_API GetReaderTypeM(UFR_HANDLE hndUFR, uint32_t *lpulReaderType);
UFR_STATUS DL_API GetReaderSerialNumberM(UFR_HANDLE hndUFR, uint32_t *lpulSerialNumber);

UFR_STATUS DL_API ReaderStillConnectedM(UFR_HANDLE hndUFR, uint32_t *connected);

UFR_STATUS DL_API ReaderKeyWriteM(UFR_HANDLE hndUFR, const uint8_t *aucKey, uint8_t ucKeyIndex);
UFR_STATUS DL_API ReaderKeysLockM(UFR_HANDLE hndUFR, const uint8_t *password);
UFR_STATUS DL_API ReaderKeysUnlockM(UFR_HANDLE hndUFR, const uint8_t *password);
UFR_STATUS DL_API ReaderUISignalM(UFR_HANDLE hndUFR, uint8_t light_signal_mode,
                                     uint8_t beep_signal_mode);
UFR_STATUS DL_API ReadUserDataM(UFR_HANDLE hndUFR, uint8_t *aucData);
UFR_STATUS DL_API WriteUserDataM(UFR_HANDLE hndUFR, const uint8_t *aucData);
UFR_STATUS DL_API GetCardIdM(UFR_HANDLE hndUFR, uint8_t *lpucCardType, uint32_t *lpulCardSerial);
UFR_STATUS DL_API BlockReadM(UFR_HANDLE hndUFR, uint8_t *data,
                                uint8_t block_address,
                                uint8_t auth_mode,
                                uint8_t key_index);
UFR_STATUS DL_API BlockWriteM(UFR_HANDLE hndUFR, const uint8_t *data,
                                 uint8_t block_address,
                                 uint8_t auth_mode,
                                 uint8_t key_index);
UFR_STATUS DL_API BlockInSectorReadM(UFR_HANDLE hndUFR, uint8_t *data,
                                        uint8_t sector_address,
                                        uint8_t block_in_sector_address,
                                        uint8_t auth_mode,
                                        uint8_t key_index);
UFR_STATUS DL_API BlockInSectorWriteM(UFR_HANDLE hndUFR, const uint8_t *data,
                                         uint8_t sector_address,
                                         uint8_t block_in_sector_address,
                                         uint8_t auth_mode,
                                         uint8_t key_index);
UFR_STATUS DL_API LinearReadM(UFR_HANDLE hndUFR, uint8_t *aucData,
                                 uint16_t usLinearAddress,
                                 uint16_t usDataLength,
                                 uint16_t *lpusBytesReturned,
                                 uint8_t ucKeyMode,
                                 uint8_t ucReaderKeyIndex);
UFR_STATUS DL_API LinRowReadM(UFR_HANDLE hndUFR, uint8_t *aucData,
                                 uint16_t usLinearAddress,
                                 uint16_t usDataLength,
                                 uint16_t *lpusBytesReturned,
                                 uint8_t ucKeyMode,
                                 uint8_t ucReaderKeyIndex);
UFR_STATUS DL_API LinearWriteM(UFR_HANDLE hndUFR, const uint8_t *aucData,
                                  uint16_t usLinearAddress,
                                  uint16_t usDataLength,
                                  uint16_t *lpusBytesWritten,
                                  uint8_t ucKeyMode,
                                  uint8_t ucReaderKeyIndex);
UFR_STATUS DL_API LinearFormatCardM(UFR_HANDLE hndUFR, const uint8_t *new_key_A,
                                       uint8_t blocks_access_bits,
                                       uint8_t sector_trailers_access_bits,
                                       uint8_t sector_trailers_byte9,
                                       const uint8_t *new_key_B,
                                       uint8_t *lpucSectorsFormatted,
                                       uint8_t auth_mode,
                                       uint8_t key_index);
UFR_STATUS DL_API SectorTrailerWriteM(UFR_HANDLE hndUFR, uint8_t addressing_mode,
                                         uint8_t address,
                                         const uint8_t *new_key_A,
                                         uint8_t block0_access_bits,
                                         uint8_t block1_access_bits,
                                         uint8_t block2_access_bits,
                                         uint8_t sector_trailer_access_bits,
                                         uint8_t sector_trailer_byte9,
                                         const uint8_t *new_key_B,
                                         uint8_t auth_mode,
                                         uint8_t key_index);
UFR_STATUS DL_API SectorTrailerWriteUnsafeM(UFR_HANDLE hndUFR, uint8_t addressing_mode,
                                               uint8_t address,
                                               const uint8_t *sector_trailer,
                                               uint8_t auth_mode,
                                               uint8_t key_index);
UFR_STATUS DL_API ValueBlockReadM(UFR_HANDLE hndUFR, int32_t *value,
                                     uint8_t *value_addr,
                                     uint8_t block_address,
                                     uint8_t auth_mode,
                                     uint8_t key_index);
UFR_STATUS DL_API ValueBlockInSectorReadM(UFR_HANDLE hndUFR, int32_t *value,
                                             uint8_t *value_addr,
                                             uint8_t sector_address,
                                             uint8_t block_in_sector_address,
                                             uint8_t auth_mode,
                                             uint8_t key_index);
UFR_STATUS DL_API ValueBlockWriteM(UFR_HANDLE hndUFR, int32_t value,
                                      uint8_t value_addr,
                                      uint8_t block_address,
                                      uint8_t auth_mode,
                                      uint8_t key_index);
UFR_STATUS DL_API ValueBlockInSectorWriteM(UFR_HANDLE hndUFR, int32_t value,
                                              uint8_t value_addr,
                                              uint8_t sector_address,
                                              uint8_t block_in_sector_address,
                                              uint8_t auth_mode,
                                              uint8_t key_index);
UFR_STATUS DL_API ValueBlockIncrementM(UFR_HANDLE hndUFR, int32_t increment_value,
                                          uint8_t block_address,
                                          uint8_t auth_mode,
                                          uint8_t key_index);
UFR_STATUS DL_API ValueBlockInSectorIncrementM(UFR_HANDLE hndUFR, int32_t increment_value,
                                                  uint8_t sector_address,
                                                  uint8_t block_in_sector_address,
                                                  uint8_t auth_mode,
                                                  uint8_t key_index);
UFR_STATUS DL_API ValueBlockDecrementM(UFR_HANDLE hndUFR, int32_t decrement_value,
                                          uint8_t block_address,
                                          uint8_t auth_mode,
                                          uint8_t key_index);
UFR_STATUS DL_API ValueBlockInSectorDecrementM(UFR_HANDLE hndUFR, int32_t decrement_value,
                                                  uint8_t sector_address,
                                                  uint8_t block_in_sector_address,
                                                  uint8_t auth_mode,
                                                  uint8_t key_index);

UFR_STATUS DL_API BlockRead_AKM1M(UFR_HANDLE hndUFR, uint8_t *data,
                                    uint8_t block_address,
                                    uint8_t auth_mode);
UFR_STATUS DL_API BlockWrite_AKM1M(UFR_HANDLE hndUFR, const uint8_t *data,
                                 uint8_t block_address,
                                 uint8_t auth_mode);
UFR_STATUS DL_API BlockInSectorRead_AKM1M(UFR_HANDLE hndUFR, uint8_t *data,
                                        uint8_t sector_address,
                                        uint8_t block_in_sector_address,
                                        uint8_t auth_mode);
UFR_STATUS DL_API BlockInSectorWrite_AKM1M(UFR_HANDLE hndUFR, const uint8_t *data,
                                         uint8_t sector_address,
                                         uint8_t block_in_sector_address,
                                         uint8_t auth_mode);
UFR_STATUS DL_API LinearRead_AKM1M(UFR_HANDLE hndUFR, uint8_t *data,
                                 uint16_t linear_address,
                                 uint16_t length,
                                 uint16_t *bytes_returned,
                                 uint8_t auth_mode);
UFR_STATUS DL_API LinRowRead_AKM1M(UFR_HANDLE hndUFR, uint8_t *data,
                                 uint16_t linear_address,
                                 uint16_t length,
                                 uint16_t *bytes_returned,
                                 uint8_t auth_mode);
UFR_STATUS DL_API LinearWrite_AKM1M(UFR_HANDLE hndUFR, const uint8_t *data,
                                  uint16_t linear_address,
                                  uint16_t length,
                                  uint16_t *bytes_written,
                                  uint8_t auth_mode);
UFR_STATUS DL_API LinearFormatCard_AKM1M(UFR_HANDLE hndUFR, const uint8_t *new_key_A,
                                       uint8_t blocks_access_bits,
                                       uint8_t sector_trailers_access_bits,
                                       uint8_t sector_trailers_byte9,
                                       const uint8_t *new_key_B,
                                       uint8_t *lpucSectorsFormatted,
                                       uint8_t auth_mode);
UFR_STATUS DL_API SectorTrailerWrite_AKM1M(UFR_HANDLE hndUFR, uint8_t addressing_mode,
                                         uint8_t address,
                                         const uint8_t *new_key_A,
                                         uint8_t block0_access_bits,
                                         uint8_t block1_access_bits,
                                         uint8_t block2_access_bits,
                                         uint8_t sector_trailer_access_bits,
                                         uint8_t sector_trailer_byte9,
                                         const uint8_t *new_key_B,
                                         uint8_t auth_mode);
UFR_STATUS DL_API SectorTrailerWriteUnsafe_AKM1M(UFR_HANDLE hndUFR, uint8_t addressing_mode,
                                               uint8_t address,
                                               const uint8_t *sector_trailer,
                                               uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockRead_AKM1M(UFR_HANDLE hndUFR, int32_t *value,
                                     uint8_t *value_addr,
                                     uint8_t block_address,
                                     uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockInSectorRead_AKM1M(UFR_HANDLE hndUFR, int32_t *value,
                                             uint8_t *value_addr,
                                             uint8_t sector_address,
                                             uint8_t block_in_sector_address,
                                             uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockWrite_AKM1M(UFR_HANDLE hndUFR, int32_t value,
                                      uint8_t value_addr,
                                      uint8_t block_address,
                                      uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockInSectorWrite_AKM1M(UFR_HANDLE hndUFR, int32_t value,
                                              uint8_t value_addr,
                                              uint8_t sector_address,
                                              uint8_t block_in_sector_address,
                                              uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockIncrement_AKM1M(UFR_HANDLE hndUFR, int32_t increment_value,
                                          uint8_t block_address,
                                          uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockInSectorIncrement_AKM1M(UFR_HANDLE hndUFR, int32_t increment_value,
                                                  uint8_t sector_address,
                                                  uint8_t block_in_sector_address,
                                                  uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockDecrement_AKM1M(UFR_HANDLE hndUFR, int32_t decrement_value,
                                          uint8_t block_address,
                                          uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockInSectorDecrement_AKM1M(UFR_HANDLE hndUFR, int32_t decrement_value,
                                                  uint8_t sector_address,
                                                  uint8_t block_in_sector_address,
                                                  uint8_t auth_mode);

UFR_STATUS DL_API BlockRead_AKM2M(UFR_HANDLE hndUFR, uint8_t *data,
                                    uint8_t block_address,
                                    uint8_t auth_mode);
UFR_STATUS DL_API BlockWrite_AKM2M(UFR_HANDLE hndUFR, const uint8_t *data,
                                 uint8_t block_address,
                                 uint8_t auth_mode);
UFR_STATUS DL_API BlockInSectorRead_AKM2M(UFR_HANDLE hndUFR, uint8_t *data,
                                        uint8_t sector_address,
                                        uint8_t block_in_sector_address,
                                        uint8_t auth_mode);
UFR_STATUS DL_API BlockInSectorWrite_AKM2M(UFR_HANDLE hndUFR, const uint8_t *data,
                                         uint8_t sector_address,
                                         uint8_t block_in_sector_address,
                                         uint8_t auth_mode);
UFR_STATUS DL_API LinearRead_AKM2M(UFR_HANDLE hndUFR, uint8_t *data,
                                 uint16_t linear_address,
                                 uint16_t length,
                                 uint16_t *bytes_returned,
                                 uint8_t auth_mode);
UFR_STATUS DL_API LinRowRead_AKM2M(UFR_HANDLE hndUFR, uint8_t *data,
                                 uint16_t linear_address,
                                 uint16_t length,
                                 uint16_t *bytes_returned,
                                 uint8_t auth_mode);
UFR_STATUS DL_API LinearWrite_AKM2M(UFR_HANDLE hndUFR, const uint8_t *data,
                                  uint16_t linear_address,
                                  uint16_t length,
                                  uint16_t *bytes_written,
                                  uint8_t auth_mode);
UFR_STATUS DL_API LinearFormatCard_AKM2M(UFR_HANDLE hndUFR, const uint8_t *new_key_A,
                                       uint8_t blocks_access_bits,
                                       uint8_t sector_trailers_access_bits,
                                       uint8_t sector_trailers_byte9,
                                       const uint8_t *new_key_B,
                                       uint8_t *lpucSectorsFormatted,
                                       uint8_t auth_mode);
UFR_STATUS DL_API SectorTrailerWrite_AKM2M(UFR_HANDLE hndUFR, uint8_t addressing_mode,
                                         uint8_t address,
                                         const uint8_t *new_key_A,
                                         uint8_t block0_access_bits,
                                         uint8_t block1_access_bits,
                                         uint8_t block2_access_bits,
                                         uint8_t sector_trailer_access_bits,
                                         uint8_t sector_trailer_byte9,
                                         const uint8_t *new_key_B,
                                         uint8_t auth_mode);
UFR_STATUS DL_API SectorTrailerWriteUnsafe_AKM2M(UFR_HANDLE hndUFR, uint8_t addressing_mode,
                                               uint8_t address,
                                               const uint8_t *sector_trailer,
                                               uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockRead_AKM2M(UFR_HANDLE hndUFR, int32_t *value,
                                     uint8_t *value_addr,
                                     uint8_t block_address,
                                     uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockInSectorRead_AKM2M(UFR_HANDLE hndUFR, int32_t *value,
                                             uint8_t *value_addr,
                                             uint8_t sector_address,
                                             uint8_t block_in_sector_address,
                                             uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockWrite_AKM2M(UFR_HANDLE hndUFR, int32_t value,
                                      uint8_t value_addr,
                                      uint8_t block_address,
                                      uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockInSectorWrite_AKM2M(UFR_HANDLE hndUFR, int32_t value,
                                              uint8_t value_addr,
                                              uint8_t sector_address,
                                              uint8_t block_in_sector_address,
                                              uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockIncrement_AKM2M(UFR_HANDLE hndUFR, int32_t increment_value,
                                          uint8_t block_address,
                                          uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockInSectorIncrement_AKM2M(UFR_HANDLE hndUFR, int32_t increment_value,
                                                  uint8_t sector_address,
                                                  uint8_t block_in_sector_address,
                                                  uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockDecrement_AKM2M(UFR_HANDLE hndUFR, int32_t decrement_value,
                                          uint8_t block_address,
                                          uint8_t auth_mode);
UFR_STATUS DL_API ValueBlockInSectorDecrement_AKM2M(UFR_HANDLE hndUFR, int32_t decrement_value,
                                                  uint8_t sector_address,
                                                  uint8_t block_in_sector_address,
                                                  uint8_t auth_mode);

UFR_STATUS DL_API BlockRead_PKM(UFR_HANDLE hndUFR, uint8_t *data,
                                   uint8_t block_address,
                                   uint8_t auth_mode,
                                   const uint8_t *key);
UFR_STATUS DL_API BlockWrite_PKM(UFR_HANDLE hndUFR, const uint8_t *data,
                                    uint8_t block_address,
                                    uint8_t auth_mode,
                                    const uint8_t *key);
UFR_STATUS DL_API BlockInSectorRead_PKM(UFR_HANDLE hndUFR, uint8_t *data,
                                           uint8_t sector_address,
                                           uint8_t block_in_sector_address,
                                           uint8_t auth_mode,
                                           const uint8_t *key);
UFR_STATUS DL_API BlockInSectorWrite_PKM(UFR_HANDLE hndUFR, const uint8_t *data,
                                            uint8_t sector_address,
                                            uint8_t block_in_sector_address,
                                            uint8_t auth_mode,
                                            const uint8_t *key);
UFR_STATUS DL_API LinearRead_PKM(UFR_HANDLE hndUFR, uint8_t *data,
                                    uint16_t linear_address,
                                    uint16_t length,
                                    uint16_t *bytes_returned,
                                    uint8_t auth_mode,
                                    const uint8_t *key);
UFR_STATUS DL_API LinRowRead_PKM(UFR_HANDLE hndUFR, uint8_t *data,
                                    uint16_t linear_address,
                                    uint16_t length,
                                    uint16_t *bytes_returned,
                                    uint8_t auth_mode,
                                    const uint8_t *key);
UFR_STATUS DL_API LinearWrite_PKM(UFR_HANDLE hndUFR, const uint8_t *data,
                                     uint16_t linear_address,
                                     uint16_t length,
                                     uint16_t *bytes_written,
                                     uint8_t auth_mode,
                                     const uint8_t *key);
UFR_STATUS DL_API LinearFormatCard_PKM(UFR_HANDLE hndUFR, const uint8_t *new_key_A,
                                          uint8_t blocks_access_bits,
                                          uint8_t sector_trailers_access_bits,
                                          uint8_t sector_trailers_byte9,
                                          const uint8_t *new_key_B,
                                          uint8_t *lpucSectorsFormatted,
                                          uint8_t auth_mode,
                                          const uint8_t *key);
UFR_STATUS DL_API SectorTrailerWrite_PKM(UFR_HANDLE hndUFR, uint8_t addressing_mode,
                                            uint8_t address,
                                            const uint8_t *new_key_A,
                                            uint8_t block0_access_bits,
                                            uint8_t block1_access_bits,
                                            uint8_t block2_access_bits,
                                            uint8_t sector_trailer_access_bits,
                                            uint8_t sector_trailer_byte9,
                                            const uint8_t *new_key_B,
                                            uint8_t auth_mode,
                                            const uint8_t *key);
UFR_STATUS DL_API SectorTrailerWriteUnsafe_PKM(UFR_HANDLE hndUFR, uint8_t addressing_mode,
                                                  uint8_t address,
                                                  const uint8_t *sector_trailer,
                                                  uint8_t auth_mode,
                                                  const uint8_t *key);
UFR_STATUS DL_API ValueBlockRead_PKM(UFR_HANDLE hndUFR, int32_t *value,
                                        uint8_t *value_addr,
                                        uint8_t block_address,
                                        uint8_t auth_mode,
                                        const uint8_t *key);
UFR_STATUS DL_API ValueBlockInSectorRead_PKM(UFR_HANDLE hndUFR, int32_t *value,
                                                uint8_t *value_addr,
                                                uint8_t sector_address,
                                                uint8_t block_in_sector_address,
                                                uint8_t auth_mode,
                                                const uint8_t *key);
UFR_STATUS DL_API ValueBlockWrite_PKM(UFR_HANDLE hndUFR, int32_t value,
                                         uint8_t value_addr,
                                         uint8_t block_address,
                                         uint8_t auth_mode,
                                         const uint8_t *key);
UFR_STATUS DL_API ValueBlockInSectorWrite_PKM(UFR_HANDLE hndUFR, int32_t value,
                                                 uint8_t value_addr,
                                                 uint8_t sector_address,
                                                 uint8_t block_in_sector_address,
                                                 uint8_t auth_mode,
                                                 const uint8_t *key);
UFR_STATUS DL_API ValueBlockIncrement_PKM(UFR_HANDLE hndUFR, int32_t increment_value,
                                             uint8_t block_address,
                                             uint8_t auth_mode,
                                             const uint8_t *key);
UFR_STATUS DL_API ValueBlockInSectorIncrement_PKM(UFR_HANDLE hndUFR, int32_t increment_value,
                                                     uint8_t sector_address,
                                                     uint8_t block_in_sector_address,
                                                     uint8_t auth_mode,
                                                     const uint8_t *key);
UFR_STATUS DL_API ValueBlockDecrement_PKM(UFR_HANDLE hndUFR, int32_t decrement_value,
                                             uint8_t block_address,
                                             uint8_t auth_mode,
                                             const uint8_t *key);
UFR_STATUS DL_API ValueBlockInSectorDecrement_PKM(UFR_HANDLE hndUFR, int32_t decrement_value,
                                                     uint8_t sector_address,
                                                     uint8_t block_in_sector_address,
                                                     uint8_t auth_mode,
                                                     const uint8_t *key);
UFR_STATUS DL_API GetReaderHardwareVersionM(UFR_HANDLE hndUFR, uint8_t *version_major,
                                               uint8_t *version_minor);
UFR_STATUS DL_API GetReaderFirmwareVersionM(UFR_HANDLE hndUFR, uint8_t *version_major,
                                               uint8_t *version_minor);

// New commands (for RTC & I2C EEPROM):
UFR_STATUS DL_API GetReaderTimeM(UFR_HANDLE hndUFR, uint8_t *time);
UFR_STATUS DL_API SetReaderTimeM(UFR_HANDLE hndUFR, uint8_t *password, uint8_t *time);
UFR_STATUS DL_API ChangeReaderPasswordM(UFR_HANDLE hndUFR, uint8_t *old_password, uint8_t *new_password);
UFR_STATUS DL_API ReaderEepromWriteM(UFR_HANDLE hndUFR, uint8_t *data, uint32_t address, uint32_t size, uint8_t *password);
UFR_STATUS DL_API ReaderEepromReadM(UFR_HANDLE hndUFR, uint8_t *data, uint32_t address, uint32_t size);
UFR_STATUS DL_API ChangeReaderJobIdM(UFR_HANDLE hndUFR, uint8_t *job_id, uint8_t *new_password);

UFR_STATUS DL_API GetReaderSerialDescriptionM(UFR_HANDLE hndUFR, uint8_t pSerialDescription[8]);

// New since version 2.0:
UFR_STATUS DL_API GetBuildNumberM(UFR_HANDLE hndUFR, uint8_t *build);
UFR_STATUS DL_API GetCardIdExM(UFR_HANDLE hndUFR, uint8_t *lpucSak,
		uint8_t *aucUid, uint8_t *lpucUidSize);
UFR_STATUS DL_API GetLastCardIdExM(UFR_HANDLE hndUFR, uint8_t *lpucSak,
		uint8_t *aucUid, uint8_t *lpucUidSize);

//------------------------------------------------------------------------------
//Multi card mode:
//------------------------------------------------------------------------------
UFR_STATUS DL_API EnableAntiCollisionM(UFR_HANDLE hndUFR);
UFR_STATUS DL_API DisableAntiCollisionM(UFR_HANDLE hndUFR);
UFR_STATUS DL_API EnumCardsM(UFR_HANDLE hndUFR, uint8_t *lpucCardsNumber, uint8_t *lpucUidListSize); // Card pointer is on the first card in list
UFR_STATUS DL_API ListCardsM(UFR_HANDLE hndUFR, uint8_t *aucUidList, uint8_t ucUidListSize); // Before calling this function you must call EnumCards() first.
UFR_STATUS DL_API SelectCardM(UFR_HANDLE hndUFR, const uint8_t *aucUid, uint8_t ucUidSize, uint8_t *lpucSelctedCardType);
UFR_STATUS DL_API DeslectCardM(UFR_HANDLE hndUFR);
UFR_STATUS DL_API GetAntiCollisionStatusM(UFR_HANDLE hndUFR, int8_t *lpcIsAntiCollEnabled, int8_t *lpcIsAnyCardSelected);
//------------------------------------------------------------------------------
UFR_STATUS DL_API GetDlogicCardTypeM(UFR_HANDLE hndUFR, uint8_t *lpucCardType);
UFR_STATUS DL_API GetNfcT2TVersionM(UFR_HANDLE hndUFR, uint8_t lpucVersionResponse[8]);
UFR_STATUS DL_API GetCardSizeM(UFR_HANDLE hndUFR, uint32_t *lpulLinearSize, uint32_t *lpulRawSize);

// uFCoder PRO MODE
UFR_STATUS DL_API GetReaderProModeM(UFR_HANDLE hndUFR, uint32_t *pReaderProMode, uint32_t *pReaderProConfig);
UFR_STATUS DL_API SetReaderProModeM(UFR_HANDLE hndUFR, const uint32_t ReaderProMode);

// QR barcode crypt algorithm
// initialization. with TB serial like 'TB123456'
UFR_STATUS DL_API CardEncryption_InitializeM(UFR_HANDLE hndUFR, const uint8_t *TBSerialString, uint16_t job_number);

// You must define 25 bytes array in memory for CardSerialString[]
UFR_STATUS DL_API CardEncryption_GetNextEncryptedCardM(UFR_HANDLE hndUFR, const uint32_t from_timestamp, const uint32_t to_timestamp,
		uint8_t CardSerialString[25]);

UFR_STATUS DL_API CardEncryption_GetActualCardSNM(UFR_HANDLE hndUFR, uint32_t *ActualCard_SN, uint32_t *ActualCard_SN_LOG);
UFR_STATUS DL_API CardEncryption_GetJobSNM(UFR_HANDLE hndUFR, uint32_t *JobSN);
UFR_STATUS DL_API CardEncryption_GetSalterSNM(UFR_HANDLE hndUFR, uint8_t SalterSN[8], uint8_t * magicByte);

UFR_STATUS DL_API read_ndef_recordM(UFR_HANDLE hndUFR, uint8_t message_nr, uint8_t record_nr, uint8_t *tnf, uint8_t *type_record, uint8_t *type_length, uint8_t *id, uint8_t *id_length,
									   uint8_t *payload, uint32_t *payload_length);
UFR_STATUS DL_API write_ndef_recordM(UFR_HANDLE hndUFR, uint8_t message_nr, uint8_t *tnf, uint8_t *type_record, uint8_t *type_length, uint8_t *id, uint8_t *id_length,
		   uint8_t *payload, uint32_t *payload_length, uint8_t *card_formated);
UFR_STATUS DL_API write_ndef_record_mirroringM(UFR_HANDLE hndUFR, uint8_t message_nr,
		          uint8_t *tnf, uint8_t *type_record, uint8_t *type_length, uint8_t *id,
		          uint8_t *id_length, uint8_t *payload, uint32_t *payload_length,
		          uint8_t *card_formated,
		          int use_uid_ascii_mirror, int use_counter_ascii_mirror, uint32_t payload_mirroring_pos);
UFR_STATUS DL_API get_ndef_record_countM(UFR_HANDLE hndUFR, uint8_t *ndef_message_cnt, uint8_t *ndef_record_cnt, uint8_t *ndef_record_array, uint8_t *empty_ndef_message_cnt);
UFR_STATUS DL_API erase_last_ndef_recordM(UFR_HANDLE hndUFR, uint8_t message_nr);
UFR_STATUS DL_API erase_all_ndef_recordsM(UFR_HANDLE hndUFR, uint8_t message_nr);
UFR_STATUS DL_API ndef_card_initializationM(UFR_HANDLE hndUFR);
//---------------------------------------------------------------------
// Card emulation:
//---------------------------------------------------------------------
UFR_STATUS DL_API WriteEmulationNdefM(UFR_HANDLE hndUFR,
		uint8_t tnf,
		uint8_t* type_record,
		uint8_t type_length,
		uint8_t* id,
		uint8_t id_length,
		uint8_t* payload,
		uint8_t payload_length);
UFR_STATUS DL_API TagEmulationStartM(UFR_HANDLE hndUFR);
UFR_STATUS DL_API TagEmulationStopM(UFR_HANDLE hndUFR);
UFR_STATUS DL_API CombinedModeEmulationStartM(UFR_HANDLE hndUFR);
UFR_STATUS DL_API AdHocEmulationStartM(UFR_HANDLE hndUFR);
UFR_STATUS DL_API AdHocEmulationStopM(UFR_HANDLE hndUFR);
UFR_STATUS DL_API GetAdHocEmulationParamsM(UFR_HANDLE hndUFR,
											uint8_t *ThresholdMinLevel, uint8_t *ThresholdCollLevel,
											uint8_t *RFLevelAmp, uint8_t *RxGain, uint8_t *RFLevel);
UFR_STATUS DL_API SetAdHocEmulationParamsM(UFR_HANDLE hndUFR,
											uint8_t ThresholdMinLevel, uint8_t ThresholdCollLevel,
											uint8_t RFLevelAmp, uint8_t RxGain, uint8_t RFLevel);
UFR_STATUS DL_API GetExternalFieldStateM(UFR_HANDLE hndUFR, uint8_t *is_field_present);
//------------------------------------------------------------------------------
UFR_STATUS DL_API ReadECCSignatureM(UFR_HANDLE hndUFR, uint8_t lpucECCSignature[ECC_SIG_LEN],
		uint8_t lpucUid[MAX_UID_LEN], uint8_t *lpucUidLen, uint8_t *lpucDlogicCardType);

//------------------------------------------------------------------------------
UFR_STATUS DL_API ReadCounterM(UFR_HANDLE hndUFR, uint8_t counter_address, uint32_t *value);
UFR_STATUS DL_API IncrementCounterM(UFR_HANDLE hndUFR, uint8_t counter_address, uint32_t inc_value);
UFR_STATUS DL_API ReadNFCCounterM(UFR_HANDLE hndUFR, uint32_t *value); // Same as ReadCounter(2, &value);
UFR_STATUS DL_API ReadNFCCounterPwdAuth_RKM(UFR_HANDLE hndUFR, uint32_t *value, uint8_t reader_key_index);
UFR_STATUS DL_API ReadNFCCounterPwdAuth_PKM(UFR_HANDLE hndUFR, uint32_t *value, const uint8_t *key);
//------------------------------------------------------------------------------

UFR_STATUS DL_API SetAsyncCardIdSendConfigM(UFR_HANDLE hndUFR,
		uint8_t send_enable, uint8_t prefix_enable, uint8_t prefix,
		uint8_t suffix, uint8_t send_removed_enable, uint32_t async_baud_rate);

UFR_STATUS DL_API SetAsyncCardIdSendConfigExM(UFR_HANDLE hndUFR,
		uint8_t send_enable, uint8_t prefix_enable, uint8_t prefix,
		uint8_t suffix, uint8_t send_removed_enable, uint8_t reverse_byte_order,
		uint8_t decimal_representation, uint32_t async_baud_rate);

UFR_STATUS DL_API GetAsyncCardIdSendConfigM(UFR_HANDLE hndUFR,
		uint8_t *send_enable, uint8_t *prefix_enable, uint8_t *prefix,
		uint8_t *suffix, uint8_t *send_removed_enable, uint32_t *async_baud_rate);

UFR_STATUS DL_API GetAsyncCardIdSendConfigExM(UFR_HANDLE hndUFR,
		uint8_t *send_enable, uint8_t *prefix_enable, uint8_t *prefix,
		uint8_t *suffix, uint8_t *send_removed_enable, uint8_t *reverse_byte_order,
		uint8_t *decimal_representation, uint32_t *async_baud_rate);

/*****************************************************************************
 * AIS FUNCTIONS
 *****************************************************************************
 */

UFR_STATUS DL_API ais_get_card_numberM(UFR_HANDLE hndUFR, uint32_t *card_number);

UFR_STATUS DL_API ais_set_right_recordM(UFR_HANDLE hndUFR,
		uint8_t record_number, uint16_t first_reader_nr,
		uint16_t last_reader_nr, uint8_t start_hour, uint8_t start_minute,
		uint8_t end_hour, uint8_t end_minute, uint8_t *days);
UFR_STATUS DL_API ais_get_right_recordM(UFR_HANDLE hndUFR,
		uint8_t record_number, uint16_t *first_reader_nr,
		uint16_t *last_reader_nr, uint8_t *start_hour,
		uint8_t *start_minute, uint8_t *end_hour, uint8_t *end_minute,
		uint8_t *days);
UFR_STATUS DL_API ais_erase_right_recordM(UFR_HANDLE hndUFR,
		uint8_t record_number);
UFR_STATUS DL_API ais_set_validate_recordM(UFR_HANDLE hndUFR,
		uint8_t begin_year, uint8_t begin_month, uint8_t begin_day,
		uint8_t begin_hour, uint8_t begin_minute, uint8_t end_year,
		uint8_t end_month, uint8_t end_day, uint8_t end_hour,
		uint8_t end_minute);
UFR_STATUS DL_API ais_get_validate_recordM(UFR_HANDLE hndUFR,
		uint8_t *begin_year, uint8_t *begin_month, uint8_t *begin_day,
		uint8_t *begin_hour, uint8_t *begin_minute, uint8_t *end_year,
		uint8_t *end_month, uint8_t *end_day, uint8_t *end_hour,
		uint8_t *end_minute);
UFR_STATUS DL_API ais_set_card_typeM(UFR_HANDLE hndUFR, uint8_t card_type);
UFR_STATUS DL_API ais_get_card_typeM(UFR_HANDLE hndUFR, uint8_t *card_type);
UFR_STATUS DL_API ais_set_card_daily_durationM(UFR_HANDLE hndUFR, uint16_t duration);
UFR_STATUS DL_API ais_get_card_daily_durationM(UFR_HANDLE hndUFR, uint16_t *duration);
UFR_STATUS DL_API ais_set_card_total_durationM(UFR_HANDLE hndUFR, uint32_t duration);
UFR_STATUS DL_API ais_get_card_total_durationM(UFR_HANDLE hndUFR, uint32_t *duration);

// swimming pool **************************************************************

UFR_STATUS DL_API ais_get_credit_and_period_validityM(UFR_HANDLE hndUFR,
		int32_t *credit, uint32_t *begin_year, uint32_t *begin_month,
		uint32_t *begin_day, uint32_t *begin_hour, uint32_t *begin_minute, //
		uint32_t *end_year, uint32_t *end_month, uint32_t *end_day,
		uint32_t *end_hour, uint32_t *end_minute);

UFR_STATUS DL_API ais_set_credit_and_period_validityM(UFR_HANDLE hndUFR,
		int32_t credit, uint32_t begin_year, uint32_t begin_month,
		uint32_t begin_day, uint32_t begin_hour, uint32_t begin_minute, //
		uint32_t end_year, uint32_t end_month, uint32_t end_day,
		uint32_t end_hour, uint32_t end_minute);

UFR_STATUS DL_API ais_set_right_type_recordM(UFR_HANDLE hndUFR, uint8_t record_number,
		uint8_t right_record_type, uint8_t *right_data);

UFR_STATUS DL_API ais_get_right_type_recordM(UFR_HANDLE hndUFR,uint8_t record_number,
		uint8_t *right_record_type, uint8_t *right_data);

UFR_STATUS DL_API ais_set_right_record_type_max_daily_counterM(UFR_HANDLE hndUFR, uint8_t record_number,
		uint16_t first_reader_nr, uint16_t last_reader_nr, uint8_t start_hour,
		uint8_t start_minute, uint8_t end_hour, uint8_t end_minute,
		uint8_t *days, uint8_t max_daily_counter);

UFR_STATUS DL_API ais_set_right_record_type_max_daily_counterM(UFR_HANDLE hndUFR, uint8_t record_number,
		uint16_t first_reader_nr, uint16_t last_reader_nr, uint8_t start_hour,
		uint8_t start_minute, uint8_t end_hour, uint8_t end_minute,
		uint8_t *days, uint8_t max_daily_counter);

//=============================================================================

UFR_STATUS DL_API UfrXrcLockOnM(UFR_HANDLE hndUFR, uint16_t pulse_duration);
UFR_STATUS DL_API UfrXrcRelayStateM(UFR_HANDLE hndUFR, uint8_t state);
UFR_STATUS DL_API UfrXrcGetIoStateM(UFR_HANDLE hndUFR, uint8_t *intercom, uint8_t *door, uint8_t *relay_state);
UFR_STATUS DL_API UfrRedLightControlM(UFR_HANDLE hndUFR, uint8_t light_status);

UFR_STATUS DL_API UfrSetBadSelectCardNrMaxM(UFR_HANDLE hndUFR, uint8_t bad_select_nr_max);
UFR_STATUS DL_API UfrGetBadSelectCardNrMaxM(UFR_HANDLE hndUFR, uint8_t *bad_select_nr_max);

UFR_STATUS DL_API UfrEnterSleepModeM(UFR_HANDLE hndUFR);
UFR_STATUS DL_API UfrLeaveSleepModeM(UFR_HANDLE hndUFR);
UFR_STATUS DL_API AutoSleepSetM(UFR_HANDLE hndUFR, uint8_t seconds_wait);
UFR_STATUS DL_API AutoSleepGetM(UFR_HANDLE hndUFR, uint8_t *seconds_wait);

UFR_STATUS DL_API SetSpeedPermanentlyM(UFR_HANDLE hndUFR, unsigned char tx_speed, unsigned char rx_speed);
UFR_STATUS DL_API GetSpeedParametersM(UFR_HANDLE hndUFR, unsigned char *tx_speed, unsigned char *rx_speed);
UFR_STATUS DL_API SetDisplayDataM(UFR_HANDLE hndUFR, uint8_t *display_data, uint8_t data_length);
UFR_STATUS DL_API SetSpeakerFrequencyM(UFR_HANDLE hndUFR, uint16_t frequency);
UFR_STATUS DL_API SetDisplayIntensityM(UFR_HANDLE hndUFR, uint8_t intensity);
UFR_STATUS DL_API GetDisplayIntensityM(UFR_HANDLE hndUFR, uint8_t *intensity);

//#############################################################################
//#############################################################################

UFR_STATUS DL_API SetISO14443_4_ModeM(UFR_HANDLE hndUFR);
UFR_STATUS DL_API uFR_i_block_transceiveM(UFR_HANDLE hndUFR, uint8_t chaining,
		uint8_t timeout, uint8_t block_length, uint8_t *snd_data_array,
		size_t *rcv_length, uint8_t *rcv_data_array, uint32_t *ufr_status);

UFR_STATUS DL_API uFR_APDU_TransceiveM(UFR_HANDLE hndUFR, uint8_t cls, uint8_t ins, uint8_t p0, uint8_t p1,
		uint8_t *data_out, uint8_t data_out_len, uint8_t *data_in, uint32_t max_data_in_len, uint32_t *response_len,
		uint8_t send_le, uint8_t *apdu_status);

UFR_STATUS DL_API APDUHexStrTransceiveM(UFR_HANDLE hndUFR, const char *c_apdu, char **r_apdu);
UFR_STATUS DL_API APDUPlainTransceiveM(UFR_HANDLE hndUFR, const uint8_t *c_apdu, uint32_t c_apdu_len, uint8_t *r_apdu, uint32_t *r_apdu_len);
UFR_STATUS DL_API APDUTransceiveM(UFR_HANDLE hndUFR, uint8_t cls, uint8_t ins, uint8_t p0, uint8_t p1,
		const uint8_t *data_out, uint32_t Nc, uint8_t *data_in, uint32_t *Ne,
		uint8_t send_le, uint8_t *apdu_status);
UFR_STATUS DL_API i_block_trans_rcv_chainM(UFR_HANDLE hndUFR, uint8_t chaining,	uint8_t timeout,
		uint8_t block_length, uint8_t *snd_data_array,
		uint8_t *rcv_length, uint8_t *rcv_data_array, uint8_t *rcv_chained, uint32_t *ufr_status);

UFR_STATUS DL_API r_block_transceiveM(UFR_HANDLE hndUFR, uint8_t ack, uint8_t timeout,
		uint8_t *rcv_length, uint8_t *rcv_data_array, uint8_t *rcv_chained, uint32_t *ufr_status);

UFR_STATUS DL_API s_block_deselectM(UFR_HANDLE hndUFR, uint8_t timeout);

//#############################################################################

//DL_API
//void print_desfire_version(struct mifare_desfire_version_info *desfire_version);

typedef void * HND;

UFR_STATUS DL_API uFR_DESFIRE_Start(void);
UFR_STATUS DL_API uFR_DESFIRE_StartM(UFR_HANDLE hndUFR);
UFR_STATUS DL_API uFR_DESFIRE_Stop(void);
UFR_STATUS DL_API uFR_DESFIRE_StopM(UFR_HANDLE hndUFR);
UFR_STATUS DL_API uFR_APDU_Start(void);                 // Alias for uFR_DESFIRE_Start()
UFR_STATUS DL_API uFR_APDU_StartM(UFR_HANDLE hndUFR);   // Alias for uFR_DESFIRE_StartM()
UFR_STATUS DL_API uFR_APDU_Stop(void);                  // Alias for uFR_DESFIRE_Stop()
UFR_STATUS DL_API uFR_APDU_StopM(UFR_HANDLE hndUFR);    // Alias for uFR_DESFIRE_StopM()

HND DL_API uFR_mifare_desfire_tag_new (void);

void DL_API uFR_mifare_desfire_tag_free (HND tag);

HND DL_API uFR_mifare_desfire_des_key_new (uint8_t value[8]);

int DL_API uFR_mifare_desfire_get_key_settings (HND tag, uint8_t *settings, uint8_t *max_keys);

void DL_API uFR_mifare_desfire_key_free (HND key);

HND DL_API uFR_mifare_desfire_aes_key_new_with_version (uint8_t value[16], uint8_t version);

int DL_API uFR_mifare_desfire_change_key (HND tag, uint8_t key_no, HND new_key, HND old_key);

int DL_API uFR_mifare_desfire_authenticate (HND tag, uint8_t key_no, HND key);

int DL_API uFR_mifare_desfire_authenticate_aes (HND tag, uint8_t key_no, HND key);

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

UFR_STATUS DL_API uFR_int_GetDesfireUidM(UFR_HANDLE hndUFR, uint8_t aes_key_nr, uint32_t aid,
		uint8_t aid_key_nr, uint8_t *card_uid, uint8_t *card_uid_len,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_GetDesfireUid_PK_M(UFR_HANDLE hndUFR,uint8_t *aes_key_ext, uint32_t aid,
		uint8_t aid_key_nr, uint8_t *card_uid, uint8_t *card_uid_len,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireFreeMemM(UFR_HANDLE hndUFR, uint32_t *free_mem_byte, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireFormatCardM(UFR_HANDLE hndUFR, uint8_t aes_key_nr, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireFormatCard_PK_M(UFR_HANDLE hndUFR, uint8_t *aes_key_ext, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireCreateStdDataFileM(UFR_HANDLE hndUFR,uint8_t aes_key_nr, uint32_t aid, uint8_t file_id, uint32_t file_size,
		uint8_t read_key_no, uint8_t write_key_no, uint8_t read_write_key_no, uint8_t change_key_no,
		uint8_t communication_settings, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireCreateStdDataFile_PK_M(UFR_HANDLE hndUFR, uint8_t *aes_key_ext, uint32_t aid, uint8_t file_id, uint32_t file_size,
		uint8_t read_key_no, uint8_t write_key_no, uint8_t read_write_key_no, uint8_t change_key_no,
		uint8_t communication_settings, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireCreateStdDataFile_no_auth_M(UFR_HANDLE hndUFR, uint32_t aid, uint8_t file_id, uint32_t file_size,
		uint8_t read_key_no, uint8_t write_key_no, uint8_t read_write_key_no, uint8_t change_key_no,
		uint8_t communication_settings, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireDeleteFileM(UFR_HANDLE hndUFR, uint8_t aes_key_nr, uint32_t aid, uint8_t file_id,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireDeleteFile_PK_M(UFR_HANDLE hndUFR, uint8_t *aes_key_ext, uint32_t aid, uint8_t file_id,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireDeleteFile_no_auth_M(UFR_HANDLE hndUFR, uint32_t aid, uint8_t file_id,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireCreateAesApplicationM(UFR_HANDLE hndUFR,uint8_t aes_key_nr, uint32_t aid,
		uint8_t setting, uint8_t max_key_no, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireCreateAesApplication_PK_M(UFR_HANDLE hndUFR, uint8_t *aes_key_ext, uint32_t aid,
		uint8_t setting, uint8_t max_key_no, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireCreateAesApplication_no_auth_M(UFR_HANDLE hndUFR, uint32_t aid,
		uint8_t setting, uint8_t max_key_no, uint16_t *card_status,	uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireDeleteApplicationM(UFR_HANDLE hndUFR, uint8_t aes_key_nr,	uint32_t aid,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireDeleteApplication_PK_M(UFR_HANDLE hndUFR, uint8_t *aes_key_ext, uint32_t aid,
		uint16_t *card_status, uint16_t *exec_time);


UFR_STATUS DL_API uFR_int_DesfireSetConfigurationM(UFR_HANDLE hndUFR, uint8_t aes_key_nr, uint8_t random_uid, uint8_t format_disable,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireSetConfiguration_PK_M(UFR_HANDLE hndUFR, uint8_t *aes_key_ext, uint8_t random_uid, uint8_t format_disable,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireGetKeySettingsM(UFR_HANDLE hndUFR, uint8_t aes_key_nr, uint32_t aid,
		uint8_t *setting, uint8_t *max_key_no,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireGetKeySettings_PK_M(UFR_HANDLE hndUFR, uint8_t *aes_key_ext, uint32_t aid,
		uint8_t *setting, uint8_t *max_key_no,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireChangeKeySettingsM(UFR_HANDLE hndUFR,uint8_t aes_key_nr,	uint32_t aid,
		uint8_t setting, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireChangeKeySettings_PK_M(UFR_HANDLE hndUFR, uint8_t *aes_key_ext, uint32_t aid,
		uint8_t setting, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireChangeAesKeyM(UFR_HANDLE hndUFR, uint8_t aes_key_nr, uint32_t aid, uint8_t aid_key_no_auth,
		uint8_t new_aes_key[16], uint8_t aid_key_no, uint8_t old_aes_key[16],
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireChangeAesKey_PK_M(UFR_HANDLE hndUFR, uint8_t *aes_key_ext, uint32_t aid, uint8_t aid_key_no_auth,
		uint8_t new_aes_key[16], uint8_t aid_key_no, uint8_t old_aes_key[16],
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_SetParamM(UFR_HANDLE hndUFR, uint8_t aes_key[16], uint8_t key_no,
		uint32_t aid_nr, uint8_t file_id);


//---------------------------------------------------------------------------

UFR_STATUS DL_API uFR_int_DesfireReaStddDataFileM(UFR_HANDLE hndUFR, uint8_t aes_key_nr, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint16_t offset, uint16_t data_length,
		uint8_t communication_settings, uint8_t *data,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireReadStdDataFile_PK_M(UFR_HANDLE hndUFR, uint8_t *aes_key_ext, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint16_t offset, uint16_t data_length,
		uint8_t communication_settings, uint8_t *data,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireReadStdDataFile_no_auth_M(UFR_HANDLE hndUFR, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint16_t offset, uint16_t data_length,
		uint8_t communication_settings, uint8_t *data,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireWriteStdDataFileM(UFR_HANDLE hndUFR, uint8_t aes_key_nr, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint16_t offset, uint16_t data_length,
		uint8_t communication_settings, uint8_t *data,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireWriteStdDataFile_PK_M(UFR_HANDLE hndUFR, uint8_t *aes_key_ext, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint16_t offset, uint16_t data_length,
		uint8_t communication_settings, uint8_t *data,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireWriteStdDataFile_no_auth_M(UFR_HANDLE hndUFR, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint16_t offset, uint16_t data_length,
		uint8_t communication_settings, uint8_t *data,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireCreateValueFileM(UFR_HANDLE hndUFR, uint8_t aes_key_nr, uint32_t aid, uint8_t file_id,
		int32_t lower_limit, int32_t upper_limit, int32_t value, uint8_t limited_credit_enabled,
		uint8_t read_key_no, uint8_t write_key_no, uint8_t read_write_key_no, uint8_t change_key_no,
		uint8_t communication_settings, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireCreateValueFile_PK_M(UFR_HANDLE hndUFR, uint8_t *aes_key_ext, uint32_t aid, uint8_t file_id,
		int32_t lower_limit, int32_t upper_limit, int32_t value, uint8_t limited_credit_enabled,
		uint8_t read_key_no, uint8_t write_key_no, uint8_t read_write_key_no, uint8_t change_key_no,
		uint8_t communication_settings, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireCreateValueFile_no_auth_M(UFR_HANDLE hndUFR, uint32_t aid, uint8_t file_id,
		int32_t lower_limit, int32_t upper_limit, int32_t value, uint8_t limited_credit_enabled,
		uint8_t read_key_no, uint8_t write_key_no, uint8_t read_write_key_no, uint8_t change_key_no,
		uint8_t communication_settings, uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireReadValueFileM(UFR_HANDLE hndUFR, uint8_t aes_key_nr, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint8_t communication_settings, int32_t *value,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireReadValueFile_PK_M(UFR_HANDLE hndUFR, uint8_t *aes_key_ext, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint8_t communication_settings, int32_t *value,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireReadValueFile_no_auth_M(UFR_HANDLE hndUFR, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint8_t communication_settings, int32_t *value,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireIncreaseValueFileM(UFR_HANDLE hndUFR, uint8_t aes_key_nr, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint8_t communication_settings, uint32_t value,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireIncreaseValueFile_PK_M(UFR_HANDLE hndUFR, uint8_t *aes_key_ext, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint8_t communication_settings, uint32_t value,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireIncreaseValueFile_no_auth_M(UFR_HANDLE hndUFR, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint8_t communication_settings, uint32_t value,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireDecreaseValueFileM(UFR_HANDLE hndUFR, uint8_t aes_key_nr, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint8_t communication_settings, uint32_t value,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireDecreaseValueFile_PK_M(UFR_HANDLE hndUFR, uint8_t *aes_key_ext, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint8_t communication_settings, uint32_t value,
		uint16_t *card_status, uint16_t *exec_time);

UFR_STATUS DL_API uFR_int_DesfireDecreaseValueFile_no_auth_M(UFR_HANDLE hndUFR, uint32_t aid, uint8_t aid_key_nr, uint8_t file_id,
		uint8_t communication_settings, uint32_t value,
		uint16_t *card_status, uint16_t *exec_time);

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// DLL version ----------------------------------------------------------------
uint32_t DL_API GetDllVersion(void);

/*
 * Get "exploded" dll version example:
 *
 *  #include <windows.h>
 *  #include <uFCoder.h>
 *
 *  void main(int argc, char *argv[])
 *  {
 *       uint32_t dwDllVersion = 0;
 *       uint32_t dwDllMajorVersion = 0;
 *       uint32_t dwDllMinorVersion = 0;
 *       uint32_t dwDllBuild = 0;
 *
 *       dwDllVersion = GetDllVersion();
 *
 *       // "explode" the dll version:
 *       dwDllMajorVersion = (DWORD)(LOBYTE(LOWORD(dwDllVersion)));
 *       dwDllMinorVersion = (DWORD)(HIBYTE(LOWORD(dwDllVersion)));
 *
 *       // Get the dll build number.
 *       dwDllBuild = (DWORD)(HIWORD(dwDllVersion));
 *
 *       printf("Dll version is %ld.%ld (%ld)\n", dwDllMajorVersion,
 *                                             dwDllMinorVersion,
 *                                             dwDllBuild);
 *  }
 *
 */

// Originality Check (performs the check is the chip on the card/tag NXP genuine):
UFR_STATUS DL_API OriginalityCheck(const uint8_t *signature, const uint8_t *uid, uint8_t uid_len, uint8_t DlogicCardType);
// Returns:
// UFR_OPEN_SSL_DYNAMIC_LIB_NOT_FOUND in case there is no OpenSSL library (libeay32.dll) in current folder or path
// UFR_OPEN_SSL_DYNAMIC_LIB_FAILED    in case of OpenSSL library error (e.g. wrong OpenSSL version)
// UFR_NOT_NXP_GENUINE                if chip on the card/tag is NOT NXP genuine
// UFR_OK                             is chip on the card/tag is NXP GENUINE

//// debug functions:
c_string DL_API GetDllVersionStr(void);
c_string DL_API UFR_Status2String(const UFR_STATUS status);

//// Helper functions:
#ifndef _WIN32

unsigned long GetTickCount(void);

#endif // #ifndef _WIN32

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

c_string DL_API GetReaderDescription(void);
c_string DL_API GetReaderDescriptionM(UFR_HANDLE hndUFR);

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
UFR_STATUS DL_API test_i_block(void);
UFR_STATUS DL_API test_desfire_ver(void);

#ifdef __cplusplus
}
#endif

#endif /* uFCoder_H_ */
