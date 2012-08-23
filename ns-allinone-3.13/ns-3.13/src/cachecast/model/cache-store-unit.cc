/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include <iostream>
#include "ns3/log.h"
// #include "ns3/simulator.h"
#include "ns3/uinteger.h"
#include "cache-store-unit.h"
#include "cachecast-header.h"
#include <ns3/ipv4-header.h>
#define TABLE_SIZE 100

using namespace std;

NS_LOG_COMPONENT_DEFINE ("CacheStoreUnit");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CacheStoreUnit);

CacheStoreUnit::CacheStoreUnit ()
{
  NS_LOG_FUNCTION_NOARGS ();
  SetSize (TABLE_SIZE);
}

void CacheStoreUnit::configureTable()
{
	table = new bucket[m_size];
	for( uint32_t i =0; i < m_size; i++ )
	{
        table[i].valid = false;
        table[i].next = NULL;
    }
}

void
CacheStoreUnit::SetSize (uint32_t size)
{
  m_size = size;
  configureTable();
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
  CacheCastHeader ccHrd;
  p->RemoveHeader (ccHrd);
  
  uint32_t location = ccHrd.GetIndex();
  location = 0;
  if ( table[ location ].valid )
  {
    p->AddPaddingAtEnd( table[ location ].payloadSize );
  }
  else if( table[ location ].valid == false )
  {
    table[ location ].payloadSize = ccHrd.GetPayloadSize();
    table[location].valid = true;
    setPayloadInSlots( ccHrd.GetPayloadSize(), location );
  }
  return true;
}

void CacheStoreUnit::setPayloadInSlots( uint32_t s, uint32_t index )
{
  NS_LOG_FUNCTION (s << index);

  uint32_t size = s;
  for ( int i=size; i >= 0; i -= m_slotSize )
  {
      bucket *hold = new bucket();
      hold->payloadSize = m_slotSize;
      hold->next = NULL;
      bucket *temp = &table[index];
      while(temp->next)
          temp = temp->next;
      temp->next = hold;
  }
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

