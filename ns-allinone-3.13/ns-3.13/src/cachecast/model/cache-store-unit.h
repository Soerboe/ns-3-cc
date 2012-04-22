/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __CACHE_STORE_UNIT_H__
#define __CACHE_STORE_UNIT_H__

#include <map>
#include "ns3/object.h"
#include "ns3/packet.h"
#include "cachecast-unit.h"
#include "cachecast-tag.h"

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

private:
  /* Size of the CSU's cache */
  uint32_t m_size;
  
  /* Size of the CSU cache's slots */
  uint32_t m_slotSize;

  struct TableItem
  {
    uint64_t id;
    uint32_t payloadSize;

    bool operator< (const TableItem &item) const
    {
      return id < item.id;
    }
  };

  std::map<uint32_t, TableItem> m_cache;

};

} // namespace ns3

#endif /* __CACHE_STORE_UNIT_H__ */
