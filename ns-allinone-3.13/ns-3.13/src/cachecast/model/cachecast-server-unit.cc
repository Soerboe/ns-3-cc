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
  : m_payloadId (0),
    m_timeStamp (0.0),
    m_invalid (true)
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
  bool hasTag = p->PeekPacketTag (tag);
//   bool hasTag = p->RemovePacketTag (tag);
  NS_ASSERT_MSG (hasTag, "No CacheCast packet tag");

  CacheCastHeader cch (tag.GetPayloadId (), tag.GetPayloadSize (), 0);

<<<<<<< HEAD
  if (Simulator::Now ().GetSeconds () - m_table.timeStamp > 100.0)
=======
  /* Invalidate the current payload ID after one second */
  if (Simulator::Now ().GetSeconds () - m_timeStamp > 1.0)
>>>>>>> 4f71ed81737a9149a7bc8ca99fa8743d6df0db87
  {
    NS_LOG_DEBUG ("CacheCast server table invalidated");
    m_invalid = true;
    m_timeStamp = Simulator::Now ().GetSeconds ();
  }

  if (m_payloadId == tag.GetPayloadId () && !m_invalid)
  {
    // remove payload
    p->RemoveAtEnd (tag.GetPayloadSize ());
    cch.SetPayloadSize (0);
  }
  else
  {
    // new payload ID
    m_payloadId = tag.GetPayloadId ();
    m_invalid = false;
  }

//   p->AddHeader (cch);

  return true;
}

} // namespace ns3

