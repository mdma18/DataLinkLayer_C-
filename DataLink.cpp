/* Copyright (c) 2021 */

#include "Headers/DataLink.hpp"

// Constructor: Called by NET
//==============================================================================================
DataLink::DataLink(bool bFlag, int pktSize, bool* vPacket) : mPacketSize(pktSize),
                                                             sTransmit(ArrayToString(vPacket)) {
  Interface(bFlag);
}
//==============================================================================================

// Constructor: Called by PHY
//==============================================================================================
DataLink::DataLink(bool bFlag, std::string sFrame) : sTransmit(sFrame) {
  Interface(bFlag);
}
//==============================================================================================

//==============================================================================================
void DataLink::Interface(bool bFlag) {
  if (!bFlag) {
    if (sTransmit.size() < MAX_FRAME) {
      // NETReceive(bool* packet) // Let NET Layer know how many more bytes can be sent
    }

    // Start building frame and sending.
    // If encapsulation leads to a single frame, build and send.
    if (mPacketSize <= MAX_PKT) {
      Frame* thisFrame = new Frame(Last, 0, sTransmit);
    } else {
      for (int i = 0; i < sTransmit.size() != 0; i++) {
        if (sTransmit.size() < MAX_PKT) {
          Frame* thisFrame = new Frame(Last, i, sTransmit);
          break;
        }
        Frame* thisFrame = new Frame(Mid, i, sTransmit.substr(0, MAX_PKT));
        if (sTransmit.size() >= MAX_PKT) {
          sTransmit = sTransmit.substr(MAX_PKT);
        }
        mFramesMap[i] = thisFrame;
      }
    }
    SendFrames();
  } else if (bFlag) {
    std::bitset<MAX_FRAME> mFrame(sTransmit);
    ErrorDetection(mFrame);
    //Unstuff();
    // NETReceive(bool* packet)
  } else {
  }
}
//==============================================================================================

//==============================================================================================
DataLink::~DataLink() {
}
//==============================================================================================

//==============================================================================================
std::string DataLink::ArrayToString(bool* vPacket) {
  std::ostringstream os;

  for (int i = 0; i < mPacketSize; i++) {
    os << *(vPacket + i);
  }
  return std::string(os.str());
}
//==============================================================================================

//==============================================================================================
std::string DataLink::getR() {
  return sTransmit;
}
//==============================================================================================

//==============================================================================================
void DataLink::SendFrames() {
  for (const auto& zFrame : mFramesMap) {
    zFrame.second->Stringify();
  }
}
//==============================================================================================

//==============================================================================================
void DataLink::ErrorDetection(std::bitset<MAX_FRAME> mPacket) {
  if (mPacket.to_ulong() % GEN)
    std::cout << "No Errors\n";
}
//==============================================================================================

/* CRC-32C (iSCSI) polynomial in reversed bit order. */
#define POLY 0x82f63b78

/* CRC-32 (Ethernet, ZIP, etc.) polynomial in reversed bit order. */
/* #define POLY 0xedb88320 */

uint32_t crc32c(uint32_t crc, const unsigned char* buf, size_t len) {
  int k;

  crc = ~crc;
  while (len--) {
    crc ^= *buf++;
    for (k = 0; k < 8; k++)
      crc = crc & 1 ? (crc >> 1) ^ POLY : crc >> 1;
  }
  return ~crc;
}

//==============================================================================================
int main() {
  bool ar[] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
  // int num = bitArrayToInt32(ar,32);
  // printf("number = %d\n", num);
  bool* d = ar;
  //"1111000000100010000000000010000011110000111111110000000000000000001000010110010011001100"
  //0000001000000000000000000010000101100100
  std::string x, a, s = "11110000000000000000000000100001";
  //a = "000000000000000000000000001000011111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";
  DataLink NodeA(0, 32, d);
  //x = NodeA.getR();
  //DataLink NodeB(1, 32, a);
  return 0;
}
//==============================================================================================
