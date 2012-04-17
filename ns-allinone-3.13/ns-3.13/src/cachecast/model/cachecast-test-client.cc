/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "ns3/cachecast-test-client.h"
#include "cachecast.h"
#include "ns3/packet.h"
#include "ns3/address.h"
#include "ns3/inet-socket-address.h"

NS_LOG_COMPONENT_DEFINE ("CacheCastTestClient");

using namespace ns3;

NS_OBJECT_ENSURE_REGISTERED (CacheCastTestClient);

TypeId
CacheCastTestClient::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CacheCastTestClient")
    .SetParent<Application> ()
    .AddConstructor<CacheCastTestClient> ()
  ;

  return tid;
}

CacheCastTestClient::CacheCastTestClient ()
  : m_socket (0),
    m_port (9)
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
CacheCastTestClient::SetPort (uint16_t port)
{
  m_port = port;
}

void
CacheCastTestClient::StartApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  if (m_socket == 0)
  {
    TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
    m_socket = Socket::CreateSocket (GetNode (), tid);
    InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (),
                                                  m_port);
    m_socket->Bind (local);
  }

  m_socket->SetRecvCallback (MakeCallback (&CacheCastTestClient::HandleRead, this));
}

void
CacheCastTestClient::StopApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  if (m_socket != 0)
  {
    m_socket->Close();
    m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
  }
}

void
CacheCastTestClient::HandleRead (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);
  Ptr<Packet> packet;
  Address from;
  while (packet = socket->RecvFrom (from))
    {
      NS_LOG_INFO ("CacheCast client: Received " << packet->GetSize () << " bytes from " <<
                    InetSocketAddress::ConvertFrom (from).GetIpv4 ());
    }
}

