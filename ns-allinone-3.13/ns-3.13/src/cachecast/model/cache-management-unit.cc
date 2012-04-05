/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
// #include "ns3/simulator.h"
#include "cache-management-unit.h"

NS_LOG_COMPONENT_DEFINE ("CacheManagementUnit");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CacheManagementUnit);

CacheManagementUnit::CacheManagementUnit ()
{
  NS_LOG_FUNCTION_NOARGS ();

}

bool
CacheManagementUnit::HandlePacket (Ptr<Packet> p)
{
  
  // THE HANDLING OF THE PACKET

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

