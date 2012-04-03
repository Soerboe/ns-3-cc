/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "cachecast-unit.h"

NS_LOG_COMPONENT_DEFINE ("CacheCastUnit");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CacheCastUnit);

TypeId
CacheCastUnit::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CacheCastUnit")
    .SetParent<Object> ()
  ;
  return tid;
}

TypeId
CacheCastUnit::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

} // namespace ns3

