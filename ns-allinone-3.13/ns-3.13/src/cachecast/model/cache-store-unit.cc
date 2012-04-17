/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/log.h"
// #include "ns3/simulator.h"
#include "ns3/uinteger.h"
#include "cache-store-unit.h"
#include "cachecast-header.h"

NS_LOG_COMPONENT_DEFINE ("CacheStoreUnit");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CacheStoreUnit);

CacheStoreUnit::CacheStoreUnit ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
CacheStoreUnit::SetSize (uint32_t size)
{
  m_size = size;
}

void
CacheStoreUnit::SetSlotSize (uint32_t slotSize)
{
  m_slotSize = slotSize;
}

bool
CacheStoreUnit::HandlePacket (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (p);
  CacheCastHeader cch;
  p->RemoveHeader(cch);  
  
  cch->GetIndex();
  cch->GetPayloadSize();
  
  
  if( cch->GetPayloadSize()>m_size)
    return false;

   /* Get IP address of packet */
  Ipv4Header ipHdr;
  uint32_t ipRead = p->PeekHeader (ipHdr);
  // Handle IPv6?
  NS_ASSERT (ipRead);
  uint32_t addr = ipHdr.GetSource ().Get ();

  /* Construct universally unique id */
  uint64_t id = ((uint64_t) addr << 32) | cch->GetPayloadId();

  TableItem item ( cch->GetPayloadId(), cch->GetPayloadSize(), false);
  m_tableIndexToItem.insert(item);

  if(cch->GetPayloadSize() == 0)
  {
     
  }else
  {
  
  }
  
  /*
  CacheCastTag cct
  p->AddPacketTag(cct);   
  */
  return true;
}

TypeId
CacheStoreUnit::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CacheStoreUnit")
    .SetParent<CacheCastUnit> ()
    .AddConstructor<CacheStoreUnit> ()
    .AddAttribute ("Size",
                   "The number of slots in the cache.",
                   UintegerValue (1000),
                   MakeUintegerAccessor (&CacheStoreUnit::m_size),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("SlotSize",
                   "The size of each slot in the cache.",
                   UintegerValue (1024),
                   MakeUintegerAccessor (&CacheStoreUnit::m_slotSize),
                   MakeUintegerChecker<uint32_t> ())
  ;
  return tid;
}

TypeId
CacheStoreUnit::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

} // namespace ns3

