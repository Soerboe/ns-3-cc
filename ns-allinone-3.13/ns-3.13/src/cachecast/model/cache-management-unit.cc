/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/log.h"
#include <iostream>
#include "cachecast-header.h"

#define CACHE_HIT 1
#define CACHE_MISS -1

// #include "ns3/simulator.h"
#include "ns3/uinteger.h"
#include "cache-management-unit.h"
NS_LOG_COMPONENT_DEFINE ("CacheManagementUnit");
using namespace std;

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CacheManagementUnit);
NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

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
  
<<<<<<< HEAD
  CacheCastHeader ccHrd;
  p->RemoveHeader (ccHrd);
  
  uint32_t location = searchPayloadID( ccHrd.GetPayloadId());
  uint32_t hashValue = ccHrd.GetPayloadSize() % 7;
  if ( location != 500 )        /* CACHE HIT */
  {
    ccHrd.SetIndex( location );
    /*
    if ( ccHrd.GetPayloadSize() == table[ location ].payloadSize )
    {
      //p->RemoveAtEnd( ccHrd.GetPayloadSize() );
      //ccHrd.SetPayloadSize( 0 );
    }*/
  }
  else        /* CACHE MISS */
  {
    table[ hashValue ].index = hashValue;
    table[ hashValue ].payloadID = ccHrd.GetPayloadId();
    table[ hashValue ].payloadSize = ccHrd.GetPayloadSize();
    ccHrd.SetIndex( table[ hashValue ].index );
  }
  p->AddHeader (ccHrd);
=======

  // THE HANDLING OF THE PACKET

>>>>>>> 4f71ed81737a9149a7bc8ca99fa8743d6df0db87
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

