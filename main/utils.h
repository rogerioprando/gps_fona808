#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>
#include <stdio.h> 
#include <time.h>

int64_t ipow(int32_t base, uint8_t exp);
uint64_t gettimestamp_from_datestr(const char *datestr);
float coord_from_chars(const char *coord_chars, uint16_t len);
uint64_t chars_to_number(const char *s, size_t len, uint8_t base);


#endif /* UTILS_H_ */
