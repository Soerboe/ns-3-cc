/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/ipv4-header.h"

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

// CacheManagementUnit::~CacheManagementUnit ()
// {
//   NS_LOG_FUNCTION_NOARGS ();
// 
// }

void
CacheManagementUnit::SetSize (uint32_t size)
{
  m_size = size;
}

bool
CacheManagementUnit::HandlePacket (Ptr<Packet> p)
{
//   NS_ASSERT_MSG (m_size > 0, "CMU's table size must be a positive integer");
  
  CacheCastTag tag;
  p->RemovePacketTag (tag);

  /* Get IP address of packet */
  Ipv4Header ipHdr;
  uint32_t ipRead = p->PeekHeader (ipHdr);
  // Handle IPv6?
  NS_ASSERT (ipRead);
  uint32_t addr = ipHdr.GetSource ().Get ();

  /* Construct universally unique id */
  uint64_t id = ((uint64_t) addr << 32) | tag.GetPayloadId ();

  /* Search for id in table */
  std::map<uint64_t, uint32_t>::iterator it;
  it = m_tableIdToIndex.find (id);

  CacheCastHeader cch (tag.GetPayloadId (), tag.GetPayloadSize (), 0);

  /* Cache hit */
  if (it != m_tableIdToIndex.end ())
  {
    // TODO handle wrapping, timeout

    p->RemoveAtEnd (tag.GetPayloadSize ());
    cch.SetPayloadSize (0);
    cch.SetIndex ((*it).second);
  }
  /* Cache miss */
  else
  {
    TableItem item (tag.GetPayloadId (), Simulator::Now ().GetSeconds (), false);
//     m_tableIndexToItem.erase (m_currIndex); // remove already existing element
    m_tableIndexToItem.insert(item);
    m_tableIdToIndex[id] = m_currIndex;
    cch.SetIndex (m_currIndex);
    m_currIndex = (m_currIndex + 1) % m_size;
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
  ;
  return tid;
}

TypeId
CacheManagementUnit::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

} // namespace ns3

