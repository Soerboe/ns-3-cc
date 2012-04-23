/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __CACHE_STORE_UNIT_H__
#define __CACHE_STORE_UNIT_H__

#include "ns3/object.h"
#include "ns3/packet.h"
#include "cachecast-unit.h"
#include "cachecast-tag.h"

//#include "cacheImplementation.h"

struct bucket
{
	uint32_t payloadSize;
	uint32_t payloadID;
	uint32_t index;
	bool valid;
	
	bucket *next;
};

namespace ns3 {

/**
 * /brief Cache Store Unit
 *
 */
 

class CacheStoreUnit : public CacheCastUnit
{
public:
  /**
   * /brief Construct an empty CacheStoreUnit
   */
    
    CacheStoreUnit ();
    uint32_t getHashKey(uint32_t);
    bucket *table;
    CacheCastTag *tag_obj;
    void configureTable();
    
  /** 
   * Documented in CacheCastUnit 
   */
  virtual bool HandlePacket (Ptr<Packet> p);

  /**
   * Set the size of the CSU's cache
   */
  void SetSize (uint32_t size);

  /**
   * Set the size of the CSU cache's slots
   */
  void SetSlotSize (uint32_t slotSize);

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  
  uint32_t m_size;

private:
  /* Size of the CSU's cache */
  
  /* Size of the CSU cache's slots */
  void setPayloadInSlots( uint32_t , uint32_t);
  uint32_t m_slotSize;
};

} // namespace ns3

#endif /* __CACHE_STORE_UNIT_H__ */
