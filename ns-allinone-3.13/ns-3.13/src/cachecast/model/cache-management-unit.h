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
 
 struct bucket
{
	uint32_t payloadSize;
	uint32_t payloadID;
	uint32_t index;
};

class CacheManagementUnit : public CacheCastUnit
{
public:

    bucket *table;
    CacheCastTag *tag_obj;	
	void configureTable();
	uint32_t searchPayloadID( uint32_t );
	
	
	
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

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

private:
  struct TableItem
  {
    uint32_t payloadId;
    double timeStamp;
    bool invalid;

    TableItem (uint32_t _payloadId, double _timeStamp, bool _invalid)
    {
      payloadId = _payloadId;
      timeStamp = _timeStamp;
      invalid = _invalid;
    }
  };

  /* Size of the CMU's table */
  uint32_t m_size;

  uint32_t m_currIndex;

  std::map<uint32_t, TableItem> m_tableIndexToItem;
  std::map<uint64_t, uint32_t> m_tableIdToIndex;
};

} // namespace ns3

#endif /* __CACHE_MANAGEMENT_UNIT_H__ */
