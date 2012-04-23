/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/log.h"
#include <iostream>
#include "cachecast-header.h"
#define TABLE_SIZE 100
//#include "ns3/simulator.h"

#include <ns3/ipv4-header.h>

#include "ns3/uinteger.h"
#include "cache-management-unit.h"
#include "cachecast-net-device.h"

NS_LOG_COMPONENT_DEFINE ("CacheManagementUnit");
using namespace std;

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CacheManagementUnit);
//NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

bool first = true;
CacheManagementUnit::CacheManagementUnit ()
{
  NS_LOG_FUNCTION_NOARGS ();
  SetSize (TABLE_SIZE);
}

void CacheManagementUnit::configureTable()
{
	table = new bucket[m_size];
	for( uint32_t i =0; i < m_size; i++ )
    table[i].valid = false;
}

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

  CacheCastHeader ccHrd ;
  p->RemoveHeader (ccHrd);
  
  Ipv4Header ipHdr;
  p->PeekHeader (ipHdr);
  uint32_t ipAddr = ipHdr.GetSource ().Get ();
  uint32_t hashKey = searchPayloadID(ccHrd.GetPayloadId(), ipAddr);
  
  if ( hashKey != 500 )
  {
    ccHrd.SetIndex( hashKey );
  }
  else
  {
  
    hashKey = GenHashKey(ccHrd.GetPayloadId(), ipAddr);
    table[ hashKey ].payloadID = ccHrd.GetPayloadId();
    table[ hashKey ].ipAddr = ipAddr;
    table[ hashKey ].valid = true;
    ccHrd.SetIndex( hashKey );
  }
  /*cout <<"\n----------------------------------------------------\n";
  cout << "IP IS: " <<ipAddr << "  :::\n\n";
  for(uint32_t i=0; i<m_size; i++)
  {
    if(table[i].ipAddr != 0)
    cout << table[i].ipAddr << " ";
  }
  cout <<"\n----------------------------------------------------\n";
  */
  p->AddHeader (ccHrd);
  return true;
}

uint32_t CacheManagementUnit::GenHashKey(uint32_t ID, uint32_t ipAddr)
{
  uint32_t result = (ID + ipAddr) % m_size;
  return result;
}

uint32_t CacheManagementUnit::searchPayloadID( uint32_t ID, uint32_t ipAddr)
{
  for( uint32_t i =0; i < m_size; i++ )
  {
    if ( table[i].valid == true )
    {
      if((table[i].payloadID == ID) && (table[i].ipAddr == ipAddr))
        return i;
    }
  }
  return 500;               // This value will not occoured in case of CACHE_HIT
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

