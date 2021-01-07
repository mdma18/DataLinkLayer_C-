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
  /* Logical Link Layer */
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
  DataLink(bool bFlag, std::string sFrame);
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
  void ErrorDetection(std::bitset<MAX_FRAME> mPacket);

  //void Unstuff();

  std::string ArrayToString(bool* vPacket);

  // void ErrorCtrl();
  // void FlowCtrl();

 private:
  /* Attributes */

  std::string sTransmit;
  zBYTE zRTA;
  /**
   * @brief Map container which stores frames indexed by sequence numbers.
   * 
   */
  std::map<int, Frame*> mFramesMap;
};

#endif  // HEADERS_DATALINK_HPP_
