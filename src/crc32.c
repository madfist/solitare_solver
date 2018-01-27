// FROM: https://stackoverflow.com/questions/27939882/fast-crc-algorithm
#include <stddef.h>
#include <stdint.h>

/* CRC-32C (iSCSI) polynomial in reversed bit order. */
#define POLY32 0x82f63b78

/* CRC-32 (Ethernet, ZIP, etc.) polynomial in reversed bit order. */
/* #define POLY32 0xedb88320 */

/* CRC-64-ISO */
#define POLY64 0xD800000000000000

/* CRC-64-ECMA */
/* #define POLY64 0xC96C5795D7870F42 */

uint32_t crc32c(uint32_t crc, const unsigned char *buf, size_t len)
{
    int k;

    crc = ~crc;
    while (len--) {
        crc ^= *buf++;
        for (k = 0; k < 8; k++)
            crc = crc & 1 ? (crc >> 1) ^ POLY32 : crc >> 1;
    }
    return ~crc;
}

uint64_t crc64c(uint64_t crc, const unsigned char *buf, size_t len)
{
    int k;

    crc = ~crc;
    while (len--) {
        crc ^= *buf++;
        for (k = 0; k < 8; k++)
            crc = crc & 1 ? (crc >> 1) ^ POLY64 : crc >> 1;
    }
    return ~crc;
}