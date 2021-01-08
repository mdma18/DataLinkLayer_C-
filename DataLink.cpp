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
void DataLink::Tick(void) {
// Collision detection is enabled by default
#if !(RFM12_NOCOLLISIONDETECTION)
  uint16_t uStatus;

  // Start with a channel free count of 16, this is necessary for the ASK receive feature to work
  static uint8_t nChannel = 16;  // Static local variables produce smaller code size than globals
#endif

// Debug
#if RFM12_UART_DEBUG
  static uint8_t uOldState;
  uint8_t uState = ctrl.rfm12_state;
  if (uOldState != uState) {
    uart_putstr("mode change: ");
    switch (uState) {
      case IDLE:
        uart_putc('i');
        break;
      case ACTIVE:
        uart_putc('r');
        break;
      case TX_STATE:
        uart_putc('t');
        break;
      default:
        uart_putc('?');
    }
    uart_putstr("\r\n");
    uOldState = uState;
  }
#endif

  // Don't disturb RFM12 if transmitting or receiving
  if (ctrl.rfm12_state != STATE_RX_IDLE) {
    return;
  }

//collision detection is enabled by default
#if !(RFM12_NOCOLLISIONDETECTION)
  //disable the interrupt (as we're working directly with the transceiver now)
  //hint: we could be losing an interrupt here, because we read the status register.
  //this applys for the Wakeup timer, as it's flag is reset by reading.
  RFM12_INT_OFF();
  status = rfm12_read(RFM12_CMD_STATUS);
  RFM12_INT_ON();

  //  Detect a carrier
  if (status & RFM12_STATUS_RSSI) {
    // Yes: reset counter and return
    nChannel = CHANNEL_FREE_TIME;
    return;
  }
  // No

  // Is the channel free long enough ?
  if (nChannel != 0) {
    // No:
    nChannel--;  // decrement counter
    return;
  }
  // Yes: we can begin transmitting
#endif

  // Do we have something to transmit?
  if (ctrl.txstate == STATUS_OCCUPIED) {
    rfm12_start_tx();
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
