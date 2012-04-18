/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef CACHECAST_TEST_APPLICATION_H
#define CACHECAST_TEST_APPLICATION_H

#include "ns3/applications-module.h"
#include "ns3/socket.h"

namespace ns3 {

class CacheCastTestApplication : public Application
{
public:
  static TypeId GetTypeId (void);
  CacheCastTestApplication ();
  void AddAddress (Address address);
  void SetPacketSize (uint32_t packetSize);
private:
  void SocketFailed (uint32_t socketIndex);
  virtual void StartApplication (void);
  virtual void StopApplication (void);

  uint32_t m_packetSize;
  std::vector<Address> m_address;
};

} // namespace ns3

#endif /* CACHECAST_TEST_APPLICATION_H */
