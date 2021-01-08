/**
 * @file Frame.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-01-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef HEADERS_FRAME_HPP_
#define HEADERS_FRAME_HPP_

#include "Definitions.hpp"
class Frame {
 public:
  /**
  * @brief Construct a new Frame object
  * 
  * @param framePos Indicates whether this specific frame 
  * is a Middle/Last section in the collection of frames.
  * @param nSeq This frame's sequence number. 
  * @param sSubPacket The NET sub-packet to be transmitted. 
  */
  Frame(Pos framePos, int nSeq, std::string sSubPacket);
  /**
   * @brief Construct a new Frame object used by PHY
   * 
   * @param uFrame Frame in unsigned char array format.
   */
  Frame(const uint8_t* uFrame);
  /**
   * @brief Destroy the Frame object
   * 
   */
  virtual ~Frame();
  /**
   * @brief Process the Sub-packet, apply Escape/Delimiters as required. 
   * 
   * @param sSubPkt NET Sub-packet to be encapsulated within the frame.
   * @return std::bitset<MAX_PKT> Processed NET packet. Max 23 bytes long.
   */
  std::bitset<MAX_PKT> ProcessSubPacket(std::string sSubPkt);
  /**
   * @brief Construct a string object representing the complete frame.
   * 
   */
  void Stringify();
  /**
   * @brief Deframes a received frame and checks for errors. 
   * 
   * @return true if no errors.
   * @return false if errors found.
   */
  bool Deframe();
  /**
   * @brief Construct a Checksum to append in the frame.
   * 
   * @param crc Payload data
   * @param buf Buffer for operation
   * @param len Length of message
   * @return std::bitset<BYTE> A byte with the remainder of the operation.
   */
  std::bitset<BYTE> Checksum(uint8_t crc, const unsigned char* buf, size_t len);
  /**
   * @brief Get the Sequence member variable for frame indexing
   * 
   * @return int Sequence number which belongs to this frame.
   */
  int GetSequence();
  /**
   * @brief Get the Sub-Packet for the NET.
   * 
   * @return std::bitset<MAX_PKT> Deframed sub-packet.
   */
  std::bitset<MAX_PKT> GetPacket();
  /**
   * @brief Get the Flag attribute.
   * 
   * @return true Checksum passed.
   * @return false Checksum failed.
   */
  bool GetFlag();

 private:
  /* Defined to prevent copying */
  Frame(const Frame& other);
  Frame& operator=(const Frame& other);

  /* Attributes */
  /**
   * @brief Start of Frame indicator.
   * 
   */
  std::bitset<BYTE> mHeader;
  /**
   * @brief Frame acknowledgement number
   * 
   */
  std::bitset<BYTE> mControl;
  /**
   * @brief Frame sequence number
   * 
   */
  std::bitset<BYTE> mSeqN;
  /**
   * @brief NET packet, 1-23 bytes long
   * 
   */
  std::bitset<MAX_PKT> mSubPacket;
  /**
   * @brief CRC-6 Checksum
   * 
   */
  std::bitset<BYTE> mChecksum;
  /**
   * @brief End of frame indicator.
   * 
   */
  std::bitset<BYTE> mTrailer;
  /**
   * @brief Variable used for communication via rfm12b radio module
   * 
   */
  const uint8_t* mFrame;
  /**
   * @brief Used for Checksum 
   * 
   */
  bool bCheck;
};
#endif  // HEADERS_FRAME_HPP_