/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __CACHECAST_SERVER_UNIT_H__
#define __CACHECAST_SERVER_UNIT_H__

#include "cachecast-unit.h"

#define TIMEOUT 1.0

namespace ns3 {

/**
 * /brief CacheCast Server unit
 *
 * Handles the sending of packets from the server
 */
class CacheCastServerUnit : public CacheCastUnit
{
public:
  /**
   * /brief Construct an empty CacheCastServerUnit
   */
  CacheCastServerUnit ();

  static TypeId GetTypeId (void);

  /**
   * Documented in CacheCastUnit
   */
  bool HandlePacket (Ptr<Packet> p);

private:
  uint32_t m_payloadId;
  double m_timeStamp;
  bool m_invalid;
};

} // namespace ns3

#endif /* __CACHECAST_SERVER_UNIT_H__ */
