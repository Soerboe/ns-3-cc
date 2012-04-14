/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "cachecast-header.h"

// NS_LOG_COMPONENT_DEFINE ("CacheCastHeader");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CacheCastHeader);

CacheCastHeader::CacheCastHeader ()
  : m_payloadId (0),
    m_payloadSize (0),
    m_index (0)
{
}

CacheCastHeader::CacheCastHeader (uint32_t payloadId, uint16_t payloadSize,
    uint32_t index)
  : m_payloadId (payloadId),
    m_payloadSize (payloadSize),
    m_index (index)
{
}

uint32_t
CacheCastHeader::GetPayloadId (void) const
{
  return m_payloadId;
}

uint16_t
CacheCastHeader::GetPayloadSize (void) const
{
  return m_payloadSize;
}

uint32_t
CacheCastHeader::GetIndex (void) const
{
  return m_index;
}

void
CacheCastHeader::SetPayloadId (uint32_t payloadId)
{
  m_payloadId = payloadId;
}

void
CacheCastHeader::SetPayloadSize (uint16_t payloadSize)
{
  m_payloadSize = payloadSize;
}

void
CacheCastHeader::SetIndex (uint32_t index)
{
  m_index = index;
}

TypeId
CacheCastHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CacheCastHeader")
    .SetParent<Header> ()
    .AddConstructor<CacheCastHeader> ();

  return tid;
}

TypeId
CacheCastHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void
CacheCastHeader::Print (std::ostream &os) const
{
  os << "payload ID=" << m_payloadId << " "
     << "payload size=" << m_payloadSize << " "
     << "index=" << m_index
  ;
}

uint32_t
CacheCastHeader::GetSerializedSize (void) const
{
  return sizeof (uint32_t) * 2 + sizeof(uint16_t);
}

void
CacheCastHeader::Serialize (Buffer::Iterator start) const
{
  start.WriteHtonU32 (m_payloadId);
  start.WriteHtonU16 (m_payloadSize);
  start.WriteHtonU32 (m_index);
}

uint32_t
CacheCastHeader::Deserialize (Buffer::Iterator start)
{
  m_payloadId = start.ReadNtohU32 ();
  m_payloadSize = start.ReadNtohU16 ();
  m_index = start.ReadNtohU32 ();
  return GetSerializedSize ();
}

} // namespace ns3

