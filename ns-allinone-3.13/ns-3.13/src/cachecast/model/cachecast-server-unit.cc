/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "ns3/udp-header.h"
#include "ns3/simulator.h"
#include "ns3/cachecast-tag.h"
#include "cachecast-server-unit.h"
#include "cachecast-header.h"
#include "cachecast-pid.h"

NS_LOG_COMPONENT_DEFINE ("CacheCastServerUnit");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CacheCastServerUnit);

CacheCastServerUnit::CacheCastServerUnit ()
  : m_table (0, 0.0, true)
{
  NS_LOG_FUNCTION_NOARGS ();
}

TypeId
CacheCastServerUnit::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CacheCastServerUnit")
    .SetParent<CacheCastUnit> ()
    .AddConstructor<CacheCastServerUnit> ()
  ;
  return tid;
}

bool
CacheCastServerUnit::HandlePacket (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (p);

  CacheCastTag tag;
  bool hasTag = p->RemovePacketTag (tag);
  NS_ASSERT_MSG (hasTag, "No CacheCast packet tag");

  CacheCastHeader cch (tag.GetPayloadId (), tag.GetPayloadSize (), 0);

  if (Simulator::Now ().GetSeconds () - m_table.timeStamp > 100.0)
  {
    NS_LOG_DEBUG ("CacheCast server table invalidated");
    m_table.invalid = true;
    m_table.timeStamp = Simulator::Now ().GetSeconds ();
  }

  if (m_table.payloadId == tag.GetPayloadId () && !m_table.invalid)
  {
    // remove payload
    p->RemoveAtEnd (tag.GetPayloadSize ());
    cch.SetPayloadSize (0);
  }
  else
  {
    // new payload ID
    m_table.payloadId = tag.GetPayloadId ();
    m_table.invalid = false;
  }

  p->AddHeader (cch);

  return true;
}

} // namespace ns3

