/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/ipv4-header.h"
#include "ns3/uinteger.h"

#include "cache-management-unit.h"
#include "cachecast-tag.h"
#include "cachecast-header.h"

NS_LOG_COMPONENT_DEFINE ("CacheManagementUnit");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CacheManagementUnit);

CacheManagementUnit::CacheManagementUnit ()
  : m_size (0),
    m_currIndex (0)
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
CacheManagementUnit::SetSize (uint32_t size)
{
  m_size = size;
}

void
CacheManagementUnit::SetSlotSize (uint32_t slotSize)
{
  m_slotSize = slotSize;
}

bool
CacheManagementUnit::HandlePacket (Ptr<Packet> p)
{
  NS_ASSERT_MSG (m_size > 0, "CMU's table size must be a positive integer");
  
  CacheCastTag tag;
  p->PeekPacketTag (tag);

  /* Check if there are enough slots */
  uint32_t slotsCount = tag.GetPayloadSize () / m_slotSize + 1;
  NS_ASSERT_MSG (slotsCount > m_size, "CacheCast packet is too large for the CSU");

  /* Get IP address of packet */
  Ipv4Header ipHdr;
  uint32_t ipRead = p->PeekHeader (ipHdr);
  NS_ASSERT (ipRead);
  uint32_t addr = ipHdr.GetSource ().Get ();

  /* Construct universally unique id */
  uint64_t id = ((uint64_t) addr << 32) | tag.GetPayloadId ();

  /* Search for id in table */
  std::map<uint64_t, uint32_t>::iterator it;
  it = m_tableIdToIndex.find (id);

  CacheCastHeader cch (tag.GetPayloadId (), tag.GetPayloadSize (), 0);

    // TODO handle wrapping, timeout

  /* Cache hit */
  if (it != m_tableIdToIndex.end ())
  {
    p->RemoveAtEnd (tag.GetPayloadSize ());
    cch.SetPayloadSize (0);
    cch.SetIndex ((*it).second);
  }
  /* Cache miss */
  else
  {
    cch.SetIndex (m_currIndex);

    for (int i = 0; i < slotsCount; i++)
    {
      TableItem &item = m_tableIndexToItem [m_currIndex]; //TODO

      if (item.idInSlot)
      {
        m_tableIndexToItem[m_currIndex].idInSlot = false;
        m_tableIdToIndex.erase (item.id);
      }
      
      m_currIndex = (m_currIndex + 1) % m_size;
    }
//TODO
//     TableItem item (id, Simulator::Now ().GetSeconds (), false);
//     m_tableIndexToItem[cch.GetIndex ()] = item; // remove already existing element
    m_tableIndexToItem[cch.GetIndex ()].id = id; // remove already existing element
//     m_tableIndexToItem.insert(item);
    m_tableIdToIndex[id] = cch.GetIndex ();

  }

  p->AddHeader (cch);

  return true;
}

TypeId
CacheManagementUnit::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CacheManagementUnit")
    .SetParent<CacheCastUnit> ()
    .AddConstructor<CacheManagementUnit> ()
    .AddAttribute ("Size",
                   "The number of elements in the table.",
                   UintegerValue (1000),
                   MakeUintegerAccessor (&CacheManagementUnit::m_size),
                   MakeUintegerChecker<uint32_t> ())
  ;
  return tid;
}

TypeId
CacheManagementUnit::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

} // namespace ns3

