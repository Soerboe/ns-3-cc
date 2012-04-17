/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef __CACHE_MANAGEMENT_UNIT_H__
#define __CACHE_MANAGEMENT_UNIT_H__

#include <map>
#include "ns3/object.h"
#include "ns3/packet.h"
#include "cachecast-unit.h"
#include "cachecast-tag.h"

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
//   ~CacheManagementUnit ();

  /** 
   * Documented in CacheCastUnit 
   */
  virtual bool HandlePacket (Ptr<Packet> p);

  /**
   * Set the size of the CMU's table
   */
  void SetSize (uint32_t size);

  /**
   * Set the size of the CSU cache's slots
   */
  void SetSlotSize (uint32_t slotSize);

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

private:
  struct TableItem
  {
    uint64_t id;
    double timeStamp;
    bool invalid;
    bool idInSlot;

    TableItem () {
//TODO 
    }

    TableItem (uint64_t _id, double _timeStamp, bool _invalid)
    {
      id = _id;
      timeStamp = _timeStamp;
      invalid = _invalid;
      idInSlot = true;
    }

    bool operator<(const TableItem &item) const
    {
      return id < item.id;
    }

  };

  /* Size of the CMU's table */
  uint32_t m_size;

  /* Size of the slots in the CSU */
  uint32_t m_slotSize;

  uint32_t m_currIndex;

  std::map<uint32_t, TableItem> m_tableIndexToItem;
  std::map<uint64_t, uint32_t> m_tableIdToIndex;
};

} // namespace ns3

#endif /* __CACHE_MANAGEMENT_UNIT_H__ */
