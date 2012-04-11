/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/cachecast-server-helper.h"
#include "ns3/cachecast-module.h"

using namespace ns3;

//              TOPOLOGY
//
//
//            n3        n4 
//   10.1.3.2 |         |  10.1.4.2
//            |         |
//            |         |
//   10.1.3.1 |         |  10.1.4.1
//            n1        n2
//   10.1.1.2  \        /  10.1.2.2
//              \      /
//               \    /
//       10.1.1.1 \  / 10.1.2.1
//                 n0
//              (server)

#define PORT 9

int 
main (int argc, char *argv[])
{
  LogComponentEnable ("CacheCastTestApplication", LOG_LEVEL_ALL);
  LogComponentEnable ("CacheCastServerUnit", LOG_LEVEL_ALL);
  LogComponentEnable ("CacheCastPid", LOG_LEVEL_ALL);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("CacheCastNetDevice", LOG_LEVEL_ALL);

  NodeContainer nodes;
  nodes.Create (5);

  /* Set up channel n0 <-> n1 */
  CacheCastServerHelper ccServerHelper;
  ccServerHelper.SetDeviceAttribute ("DataRate", StringValue ("2Mbps"));
  ccServerHelper.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer devices1 = ccServerHelper.Install (nodes.Get(0), nodes.Get(1));

  /* Set up channel n0 <-> n2 */
  ccServerHelper.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  ccServerHelper.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer devices2 = ccServerHelper.Install (nodes.Get(0), nodes.Get(2));

  /* Set up channel n1 <-> n3 */
  CacheCastHelper ccHelper;
  ccHelper.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  ccHelper.SetChannelAttribute ("Delay", StringValue ("5ms"));
  NetDeviceContainer devices3 = ccHelper.Install (nodes.Get(1), nodes.Get(3));

  /* Set up channel n2 <-> n4 */
  ccHelper.SetDeviceAttribute ("DataRate", StringValue ("3Mbps"));
  ccHelper.SetChannelAttribute ("Delay", StringValue ("4ms"));
  NetDeviceContainer devices4 = ccHelper.Install (nodes.Get(2), nodes.Get(4));

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

  UdpEchoServerHelper echoServer1 (PORT);
  ApplicationContainer serverApps1 = echoServer1.Install (nodes.Get (3));
  serverApps1.Start (Seconds (1.0));
  serverApps1.Stop (Seconds (10.0));

  UdpEchoServerHelper echoServer2 (PORT);
  ApplicationContainer serverApps2 = echoServer2.Install (nodes.Get (4));
  serverApps2.Start (Seconds (1.0));
  serverApps2.Stop (Seconds (10.0));


  Ptr<CacheCastTestApplication> app = Create<CacheCastTestApplication> ();
  Address addr1 (InetSocketAddress (interfaces3.GetAddress (1), PORT));
  Address addr2 (InetSocketAddress (interfaces4.GetAddress (1), PORT));
  app->AddAddress (addr1);
  app->AddAddress (addr2);
//   app->AddAddress (addr2);
//   app->AddAddress (addr1);
//   app->AddAddress (addr1);
//   app->AddAddress (addr1);
//   app->AddAddress (addr1);
  nodes.Get (0)->AddApplication (app);
  app->SetStartTime (Seconds (2.0));
  app->SetStopTime (Seconds (10.0));

  Packet::EnableChecking();

//   ccHelper.EnablePcapAll ("cc");
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}


