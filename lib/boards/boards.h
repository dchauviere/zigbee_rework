/* Board ID */

/* Board include */
#if (BOARD_TS0012)
	#include "../boards/board_ts0012.h"
#elif (BOARD_TS0013)
	#include "../boards/board_ts0013.h"
#endif

#if defined(MCU_CORE_826x)
	#define CLOCK_SYS_CLOCK_HZ  		32000000
#elif defined(MCU_CORE_8258)
    #if (CHIP_TYPE == TLSR_8258_1M)
        #define FLASH_CAP_SIZE_1M			1
    #endif
    #define CLOCK_SYS_CLOCK_HZ  		48000000
#elif defined(MCU_CORE_8278)
	#define FLASH_CAP_SIZE_1M			1
	#define CLOCK_SYS_CLOCK_HZ  		48000000
#elif defined(MCU_CORE_B91)
	#define FLASH_CAP_SIZE_1M			1
	#define CLOCK_SYS_CLOCK_HZ  		48000000
#elif defined(MCU_CORE_B92)
	#define FLASH_CAP_SIZE_1M			1
	#define CLOCK_SYS_CLOCK_HZ  		48000000
#else
	#error "MCU is undefined!"
#endif
