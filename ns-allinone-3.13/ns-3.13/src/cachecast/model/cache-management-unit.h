/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __CACHE_MANAGEMENT_UNIT_H__
#define __CACHE_MANAGEMENT_UNIT_H__

#include "ns3/object.h"
#include "ns3/packet.h"
#include "cachecast-unit.h"
#include "cachecast-tag.h"
#include "cachecast-net-device.h"

namespace ns3 {

/**
 * /brief Cache Management Unit
 *
 */
 
 struct bucket
{
	uint32_t payloadID;
	uint32_t ipAddr;
	bool valid;
};

class CacheManagementUnit : public CacheCastUnit
{
public:

    bucket *table;
    CacheCastTag *tag_obj;	
	void configureTable();
	uint32_t searchPayloadID( uint32_t, uint32_t);
	uint32_t GenHashKey(uint32_t, uint32_t);
	
	
	
  /**
   * /brief Construct an empty CacheManagementUnit
   */
  CacheManagementUnit ();

  /** 
   * Documented in CacheCastUnit 
   */
  virtual bool HandlePacket (Ptr<Packet> p );

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
