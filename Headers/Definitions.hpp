/**
 * @file Definitions.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-01-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef HEADERS_DEFINITIONS_HPP_
#define HEADERS_DEFINITIONS_HPP_

/* Standard Libraries */
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <bitset>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

constexpr auto NET_PACKET = 1024;      // NET Packet size.
constexpr auto MAX_FRAME = 256;        // Max Frame size.
constexpr auto MAX_PKT = 184;          // Max NET Packet size per frame. Max 23 bytes long, 184-bits partial NET packet
constexpr auto GEN = 0x47;             // CRC-6-CDMA-2000B (can detect all double bit errors and not vulnerable to systematic errors) G(x) x^6 + x^2 + x +1 (0x47)
constexpr auto HEADER = 0xF0;          // Indicates the start of the DLL frame.
constexpr auto TRAILER = 0xCC;         // Indicates the end of the DLL frame.
constexpr auto ESCAPE = 0xFF;          // Escapes the next byte to avoid frame synchronisation issues.
constexpr auto FRAME_DATA = 0x11;      // DLL frame contains a part of the encapsulated NET packet.
constexpr auto LAST_FRAME_IND = 0x22;  // DLL frame contains the last part of the encapsulated NET packet
constexpr auto POS_ACK = 0x44;         // Control byte for dummy DLL frame which acknowledges the frame whose sequence number is specified in the NET packet part. See Table 7.
constexpr auto NEG_ACK = 0x88;         // Control byte for dummy DLL frame which rejects the current frame.
constexpr auto BYTE = 8;               // Byte size.
constexpr auto MAX_FRAMES = 6;         // Max frames in buffer.
constexpr auto POLY = 0x23;            // CRC-6-CDMA2000-B polynomial.

enum Pos {
  Mid,
  Last
};

typedef std::bitset<8> zBYTE;  // Byte varaible, 8-bits.

#endif  // HEADERS_DEFINITIONS_HPP_
