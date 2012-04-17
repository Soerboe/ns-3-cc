/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/abort.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/queue.h"
#include "ns3/config.h"
#include "ns3/packet.h"
#include "ns3/names.h"
// #include "ns3/mpi-interface.h"
// #include "ns3/mpi-receiver.h"
#include "ns3/trace-helper.h"

#include "cachecast-helper.h"
#include "ns3/cachecast-net-device.h"
#include "ns3/cachecast-channel.h"
#include "ns3/cache-management-unit.h"
#include "ns3/cache-store-unit.h"

NS_LOG_COMPONENT_DEFINE ("CacheCastHelper");

namespace ns3 {

CacheCastHelper::CacheCastHelper ()
{
  m_deviceFactory.SetTypeId ("ns3::CacheCastNetDevice");
  m_queueFactory.SetTypeId ("ns3::DropTailQueue");
  m_channelFactory.SetTypeId ("ns3::CacheCastChannel");
}

void 
CacheCastHelper::SetQueue (std::string type,
                              std::string n1, const AttributeValue &v1,
                              std::string n2, const AttributeValue &v2,
                              std::string n3, const AttributeValue &v3,
                              std::string n4, const AttributeValue &v4)
{
  m_queueFactory.SetTypeId (type);
  m_queueFactory.Set (n1, v1);
  m_queueFactory.Set (n2, v2);
  m_queueFactory.Set (n3, v3);
  m_queueFactory.Set (n4, v4);
}

void 
CacheCastHelper::SetDeviceAttribute (std::string n1, const AttributeValue &v1)
{
  m_deviceFactory.Set (n1, v1);
}

void 
CacheCastHelper::SetChannelAttribute (std::string n1, const AttributeValue &v1)
{
  m_channelFactory.Set (n1, v1);
}

void 
CacheCastHelper::EnablePcapInternal (std::string prefix, Ptr<NetDevice> nd, bool promiscuous, bool explicitFilename)
{
  //
  // All of the Pcap enable functions vector through here including the ones
  // that are wandering through all of devices on perhaps all of the nodes in
  // the system.  We can only deal with devices of type CacheCastNetDevice.
  //
  Ptr<CacheCastNetDevice> device = nd->GetObject<CacheCastNetDevice> ();
  if (device == 0)
    {
      NS_LOG_INFO ("CacheCastHelper::EnablePcapInternal(): Device " << device << " not of type ns3::CacheCastNetDevice");
      return;
    }

  PcapHelper pcapHelper;

  std::string filename;
  if (explicitFilename)
    {
      filename = prefix;
    }
  else
    {
      filename = pcapHelper.GetFilenameFromDevice (prefix, device);
    }

  Ptr<PcapFileWrapper> file = pcapHelper.CreateFile (filename, std::ios::out, 
                                                     PcapHelper::DLT_PPP);
  pcapHelper.HookDefaultSink<CacheCastNetDevice> (device, "PromiscSniffer", file);
}

void 
CacheCastHelper::EnableAsciiInternal (
  Ptr<OutputStreamWrapper> stream, 
  std::string prefix, 
  Ptr<NetDevice> nd,
  bool explicitFilename)
{
  //
  // All of the ascii enable functions vector through here including the ones
  // that are wandering through all of devices on perhaps all of the nodes in
  // the system.  We can only deal with devices of type CacheCastNetDevice.
  //
  Ptr<CacheCastNetDevice> device = nd->GetObject<CacheCastNetDevice> ();
  if (device == 0)
    {
      NS_LOG_INFO ("CacheCastHelper::EnableAsciiInternal(): Device " << device << 
                   " not of type ns3::CacheCastNetDevice");
      return;
    }

  //
  // Our default trace sinks are going to use packet printing, so we have to 
  // make sure that is turned on.
  //
  Packet::EnablePrinting ();

  //
  // If we are not provided an OutputStreamWrapper, we are expected to create 
  // one using the usual trace filename conventions and do a Hook*WithoutContext
  // since there will be one file per context and therefore the context would
  // be redundant.
  //
  if (stream == 0)
    {
      //
      // Set up an output stream object to deal with private ofstream copy 
      // constructor and lifetime issues.  Let the helper decide the actual
      // name of the file given the prefix.
      //
      AsciiTraceHelper asciiTraceHelper;

      std::string filename;
      if (explicitFilename)
        {
          filename = prefix;
        }
      else
        {
          filename = asciiTraceHelper.GetFilenameFromDevice (prefix, device);
        }

      Ptr<OutputStreamWrapper> theStream = asciiTraceHelper.CreateFileStream (filename);

      //
      // The MacRx trace source provides our "r" event.
      //
      asciiTraceHelper.HookDefaultReceiveSinkWithoutContext<CacheCastNetDevice> (device, "MacRx", theStream);

      //
      // The "+", '-', and 'd' events are driven by trace sources actually in the
      // transmit queue.
      //
      Ptr<Queue> queue = device->GetQueue ();
      asciiTraceHelper.HookDefaultEnqueueSinkWithoutContext<Queue> (queue, "Enqueue", theStream);
      asciiTraceHelper.HookDefaultDropSinkWithoutContext<Queue> (queue, "Drop", theStream);
      asciiTraceHelper.HookDefaultDequeueSinkWithoutContext<Queue> (queue, "Dequeue", theStream);

      // PhyRxDrop trace source for "d" event
      asciiTraceHelper.HookDefaultDropSinkWithoutContext<CacheCastNetDevice> (device, "PhyRxDrop", theStream);

      return;
    }

  //
  // If we are provided an OutputStreamWrapper, we are expected to use it, and
  // to providd a context.  We are free to come up with our own context if we
  // want, and use the AsciiTraceHelper Hook*WithContext functions, but for 
  // compatibility and simplicity, we just use Config::Connect and let it deal
  // with the context.
  //
  // Note that we are going to use the default trace sinks provided by the 
  // ascii trace helper.  There is actually no AsciiTraceHelper in sight here,
  // but the default trace sinks are actually publicly available static 
  // functions that are always there waiting for just such a case.
  //
  uint32_t nodeid = nd->GetNode ()->GetId ();
  uint32_t deviceid = nd->GetIfIndex ();
  std::ostringstream oss;

  oss << "/NodeList/" << nd->GetNode ()->GetId () << "/DeviceList/" << deviceid << "/$ns3::CacheCastNetDevice/MacRx";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultReceiveSinkWithContext, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::CacheCastNetDevice/TxQueue/Enqueue";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultEnqueueSinkWithContext, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::CacheCastNetDevice/TxQueue/Dequeue";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultDequeueSinkWithContext, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::CacheCastNetDevice/TxQueue/Drop";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultDropSinkWithContext, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::CacheCastNetDevice/PhyRxDrop";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultDropSinkWithContext, stream));
}

