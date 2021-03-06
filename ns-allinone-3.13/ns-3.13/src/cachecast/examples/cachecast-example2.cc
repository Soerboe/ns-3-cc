/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/cachecast-server-helper.h"
#include "ns3/cachecast-module.h"

using namespace ns3;

int 
main (int argc, char *argv[])
{
  LogComponentEnable ("CacheCastTestApplication", LOG_LEVEL_ALL);
  LogComponentEnable ("CacheCastNetDevice", LOG_LEVEL_INFO);
  LogComponentEnable ("CacheCastPid", LOG_LEVEL_ALL);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (2);

  CacheCastServerHelper ccHelper;
  ccHelper.SetDeviceAttribute ("DataRate", StringValue ("10Kbps"));
  ccHelper.SetChannelAttribute ("Delay", StringValue ("100ms"));
  NetDeviceContainer devices = ccHelper.Install (nodes);

  InternetStackHelper stack;
  stack.Install (nodes);
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  UdpEchoServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  Ptr<CacheCastTestApplication> app = Create<CacheCastTestApplication> ();
  Address addr (InetSocketAddress (interfaces.GetAddress (1), 9));
  app->AddAddress (addr);
  nodes.Get (0)->AddApplication (app);
  app->SetStartTime (Seconds (2.0));
  app->SetStopTime (Seconds (10.0));

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}


