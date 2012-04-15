/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
// #include "ns3/simulator.h"
#include "ns3/uinteger.h"
#include "cache-management-unit.h"

NS_LOG_COMPONENT_DEFINE ("CacheManagementUnit");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CacheManagementUnit);

CacheManagementUnit::CacheManagementUnit ()
{
  NS_LOG_FUNCTION_NOARGS ();

}

void
CacheManagementUnit::SetSize (uint32_t size)
{
  m_size = size;
}

bool
CacheManagementUnit::HandlePacket (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (p);

  NS_ASSERT_MSG (m_size > 0, "CMU's table size must be a positive integer");
  

  // THE HANDLING OF THE PACKET

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

