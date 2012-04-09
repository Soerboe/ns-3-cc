/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "cachecast.h"
#include "ns3/log.h"
#include "cachecast-tag.h"
#include "ns3/node.h"
#include "cachecast-pid.h"
// #include "cachecast-server-net-device.h"

NS_LOG_COMPONENT_DEFINE ("CacheCast");

namespace ns3 {
NS_OBJECT_ENSURE_REGISTERED (CacheCast);

CacheCast::CacheCast (void)
{
  Packet::EnablePrinting ();
}

CacheCast::Iterator 
CacheCast::Begin (void) const
{
  return m_sockets.begin ();
}

CacheCast::Iterator 
CacheCast::End (void) const
{
  return m_sockets.end ();
}

CacheCast::Iterator 
CacheCast::BeginFailedSockets (void) const
{
  return m_failed.begin ();
}

CacheCast::Iterator 
CacheCast::EndFailedSockets (void) const
{
  return m_failed.end ();
}

void 
CacheCast::AddSocket (Ptr<Socket> socket)
{
    m_sockets.push_back (socket);
}

void
CacheCast::RemoveSocket(Ptr<Socket> socket){
  std::vector<Ptr<Socket> >::iterator vItr = m_sockets.begin();
  while ( vItr != m_sockets.end() )
  {
    if ( (*vItr) == socket )
    {
      vItr = m_sockets.erase( vItr ); // Will return next valid iterator
      break;
    }
    else
      vItr++;
  }    
}

bool 
CacheCast::Msend (Ptr<Packet> packet)
{
  uint32_t socket_index = 0;
  bool successful = true;
  std::vector<Ptr <Socket> >::iterator socket;

  if (m_sockets.size() == 0)
    return true;

  Ptr<CacheCastPid> pid = m_sockets[0]->GetNode ()->GetObject<CacheCastPid> ();
  NS_ASSERT_MSG (pid, "A CacheCast server must have a CacheCastPid object");
  uint32_t payloadId = pid->CalculateNewPayloadId ();

  for(socket = m_sockets.begin(); socket != m_sockets.end(); ++socket)
  {        
    // if DCCP gets supported handle it also
    NS_ASSERT_MSG ((*socket)->GetSocketType () == Socket::NS3_SOCK_DGRAM, "CacheCast supports only UDP sockets");

    // TODO check if socket's NetDevice is a CacheCastNetDevice and only add tag if it is

//     Ptr<Packet> p = Copy<Packet> (packet); 
    Ptr<Packet> p = packet->Copy (); 

    CacheCastTag tag (payloadId, p->GetSize ());
    p->AddPacketTag (tag);        

    if((*socket)->Send(p) < 0)
    {
      successful = false;
      SetFailedSocket (socket_index);
    }
    socket_index++; 
  }

  return successful; 
}

void
CacheCast::Merge(CacheCast cc)
{
  std::vector<Ptr<Socket> >::iterator vItr = cc.m_sockets.begin();
  while ( vItr != cc.m_sockets.end() )
  {
    AddSocket((*vItr));
    vItr++;
  } 
}

void
CacheCast::SetFailedSocket (uint32_t i){
  m_failed.push_back (m_sockets[i]);
}

TypeId
CacheCast::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CacheCast")
    .SetParent<Object> ()
    .AddConstructor<CacheCast> ()
  ;
  return tid;
}

}//end namespace ns3
