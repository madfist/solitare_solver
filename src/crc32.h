#ifndef CRC32_HEADER
#define CRC32_HEADER

extern "C" uint32_t crc32c(uint32_t crc, const unsigned char *buf, size_t len);
extern "C" uint64_t crc64c(uint64_t crc, const unsigned char *buf, size_t len);

#endif