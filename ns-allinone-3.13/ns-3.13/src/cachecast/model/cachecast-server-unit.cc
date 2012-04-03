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
  : m_cache (0, 0.0, true)
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

//   Ptr<Node> node = GetNode ();
//   Ptr<CacheCastPid> ccp = node->GetObject<CacheCastPid> ();
//   NS_ASSERT_MSG (ccp, "A CacheCast server must have a CacheCastPid object");


  CacheCastTag tag;
  bool hasTag = p->RemovePacketTag (tag);
  NS_ASSERT_MSG (hasTag, "No CacheCast packet tag");

  CacheCastHeader cch (tag.GetPayloadId (), tag.GetPayloadSize (), 0);

  if (Simulator::Now ().GetSeconds () - m_cache.timeStamp > 1.0)
  {
    NS_LOG_DEBUG ("CacheCast: Cache invalidated");
    m_cache.invalid = true;
    m_cache.timeStamp = Simulator::Now ().GetSeconds ();
  }

  if (m_cache.payloadId == tag.GetPayloadId () && m_cache.invalid == false)
  {
    // remove payload
    p->RemoveAtEnd (tag.GetPayloadSize ());
    cch.SetPayloadSize (0);
  }
  else
  {
    // new payload ID
    m_cache.payloadId = tag.GetPayloadId ();
    m_cache.invalid = false;
  }

  // TODO enable when CSU present
//     p->AddHeader (cch);





//   if (ccTag.IsLastPacket ())
//   {
//     NS_LOG_DEBUG ("Send() received the last CacheCast packet " << packet);
//     uint32_t payloadId = ccp->CalculateNewPayloadId ();
// 
//   }

  return true;
}

} // namespace ns3

