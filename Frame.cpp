#include "Headers/Frame.hpp"

//==============================================================================================
Frame::Frame(Pos framePos, int nSeq, std::string sSubPkt) : mHeader(HEADER),
                                                            //==============================================================================================
                                                            mSeqN(nSeq),
                                                            mTrailer(TRAILER),
                                                            mSubPacket(ProcessSubPacket(sSubPkt)) {
  mControl = (framePos == Mid) ? FRAME_DATA : LAST_FRAME_IND;
  Stringify();
}

Frame::~Frame() {
}

//==============================================================================================
std::bitset<MAX_PKT> Frame::ProcessSubPacket(std::string sSubPkt) {
  //==============================================================================================
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
  return std::bitset<MAX_PKT>(ssNet.str());
}

//==============================================================================================
void Frame::Stringify() {
  //==============================================================================================
  std::stringstream ssTemp;
  ssTemp << mHeader << mControl << mSeqN << mSubPacket << mChecksum << mTrailer;
  std::cout << ssTemp.str() << std::endl;
}
