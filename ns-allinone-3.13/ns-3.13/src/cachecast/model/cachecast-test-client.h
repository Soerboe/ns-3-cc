/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef CACHECAST_TEST_CLIENT_H
#define CACHECAST_TEST_CLIENT_H

#include "ns3/applications-module.h"
#include "ns3/socket.h"

namespace ns3 {

class CacheCastTestClient : public Application
{
public:
  static TypeId GetTypeId (void);
  CacheCastTestClient ();
  void SetPort (uint16_t port);
private:
  virtual void StartApplication (void);
  virtual void StopApplication (void);
  void HandleRead (Ptr<Socket> socket);

  Ptr<Socket> m_socket;
  uint16_t m_port;
};

} // namespace ns3

#endif /* CACHECAST_TEST_CLIENT_H */
