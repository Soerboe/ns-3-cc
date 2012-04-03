/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef CACHECAST_TAG_H
#define CACHECAST_TAG_H

#include "ns3/tag.h"

namespace ns3 {

class CacheCastTag : public Tag
{
public:
  /**
   * \brief Creates a new empty CacheCastTag
   * 
   * Payload ID and size must be set explicitly before tag is added to a packet
   * */
  CacheCastTag ();

  /** 
   * \brief Creates a new CacheCastTag and sets all necessary members
   *
   * \param payloadId the payload ID of the packet
   * \param payloadSize size of the payload the packet carries
   */
  CacheCastTag (uint32_t payloadId, uint32_t payloadSize);

  /**
   * \returns the size of the payload
   */
  uint32_t GetPayloadSize () const;

  /**
   * \brief Set the payload size
   * \param payloadSize the new payload size
   */
  void SetPayloadSize (uint32_t payloadSize);

  /**
   * \returns the payloadID
   */
  uint32_t GetPayloadId () const;

  /**
   * \brief Set the payload ID
   * \param payloadId the payload ID to set
   */
  void SetPayloadId (uint32_t payloadId);

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer buf) const;
  virtual void Deserialize (TagBuffer buf);
  virtual void Print (std::ostream &os) const;

private:
  /**
   * \brief The payload ID of the packet this tag is added to
   */
  uint32_t m_payloadId;
  /**
   * \brief Size of the payload of the packet
   */
  uint32_t m_payloadSize;
};

} // namespace ns3

#endif /* CACHECAST_TAG_H */
