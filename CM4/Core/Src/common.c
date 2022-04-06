/*
 * common.c
 *
 *  Created on: Feb 3, 2018
 *      Author: mason
 */

#include "common.h"

void limituint16_t(uint16_t *testedValue, uint16_t minBound, uint16_t maxBound)
{
	if(*testedValue < minBound){
		*testedValue = minBound;
		return;
	}
	if(*testedValue > maxBound){
		*testedValue = maxBound;
	}
}

void limituint32_t(uint32_t *testedValue, uint32_t minBound, uint32_t maxBound)
{
	if(*testedValue < minBound){
		*testedValue = minBound;
		return;
	}
	if(*testedValue > maxBound){
		*testedValue = maxBound;
	}
}

void limituint8_t(uint8_t *testedValue, uint8_t minBound, uint8_t maxBound)
{
	if(*testedValue < minBound){
		*testedValue = minBound;
		return;
	}
	if(*testedValue > maxBound){
		*testedValue = maxBound;
	}
}

void limitFloatPointer ( float *tested_value, float min_bound, float max_bound )
{
	if(*tested_value < min_bound)
	{
		*tested_value = min_bound;
		return;
	}
	if(*tested_value > max_bound)
	{
		*tested_value = max_bound;
	}
}

float limitFloat ( float _tested_value, float _min_bound, float _max_bound )
{
  if(_tested_value < _min_bound)
  {
    return _min_bound;
  }
  if(_tested_value > _max_bound)
  {
    return _max_bound;
  }

  return _tested_value;
}

uint16_t getMaxDayOfMonth ( uint16_t year, uint16_t month )
{
	uint16_t return_value = 30;

	switch ( month )
	{
		case 2:
			if (( year % 4 ) == 0 )
			{
				return_value = 29;
			}
			else
			{
				return_value = 28;
			}
			break;
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return_value = 31;
	}

	return return_value;
}
