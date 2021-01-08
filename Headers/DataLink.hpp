/**
 * @file DataLink.hpp
 * @author Amdadullah Md (you@domain.com)
 * @brief Data Link Driver code
 * @version 0.1
 * @date 2021-01-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef HEADERS_DATALINK_HPP_
#define HEADERS_DATALINK_HPP_

#include "Definitions.hpp"
#include "Frame.hpp"

class DataLink {
  /* Logical Link Sublayer */
 public:
  /**
   * @brief Construct a new Data Link object, called by NET
   * 
   * @param bFlag Flag to decide whether to send frames to PHY or NET
   * @param pktSize Size of Packet to transmit
   * @param vPacket Packet to transmit by NET
   */
  DataLink(bool bFlag, int pktSize, bool* vPacket);
  /**
   * @brief Construct a new Data Link object, called by PHY
   * 
   * @param bFlag Flag to decide whether to send frames to PHY or NET
   * @param sFrame Packet to transmit by PHY
   */
  DataLink(bool bFlag, const uint8_t* uReceived);
  /**
   * @brief Destroy the Data Link object
   * 
   */
  virtual ~DataLink();
  /**
   * @brief Control unit which provides services to process packets/frames for NET/PHY.
   * Builds Frames for PHY layer
   * Builds Packets for NET layer
   * 
   * @param bFlag Flag indication by NET/PHY
   */
  void Interface(bool bFlag);
  /**
   * @brief Once all necessary Frames have been built, transmit them randomly through PHY.  
   * 
   */
  void SendFrames();
  /**
   * @brief Method to send a full packet to the NET
   * 
   */
  void SendPackets();
  /**
   * @brief Converts a boolean array pointer to std::string
   * 
   * @param vPacket NET packet in boolean array format
   * @return std::string NET packet in string format used for processing.
   */
  std::string ArrayToString(bool* vPacket);

  /* PHY & MAC Sublayer */
  /**
   * @brief CSMA-CD Algorithm with transmission using the rfm12b library.
   * 
   */
  void Tick();

 private:
  /* Attributes */
  /**
   * @brief NET packet to be transmitted.
   * 
   */
  std::string sTransmit;
  /**
   * @brief Received encapsulated NET packet frame.
   * 
   */
  const uint8_t* mReceived;
  /**
   * @brief Size of NET packet to be transmitted.
   * 
   */
  int mPacketSize;
  /**
   * @brief Map container which stores frames indexed by sequence numbers.
   * 
   */
  std::map<int, Frame*> mFramesMap;
};

#endif  // HEADERS_DATALINK_HPP_
