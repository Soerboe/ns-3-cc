/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __CACHECAST_SERVER_UNIT_H__
#define __CACHECAST_SERVER_UNIT_H__

#include "cachecast-element.h"
// #include "ns3/queue.h"
// #include "ns3/callback.h"
// #include <vector>

namespace ns3 {

/**
 * /brief CacheCast Server unit
 *
 * Handles the sending of packets from the server
 */
class CacheCastServerUnit : public CacheCastElement
{
public:
  /**
   * /brief Construct an empty CacheCastServerNetDevice
   */
  CacheCastServerNetDevice ();

  static TypeId GetTypeId (void);

  bool HandlePacket (Ptr<Packet> p);


private:
  int32_t m_cache; 
};

} // namespace ns3

#endif /* __CACHECAST_SERVER_UNIT_H__ */
