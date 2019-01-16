#include "utils.h"

int64_t ipow(int32_t base, uint8_t exp) {
    static const uint8_t highest_bit_set[] = {
        0, 1, 2, 2, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4, 4, 4,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 6, 6, 6, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 6, 255, // anything past 63 is a guaranteed overflow with base > 1
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255,
    };

    uint64_t result = 1;

    switch (highest_bit_set[exp]) {
    case 255: // we use 255 as an overflow marker and return 0 on overflow/underflow
        if (base == 1) {
            return 1;
        }

        if (base == -1) {
            return 1 - 2 * (exp & 1);
        }

        return 0;
    case 6:
        if (exp & 1) result *= base;
        exp >>= 1;
        base *= base;
    case 5:
        if (exp & 1) result *= base;
        exp >>= 1;
        base *= base;
    case 4:
        if (exp & 1) result *= base;
        exp >>= 1;
        base *= base;
    case 3:
        if (exp & 1) result *= base;
        exp >>= 1;
        base *= base;
    case 2:
        if (exp & 1) result *= base;
        exp >>= 1;
        base *= base;
    case 1:
        if (exp & 1) result *= base;
    default:
        return result;
    }
}

uint64_t gettimestamp_from_datestr(const char *datestr) {
    uint8_t cursor = 0;
    uint8_t fieldlen = 2;
    struct tm t = {0};

    int year = (uint16_t)chars_to_number(datestr + cursor, fieldlen * 2, 10);
    cursor += fieldlen * 2;
    int month = (uint8_t)chars_to_number(datestr + cursor, fieldlen, 10);
    cursor += fieldlen;
    int day = (uint8_t)chars_to_number(datestr + cursor, fieldlen, 10);
    cursor += fieldlen;
    int hour = (uint8_t)chars_to_number(datestr + cursor, fieldlen, 10);
    cursor += fieldlen;
    int min = (uint8_t)chars_to_number(datestr + cursor, fieldlen, 10);
    cursor += fieldlen;
    int sec = (uint8_t)chars_to_number(datestr + cursor, fieldlen, 10);

    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = min;
    t.tm_sec = sec;
    t.tm_isdst = 1;
    return (&t);
}

float coord_from_chars(const char *coord_chars, uint16_t len)
{
    uint16_t cursor = 0;

    int degrees = chars_to_number((const char *)(&coord_chars[cursor]), 2, 10);
    cursor += 2;
    uint16_t minutes = chars_to_number((const char*)(&coord_chars[cursor]), 2, 10);
    cursor += 3;
    int minutes_dec = chars_to_number((const char *)(&coord_chars[cursor]), 6, 10);
    cursor += 6;
    float coordinate = degrees + ((minutes + (.000001f * minutes_dec)) / 60.0f);
    return -coordinate;
}

uint64_t chars_to_number(const char *s, size_t len, uint8_t base) {
    uint64_t r = 0;
    for (size_t i = 0; i < len; i++) {
        r += (s[i] - '0') * ipow(base, ((len - 1) - i));
    }
    return r;
}
