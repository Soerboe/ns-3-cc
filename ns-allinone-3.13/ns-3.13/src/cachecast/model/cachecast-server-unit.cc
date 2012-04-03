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
  : m_cache (-1);
{
  NS_LOG_FUNCTION_NOARGS ();
}

TypeId
CacheCastServerUnit::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CacheCastServerUnit")
    .SetParent<CacheCastElement> ()
    .AddConstructor<CacheCastServerUnit> ()
  ;
  return tid;
}

bool
CacheCastServerUnit::HandlePacket (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (p);
  bool retValue = true;

  Ptr<Node> node = GetNode ();
  Ptr<CacheCastPid> ccp = node->GetObject<CacheCastPid> ();
  NS_ASSERT_MSG (ccp, "A CacheCast server must have a CacheCastPid object");

  PacketInfo info(packet, dest, protocolNumber);
  m_ccQueue.push_back (info);

  CacheCastTag ccTag;
  packet->PeekPacketTag (ccTag);

  if (ccTag.IsLastPacket ())
  {
    NS_LOG_DEBUG ("Send() received the last CacheCast packet " << packet);
    uint32_t payloadId = ccp->CalculateNewPayloadId ();

    // Get all CacheCastServerUnit on node and issue a FinishSend ()
    for (uint32_t i = 0; i < node->GetNDevices (); i++)
    {
      Ptr<CacheCastServerUnit> ccDev = DynamicCast<CacheCastServerUnit> (node->GetDevice (i));

      if (ccDev != 0)
      {
        bool ret = ccDev->FinishSend (payloadId);
        
        if (ret == true)
          NS_LOG_DEBUG ("All CacheCast packets were sent from device " << ccDev);
        else
        {
          NS_LOG_DEBUG ("Not all CacheCast packets were sent from device " << ccDev << "due to an error");
          retValue = false;
        }
      }
    }
  }

  return retValue;
}

bool
CacheCastServerUnit::FinishSend (uint32_t payloadId)
{
  NS_LOG_FUNCTION_NOARGS();
  bool retValue = true;
  uint32_t count = 0;
  std::vector<PacketInfo>::iterator it;

  for (it = m_ccQueue.begin(); it < m_ccQueue.end(); it++)
  {
    Ptr<Packet> p = it->packet;
    CacheCastTag tag;
    p->RemovePacketTag(tag);

    NS_ASSERT_MSG (tag.GetPayloadSize () >= 0, "CacheCast: Illegal payload size "
        << tag.GetPayloadSize ());
    NS_ASSERT_MSG (tag.GetSocketIndex () >= 0, "CacheCast: Illegal socket index "
        << tag.GetSocketIndex ());

    CacheCastHeader cch (payloadId, tag.GetPayloadSize (), 0);

    /* Truncate all packets besides first packet */
    if (count > 0)
    {
      p->RemoveAtEnd (tag.GetPayloadSize ());
      cch.SetPayloadSize (0);
    }
    
    // TODO enable when CSU present
//     p->AddHeader (cch);

    NS_LOG_DEBUG ("CacheCast: Sending packet " << it->packet << " " <<
        it->dest << " " << it->protocolNumber);

    bool ret = CacheCastNetDevice::Send (it->packet, it->dest, it->protocolNumber);

    /* If the packet failed to be sent onto the link, add index to failed vector */
    if (ret == false)
    {
      NS_LOG_DEBUG ("CacheCast: Packet " << it->packet << " failed to be sent on device " << this);
      m_failedCallback (tag.GetSocketIndex ());
      retValue = false;
    }

    count++;
  }

  m_ccQueue.clear ();

  return retValue;
}

} // namespace ns3

