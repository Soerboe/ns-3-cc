/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "ns3/cachecast-test-application.h"
#include "cachecast.h"
#include "ns3/packet.h"
#include "ns3/address.h"
#include "ns3/inet-socket-address.h"

NS_LOG_COMPONENT_DEFINE ("CacheCastTestApplication");

using namespace ns3;

NS_OBJECT_ENSURE_REGISTERED (CacheCastTestApplication);

TypeId
CacheCastTestApplication::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CacheCastTestApplication")
    .SetParent<Application> ()
    .AddConstructor<CacheCastTestApplication> ()
  ;

  return tid;
}

CacheCastTestApplication::CacheCastTestApplication ()
  : m_packetSize (1024)
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
CacheCastTestApplication::AddAddress (Address address)
{
  NS_LOG_FUNCTION (address);
  m_address.push_back (address);
}

void
CacheCastTestApplication::SetPacketSize (uint32_t packetSize)
{
  m_packetSize = packetSize;
}

void
CacheCastTestApplication::StartApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  CacheCast cc;
  std::vector<Address>::const_iterator it;

  for (it = m_address.begin(); it < m_address.end(); ++it) {
    Ptr<Socket> socket = Socket::CreateSocket (GetNode (), TypeId::LookupByName ("ns3::UdpSocketFactory"));
    socket->Bind();
    socket->Connect (*it);
    cc.AddSocket (socket);
  }
 
//   CacheCast cc2;
// 
//   for (it = m_address.begin(); it < m_address.end(); ++it) {
//     Ptr<Socket> socket = Socket::CreateSocket (GetNode (), TypeId::LookupByName ("ns3::UdpSocketFactory"));
//     socket->Bind();
//     socket->Connect (*it);
//     cc2.AddSocket (socket);
//   }
//    
//     cc.Merge(cc2);

  Ptr<Packet> packet = Create<Packet> (m_packetSize);

    if(!cc.Msend(packet)){
//         CacheCast::Iterator vItr = cc.BeginFailedSockets();
//         while ( vItr != cc.EndFailedSockets() )
//         {
//             cc.RemoveSocket( (*vItr) );
//             vItr++;
//         }    
    }

//   Ptr<Packet> p = Create<Packet> (1400);
//   cc2.Msend (p);


//   static int i = 0;
// 
//   if (i == 0) {
//     Time t (Seconds (2));
//     Simulator::Schedule (t, &CacheCastTestApplication::StartApplication, this);
//   }
//   i++;

}

void
CacheCastTestApplication::StopApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();
}
