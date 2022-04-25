/**
 * CRC
 * @file crc.hpp
 * @author Adam Koleszar
 */
#ifndef CRC32_HEADER
#define CRC32_HEADER

/// **C** implementation of CRC32
extern "C" uint32_t crc32c(uint32_t crc, const unsigned char *buf, size_t len);
/// **C** implementation of CRC64
extern "C" uint64_t crc64c(uint64_t crc, const unsigned char *buf, size_t len);

/**
 * @brief Convenience method that uses CRC64 if system allows
 * @param crc starting value
 * @param buf buffer to calculate checksum for
 * @param len size of buffer
 * return calculated checksum
 */
std::size_t crc(std::size_t crc, const unsigned char *buf, std::size_t len);

#endif