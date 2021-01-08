#include "Headers/Frame.hpp"

//==============================================================================================
Frame::Frame(Pos framePos, int nSeq, std::string sSubPkt) : mHeader(HEADER),
                                                            mSeqN(nSeq),
                                                            mTrailer(TRAILER),
                                                            mSubPacket(ProcessSubPacket(sSubPkt)) {
  mControl = (framePos == Mid) ? FRAME_DATA : LAST_FRAME_IND;
}

//==============================================================================================
Frame::Frame(const uint8_t* uFrame) : mFrame(uFrame), bCheck(false) {
  if (Deframe())
    std::cout << "No errors.\n";
}
//==============================================================================================

//==============================================================================================
Frame::~Frame() {
}
//==============================================================================================

//==============================================================================================
std::bitset<MAX_PKT> Frame::ProcessSubPacket(std::string sSubPkt) {
  std::stringstream ssNet;
  zBYTE bTemp;
  int i(0);

  while ((sSubPkt.size() != 0) || ssNet.str().size() <= MAX_PKT) {
    bTemp = zBYTE(sSubPkt.substr(0, BYTE));
    if (bTemp == HEADER || bTemp == TRAILER || bTemp == ESCAPE) {
      ssNet << bTemp;
      ssNet << zBYTE(ESCAPE);
    } else {
      ssNet << bTemp;
    }
    if (sSubPkt.size() >= BYTE)
      sSubPkt = sSubPkt.substr(BYTE);
    else {
      break;
    }
    i++;
  }
  if (ssNet.str().size() > MAX_PKT && sSubPkt.size() != 0) {
    return std::bitset<MAX_PKT>(sSubPkt);
  }
  return std::bitset<MAX_PKT>(ssNet.str());
}
//==============================================================================================

//==============================================================================================
void Frame::Stringify() {
  std::stringstream ssTemp;
  ssTemp << mHeader << mControl << mSeqN << mSubPacket << mChecksum << mTrailer;
  std::string sTemp(ssTemp.str());
  std::cout << ssTemp.str() << std::endl; /* For debug */
  /* Frame converted into unsigned char pointer which will be sent through PHY*/
  mFrame = reinterpret_cast<const uint8_t*>(sTemp.c_str());
}
//==============================================================================================

//==============================================================================================
bool Frame::Deframe() {
  std::string sFrame((char*)mFrame);
  /* Remove Trailer */
  sFrame = sFrame.substr(sFrame.size() - BYTE);
  /* Get Checksum */
  mChecksum = zBYTE(sFrame.substr(sFrame.size() - BYTE));
  /* If no remainder from division, no errors detected, process frame. */
  if (mChecksum.to_ulong() % POLY == 0) {
    /* Remove Header */
    sFrame = sFrame.substr(BYTE);
    /* Get Control byte */
    mControl = zBYTE(sFrame.substr(0, BYTE));
    sFrame = sFrame.substr(BYTE);
    /* Get frame sequence */
    mSeqN = zBYTE(sFrame.substr(0, BYTE));
    sFrame = sFrame.substr(BYTE);
    /* Get NET Sub-packet */
    mSubPacket = std::bitset<MAX_PKT>(sFrame);
    return true;
  }
  return false;
}
//==============================================================================================

//==============================================================================================
std::bitset<BYTE> Frame::Checksum(uint8_t crc, const unsigned char* buf, size_t len) {
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

//==============================================================================================
int Frame::GetSequence() {
  return (int)mSeqN.to_ulong();
}
//==============================================================================================

//==============================================================================================
std::bitset<MAX_PKT> Frame::GetPacket() {
  return mSubPacket;
}
//==============================================================================================

//==============================================================================================
bool Frame::GetFlag() {
  return bCheck;
}
//==============================================================================================