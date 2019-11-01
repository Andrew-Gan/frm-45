
//define SD Card Command
#define CMD0    (0)         /* GO_IDLE_STATE */
#define CMD1    (1)         /* SEND_OP_COND (MMC) */
#define ACMD41  (0x80+41)   /* SEND_OP_COND (SDC) */
#define CMD8    (8)         /* SEND_IF_COND */
#define CMD9    (9)         /* SEND_CSD */
#define CMD10   (10)        /* SEND_CID */
#define CMD12   (12)        /* STOP_TRANSMISSION */
#define ACMD13  (0x80+13)   /* SD_STATUS (SDC) */
#define CMD16   (16)        /* SET_BLOCKLEN */
#define CMD17   (17)        /* READ_SINGLE_BLOCK */
#define CMD18   (18)        /* READ_MULTIPLE_BLOCK */
#define CMD23   (23)        /* SET_BLOCK_COUNT (MMC) */
#define ACMD23  (0x80+23)   /* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24   (24)        /* WRITE_BLOCK */
#define CMD25   (25)        /* WRITE_MULTIPLE_BLOCK */
#define CMD32   (32)        /* ERASE_ER_BLK_START */
#define CMD33   (33)        /* ERASE_ER_BLK_END */
#define CMD38   (38)        /* ERASE */
#define CMD55   (55)        /* APP_CMD */
#define CMD58   (58)        /* READ_OCR */

#define sel_SPI1 1
#define sel_SPI2 2

#define milisecond 1000000
#define minute 1000000000

typedef enum {false,true} bool;

///////////////////////////////////////////////////////////////////////////////////////////////

void init_spi(uint8_t SPIX, uint16_t SPI_MODE, uint16_t SPI_Direction,
uint16_t SPI_DataSize, uint16_t SPI_CPOL, uint16_t SPI_CPHA, uint16_t SPI_NSS,
uint16_t SPI_BaudRatePrescaler, uint16_t SPI_SSO, bool GPIO_as_SS);


///////////////////////////////////////////////////////////////////////////////////////////////

uint8_t SPI_Send_8bit(uint8_t SPIX, uint8_t data);
static BYTE send_cmd(BYTE cmd, DWORD arg);
static bool wait_for_card_ready();
static void deselect_card();
static bool select_card();
DSTATUS SD_initialize ();
