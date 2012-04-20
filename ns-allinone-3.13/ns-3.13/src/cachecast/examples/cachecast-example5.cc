/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/cachecast-server-helper.h"
#include "ns3/cachecast-module.h"

using namespace ns3;

//             TOPOLOGY
//
//
//            n5       n6
//    10.1.5.2 \       / 10.1.6.2
//              \     / 
//               \   /
//       10.1.5.1 \ / 10.1.6.1
//                 n4
//        10.1.4.2 |
//                 |
//                 |
//        10.1.4.1 |
//                 n3
//        10.1.3.2 |
//                 |
//                 |
//        10.1.3.1 |
//                 n2
//       10.1.1.2 / \ 10.1.2.2
//               /   \
//              /     \
//    10.1.1.1 /       \ 10.1.2.1
//            n0        n1
//         (server)   (server)

#define PORT 9

static void
TraceCacheCast (Ptr<OutputStreamWrapper> stream, Ptr<const Packet> p)
{
  *stream->GetStream () << Simulator::Now ().GetSeconds () << "\t" 
    << p->GetSize () << "\n"; // << "\t" << p << std::endl;
}

int 
main (int argc, char *argv[])
{
  LogComponentEnable ("CacheCastTestApplication", LOG_LEVEL_ALL);
  LogComponentEnable ("CacheCastServerUnit", LOG_LEVEL_ALL);
  LogComponentEnable ("CacheCastPid", LOG_LEVEL_ALL);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("CacheCastNetDevice", LOG_LEVEL_INFO);
//   LogComponentEnable ("CacheStoreUnit", LOG_LEVEL_ALL);
//   LogComponentEnable ("CacheManagementUnit", LOG_LEVEL_ALL);
  LogComponentEnable ("CacheCastTestClient", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (7);

  /* Set up channel n0 <-> n2 */
  CacheCastServerHelper ccServerHelper;
  ccServerHelper.SetDeviceAttribute ("DataRate", StringValue ("2Mbps"));
  ccServerHelper.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer devices1 = ccServerHelper.Install (nodes.Get(0), nodes.Get(2));

  /* Set up channel n1 <-> n2 */
  ccServerHelper.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  ccServerHelper.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer devices2 = ccServerHelper.Install (nodes.Get(1), nodes.Get(2));

  /* Set up channel n2 <-> n3 */
  CacheCastHelper ccHelper;
  ccHelper.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  ccHelper.SetChannelAttribute ("Delay", StringValue ("5ms"));
  NetDeviceContainer devices3 = ccHelper.Install (nodes.Get(2), nodes.Get(3));

  /* Set up channel n3 <-> n4 */
  ccHelper.SetDeviceAttribute ("DataRate", StringValue ("3Mbps"));
  ccHelper.SetChannelAttribute ("Delay", StringValue ("4ms"));
  NetDeviceContainer devices4 = ccHelper.Install (nodes.Get(3), nodes.Get(4));

  /* Set up channel n4 <-> n5 */
  ccHelper.SetDeviceAttribute ("DataRate", StringValue ("2Mbps"));
  ccHelper.SetChannelAttribute ("Delay", StringValue ("6ms"));
  NetDeviceContainer devices5 = ccHelper.Install (nodes.Get(4), nodes.Get(5));

  /* Set up channel n4 <-> n6 */
  ccHelper.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  ccHelper.SetChannelAttribute ("Delay", StringValue ("7ms"));
  NetDeviceContainer devices6 = ccHelper.Install (nodes.Get(4), nodes.Get(6));

  /* Setup network stack */
  InternetStackHelper stack;
  stack.Install (nodes);

  /* Setup addresses */
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces1 = address.Assign (devices1);

  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces2 = address.Assign (devices2);

  address.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces3 = address.Assign (devices3);

  address.SetBase ("10.1.4.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces4 = address.Assign (devices4);

  address.SetBase ("10.1.5.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces5 = address.Assign (devices5);

  address.SetBase ("10.1.6.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces6 = address.Assign (devices6);

  /* Set up client applications */
  Ptr<CacheCastTestClient> client5 = Create<CacheCastTestClient> ();
  client5->SetPort (PORT);
  client5->SetStartTime (Seconds (1.0));
  client5->SetStopTime (Seconds (10.0));
  nodes.Get(5)->AddApplication (client5);

  Ptr<CacheCastTestClient> client6 = Create<CacheCastTestClient> ();
  client6->SetPort (PORT);
  client6->SetStartTime (Seconds (1.0));
  client6->SetStopTime (Seconds (10.0));
  nodes.Get(6)->AddApplication (client6);

  /* Set up server 0 application */
  Ptr<CacheCastTestApplication> app0 = Create<CacheCastTestApplication> ();
  Address addr5 (InetSocketAddress (interfaces5.GetAddress (1), PORT));
  Address addr6 (InetSocketAddress (interfaces6.GetAddress (1), PORT));
  app0->SetPacketSize (1000);
  app0->AddAddress (addr5);
  app0->AddAddress (addr5);
  app0->AddAddress (addr5);
//   app0->AddAddress (addr6);
//   app0->AddAddress (addr6);
  nodes.Get (0)->AddApplication (app0);
  app0->SetStartTime (Seconds (1.0));
  app0->SetStopTime (Seconds (10.0));

  /* Set up server 1 application */
//   Ptr<CacheCastTestApplication> app1 = Create<CacheCastTestApplication> ();
//   app1->SetPacketSize (600);
//   app1->AddAddress (addr5);
//   app1->AddAddress (addr6);
//   app1->AddAddress (addr6);
//   app1->AddAddress (addr6);
//   nodes.Get (1)->AddApplication (app1);
//   app1->SetStartTime (Seconds (1.0));
//   app1->SetStopTime (Seconds (10.0));

//   Packet::EnablePrinting();
//   Packet::EnableChecking();

  ccHelper.EnablePcapAll ("cc");
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  /* Node 2 */
  AsciiTraceHelper asciiTraceHelper;
  Ptr<OutputStreamWrapper> stream200 = asciiTraceHelper.CreateFileStream ("cachecast2-0-pre.txt");
  devices1.Get (1)->TraceConnectWithoutContext ("CcPreRecv", MakeBoundCallback (&TraceCacheCast, stream200));
  
  Ptr<OutputStreamWrapper> stream201 = asciiTraceHelper.CreateFileStream ("cachecast2-0-post.txt");
  devices1.Get (1)->TraceConnectWithoutContext ("CcPostRecv", MakeBoundCallback (&TraceCacheCast, stream201));

  Ptr<OutputStreamWrapper> stream210 = asciiTraceHelper.CreateFileStream ("cachecast2-1-pre.txt");
  devices2.Get (1)->TraceConnectWithoutContext ("CcPreRecv", MakeBoundCallback (&TraceCacheCast, stream210));

  Ptr<OutputStreamWrapper> stream211 = asciiTraceHelper.CreateFileStream ("cachecast2-1-post.txt");
  devices2.Get (1)->TraceConnectWithoutContext ("CcPostRecv", MakeBoundCallback (&TraceCacheCast, stream211));

  Ptr<OutputStreamWrapper> stream220 = asciiTraceHelper.CreateFileStream ("cachecast2-2-pre.txt");
  devices3.Get (0)->TraceConnectWithoutContext ("CcPreSend", MakeBoundCallback (&TraceCacheCast, stream220));

  Ptr<OutputStreamWrapper> stream221 = asciiTraceHelper.CreateFileStream ("cachecast2-2-post.txt");
  devices3.Get (0)->TraceConnectWithoutContext ("CcPostSend", MakeBoundCallback (&TraceCacheCast, stream221));

  /* Node 3 */
  Ptr<OutputStreamWrapper> stream300 = asciiTraceHelper.CreateFileStream ("cachecast3-0-pre.txt");
  devices3.Get (1)->TraceConnectWithoutContext ("CcPreRecv", MakeBoundCallback (&TraceCacheCast, stream300));

  Ptr<OutputStreamWrapper> stream301 = asciiTraceHelper.CreateFileStream ("cachecast3-0-post.txt");
  devices3.Get (1)->TraceConnectWithoutContext ("CcPostRecv", MakeBoundCallback (&TraceCacheCast, stream301));

  Ptr<OutputStreamWrapper> stream310 = asciiTraceHelper.CreateFileStream ("cachecast3-1-pre.txt");
  devices4.Get (0)->TraceConnectWithoutContext ("CcPreSend", MakeBoundCallback (&TraceCacheCast, stream310));

  Ptr<OutputStreamWrapper> stream311 = asciiTraceHelper.CreateFileStream ("cachecast3-1-post.txt");
  devices4.Get (0)->TraceConnectWithoutContext ("CcPostSend", MakeBoundCallback (&TraceCacheCast, stream311));

  /* Node 4 */
  Ptr<OutputStreamWrapper> stream400 = asciiTraceHelper.CreateFileStream ("cachecast4-0-pre.txt");
  devices4.Get (1)->TraceConnectWithoutContext ("CcPreRecv", MakeBoundCallback (&TraceCacheCast, stream400));
  
  Ptr<OutputStreamWrapper> stream401 = asciiTraceHelper.CreateFileStream ("cachecast4-0-post.txt");
  devices4.Get (1)->TraceConnectWithoutContext ("CcPostRecv", MakeBoundCallback (&TraceCacheCast, stream401));

  Ptr<OutputStreamWrapper> stream410 = asciiTraceHelper.CreateFileStream ("cachecast4-1-pre.txt");
  devices5.Get (0)->TraceConnectWithoutContext ("CcPreSend", MakeBoundCallback (&TraceCacheCast, stream410));

  Ptr<OutputStreamWrapper> stream411 = asciiTraceHelper.CreateFileStream ("cachecast4-1-post.txt");
  devices5.Get (0)->TraceConnectWithoutContext ("CcPostSend", MakeBoundCallback (&TraceCacheCast, stream411));

  Ptr<OutputStreamWrapper> stream420 = asciiTraceHelper.CreateFileStream ("cachecast4-2-pre.txt");
  devices6.Get (0)->TraceConnectWithoutContext ("CcPreSend", MakeBoundCallback (&TraceCacheCast, stream420));

  Ptr<OutputStreamWrapper> stream421 = asciiTraceHelper.CreateFileStream ("cachecast4-2-post.txt");
  devices6.Get (0)->TraceConnectWithoutContext ("CcPostSend", MakeBoundCallback (&TraceCacheCast, stream421));


  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}

