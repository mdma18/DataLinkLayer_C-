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
DataLink::DataLink(bool bFlag, const uint8_t* uReceived) : mReceived(uReceived) {
  Interface(bFlag);
}
//==============================================================================================

//==============================================================================================
DataLink::~DataLink() {
  mFramesMap.clear();  // Free up map.
}
//==============================================================================================

//==============================================================================================
void DataLink::Interface(bool bFlag) {
  /* NET transmit */
  if (!bFlag) {
    if (sTransmit.size() < MAX_FRAME) {
      // NETReceive(bool* packet) // Let NET Layer know how many more bytes can be sent
    }

    // Start building frame and sending.
    // If encapsulation leads to a single frame, build and send.
    if (mPacketSize <= MAX_PKT) {
      Frame* tFrame = new Frame(Last, 0, sTransmit);
    } else {
      for (int i = 0; i < sTransmit.size() != 0; i++) {
        if (sTransmit.size() < MAX_PKT) {
          Frame* tFrame = new Frame(Last, i, sTransmit);
          break;
        }
        Frame* tFrame = new Frame(Mid, i, sTransmit.substr(0, MAX_PKT));
        if (sTransmit.size() >= MAX_PKT) {
          sTransmit = sTransmit.substr(MAX_PKT);
        }
        mFramesMap[i] = tFrame;
      }
    }
    SendFrames();
  }
  /* PHY transmit */
  else if (bFlag) {
    // Call Frame class to handle received frame
    Frame* rFrame = new Frame(mReceived);
    if (rFrame->GetSequence() == 0) {
      /* If we're back to the first frame, it must be a different packet */
      SendPackets();
    }
    mFramesMap[rFrame->GetSequence()] = rFrame;
  }
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
void DataLink::SendFrames() {
  for (const auto& zFrame : mFramesMap) {
    zFrame.second->Stringify();
  }
}
//==============================================================================================

//==============================================================================================
void DataLink::SendPackets() {
  std::stringstream ssTemp;
  bool bFlag;  // Send Packet only if it's not corrupted.
  for (const auto& zFrame : mFramesMap) {
    bFlag = zFrame.second->GetFlag();
    if (bFlag)
      ssTemp << zFrame.second->GetPacket();
  }
  if (bFlag) {
    /* Convert to suitable bool array to send to NET */
    mReceived = reinterpret_cast<const uint8_t*>(ssTemp.str().c_str());
    // NETReceive(bool* packet) // Call NET to transmit received packet.
  }
}
//==============================================================================================

//==============================================================================================
int main() {
  bool ar[] = {0, 0, 0, 0, 0, 0, 1, 0,
               0, 0, 0, 0, 0, 0, 0, 0,
               0, 0, 0, 0, 0, 0, 0, 0,
               0, 0, 1, 0, 0, 0, 0, 1};
  // int num = bitArrayToInt32(ar,32);
  // printf("number = %d\n", num);
  bool* d = ar;
  //"1111000000100010000000000010000011110000111111110000000000000000001000010110010011001100"
  //0000001000000000000000000010000101100100
  std::string x, a, s = "11110000000000000000000000100001";
  DataLink NodeA(0, 32, d);
  //x = NodeA.getR();
  //DataLink NodeB(1, 32, a);
  return 0;
}
//==============================================================================================
