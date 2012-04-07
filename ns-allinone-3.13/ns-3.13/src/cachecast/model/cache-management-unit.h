/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __CACHE_MANAGEMENT_UNIT_H__
#define __CACHE_MANAGEMENT_UNIT_H__

#include "ns3/object.h"
#include "ns3/packet.h"
#include "cachecast-unit.h"

namespace ns3 {

/**
 * /brief Cache Management Unit
 *
 */
class CacheManagementUnit : public CacheCastUnit
{
public:
  /**
   * /brief Construct an empty CacheManagementUnit
   */
  CacheManagementUnit ();

  /** 
   * Documented in CacheCastUnit 
   */
  virtual bool HandlePacket (Ptr<Packet> p);

  /**
   * Set the size of the CMU's table
   */
  void SetSize (uint32_t size);

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

private:
  /* Size of the CMU's table */
  uint32_t m_size;
};

} // namespace ns3

#endif /* __CACHE_MANAGEMENT_UNIT_H__ */
