#ifndef __COMMON_H
#define __COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

/**
  * @brief boolean typedef
  */
typedef enum{
    TRUE = 1,
    FALSE = 0
}boolean_t;

typedef enum {
	STATE_NONE = 0,
	STATE_LOW,
	STATE_HIGH,
	STATE_FALLING,
	STATE_RISING,
	STATE_STARTING,
	STATE_RUNNING,
	STATE_DESYNC,
	STATE_STOPPED
} state_t;

typedef enum {
	BUTTON_STATE_RELEASED = 0, BUTTON_STATE_PRESSED, BUTTON_STATE_UNDEFINED
} button_physical_state_t;

typedef enum {
	BUTTON_LOGIC_RELEASED = 0, BUTTON_LOGIC_PRESSED, BUTTON_LOGIC_LONG_PRESSED
} button_logical_state_t;

typedef enum {
	ERROR_NONE = 0, ERROR_TIMEOUT, ERROR_BUS, ERROR_INVALIDPOINTER,
	ERROR_ALLOCATION, ERROR_INVALIDARGUMENT, ERROR_BUSBUSY, ERROR_BUSY,
	ERROR_NOTINITIALIZED, ERROR_DATAINVALID, ERROR_FULL, ERROR_OVERFLOW,
	ERROR_EMPTY, ERROR_INVALIDFUNCTION, ERROR_ARRAYBOUNDS, ERROR_NOTIMPLEMENTED,
	ERROR_INVALIDSTATE, ERROR_FILESYSTEM
} error_t;

typedef struct {
	uint32_t day_ones:4;
	uint32_t day_tens:2;
	uint32_t :2;
	uint32_t month_ones:4;
	uint32_t month_tens:1;
	uint32_t week_day:3;
	uint32_t year_ones:4;
	uint32_t year_tens:4;
} date_bcd_t;

typedef struct {
	uint32_t second_ones:4;
	uint32_t second_tens:3;
	uint32_t :1;
	uint32_t minute_ones:4;
	uint32_t minute_tens:3;
	uint32_t :1;
	uint32_t hour_ones:4;
	uint32_t hour_tens:2;
	uint32_t am_pm:1;
} time_bcd_t;

typedef union {
	uint8_t data[8];
	struct {
		date_bcd_t date;
		time_bcd_t time;
	};
} date_time_bcd_t;

typedef union {
	struct {
		uint32_t year:12;
		uint32_t month:4;
		uint32_t date:5;
		uint32_t hours:5;
		uint32_t minutes:6;
		uint32_t seconds:6;
		float seconds_float;
	};
	uint8_t bytes[12];
} date_time_bin_t;

typedef union {
	uint8_t data[7];
	struct {
		uint16_t year;
		uint8_t month;
		uint8_t day_of_month;
		uint8_t hour;
		uint8_t minute;
		uint8_t second;
	};
} date_time_binary_t;

// Endianess dependent
typedef union {
	uint8_t data_uint8[4];
	uint16_t data_uint16[2];
	uint32_t data_uint32;
	float data_float;
} byte_extraction_union_t;

#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "=" VALUE(var)

void limituint8_t(uint8_t*, uint8_t, uint8_t);
void limituint16_t(uint16_t*, uint16_t, uint16_t);
void limituint32_t(uint32_t*, uint32_t, uint32_t);
void limitFloatPointer ( float*, float, float );
float limitFloat( float, float, float );
uint16_t getMaxDayOfMonth ( uint16_t year, uint16_t month );

#ifdef __cplusplus
}
#endif

#endif /* __COMMON_H */
