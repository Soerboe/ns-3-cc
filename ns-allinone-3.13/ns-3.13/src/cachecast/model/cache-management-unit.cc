/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/log.h"
// #include <iostream>
#include "cachecast-header.h"

// #define CACHE_HIT 1
// #define CACHE_MISS -1

// #include "ns3/simulator.h"
#include "ns3/uinteger.h"
#include "cache-management-unit.h"

NS_LOG_COMPONENT_DEFINE ("CacheManagementUnit");
// using namespace std;

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CacheManagementUnit);
// NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

CacheManagementUnit::CacheManagementUnit ()
{
  NS_LOG_FUNCTION_NOARGS ();
  SetSize (100);
}

/* ------------------------------------------------------- */

void CacheManagementUnit::configureTable()
{
	table = new bucket[m_size];
	for( uint32_t i =0; i < m_size; i++ )
    table[i].payloadID = 10000;
}
/* ------------------------------------------------------- */

void
CacheManagementUnit::SetSize (uint32_t size)
{
  m_size = size;
  configureTable();
}

bool
CacheManagementUnit::HandlePacket (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (p);

  NS_ASSERT_MSG (m_size > 0, "CMU's table size must be a positive integer");
  
//   CacheCastHeader ccHrd ;
//   p->RemoveHeader (ccHrd);
  
  CacheCastTag cct;
  p->PeekPacketTag (cct);

  CacheCastHeader cch (cct.GetPayloadId (), cct.GetPayloadSize (), 0);

  uint32_t location = searchPayloadID( cct.GetPayloadId());
  uint32_t hashValue = cct.GetPayloadId() % 7;
  if ( location != 500 )        /* CACHE HIT */
  {
    cch.SetIndex( location );
    
    if ( cct.GetPayloadSize() == table[ location ].payloadSize )
    {
      std::cerr << "SZ: " << cct.GetPayloadSize () << "\t" << p->GetSize () << "\n";
      p->RemoveAtEnd( cct.GetPayloadSize() );
      cch.SetPayloadSize( 0 );
    }
  }
  else        /* CACHE MISS */
  {
    table[ hashValue ].index = hashValue;
    table[ hashValue ].payloadID = cct.GetPayloadId();
    table[ hashValue ].payloadSize = cct.GetPayloadSize();
    cch.SetIndex( table[ hashValue ].index );
  }

  p->AddHeader (cch);

  return true;
}

uint32_t CacheManagementUnit::searchPayloadID( uint32_t ID )
{
  for( uint32_t i =0; i < m_size; i++ )
  {
    if(table[i].payloadID == ID)
      return i;
  }
  return 500;
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

