/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "cachecast-tag.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CacheCastTag);

CacheCastTag::CacheCastTag (uint32_t payloadId, uint32_t payloadSize)
  : Tag (),
    m_payloadId (payloadId),
    m_payloadSize (payloadSize)
{
}

CacheCastTag::CacheCastTag ()
  : Tag (),
    m_payloadId (-1),
    m_payloadSize (-1)
{
}

uint32_t
CacheCastTag::GetPayloadSize () const
{
  return m_payloadSize;
}

void
CacheCastTag::SetPayloadSize (uint32_t payloadSize)
{
  m_payloadSize = payloadSize;
}

uint32_t
CacheCastTag::GetPayloadId () const
{
  return m_payloadId;
}

void
CacheCastTag::SetPayloadId (uint32_t payloadId)
{
  m_payloadId = payloadId;
}

TypeId 
CacheCastTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CacheCastTag")
    .SetParent<Tag> ()
    .AddConstructor<CacheCastTag> ()
  ;
  return tid;
}

TypeId 
CacheCastTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t 
CacheCastTag::GetSerializedSize (void) const
{
  return sizeof (uint32_t) * 2;
}

void 
CacheCastTag::Serialize (TagBuffer buf) const
{
  buf.WriteU32 (m_payloadId);
  buf.WriteU32 (m_payloadSize);
}

void 
CacheCastTag::Deserialize (TagBuffer buf)
{
  m_payloadId = buf.ReadU32 ();
  m_payloadSize = buf.ReadU32 ();
}

void 
CacheCastTag::Print (std::ostream &os) const
{
  os << "CacheCastTag: Payload ID=" << m_payloadId << " "
     << "Payload size=" << m_payloadSize;
}

} // namespace ns3