NetDeviceContainer
CacheCastHelper::Install (Ptr<Node> n1, Ptr<Node> n2)
{
  NS_ASSERT (n1 != 0);
  NS_ASSERT (n2 != 0);

  NetDeviceContainer container;

  /* Setup node 1 */
  Ptr<CacheCastNetDevice> dev1 = m_deviceFactory.Create<CacheCastNetDevice> ();
  Ptr<CacheManagementUnit> cmu = Create<CacheManagementUnit> ();
  
  // TODO Change
  cmu->SetSize(10);
  
  dev1->AddSenderUnit (cmu);
  Ptr<Queue> queue1 = m_queueFactory.Create<Queue> ();
  dev1->SetAddress (Mac48Address::Allocate ());
  dev1->SetQueue (queue1);
  n1->AddDevice (dev1);

  /* Setup node 2 */
  Ptr<CacheCastNetDevice> dev2 = m_deviceFactory.Create<CacheCastNetDevice> ();
  Ptr<CacheStoreUnit> csu = Create<CacheStoreUnit> ();
  
  // TODO Change these two
  csu->SetSize(10);
  // TODO 
  csu->SetSlotSize (1500);

  dev2->AddReceiverUnit (csu);
  Ptr<Queue> queue2 = m_queueFactory.Create<Queue> ();
  dev2->SetAddress (Mac48Address::Allocate ());
  dev2->SetQueue(queue2);
  n2->AddDevice (dev2);
  
  Ptr<CacheCastChannel> channel = m_channelFactory.Create<CacheCastChannel> ();
  dev1->Attach (channel);
  dev2->Attach (channel);
  container.Add (dev1);
  container.Add (dev2);

  return container;
}

NetDeviceContainer 
CacheCastHelper::Install (NodeContainer c)
{
  NS_ASSERT (c.GetN () == 2);
  return Install (c.Get (0), c.Get (1));
}

} // namespace ns3

