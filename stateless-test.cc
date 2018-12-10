#include "ns3/log.h"
#include "ns3/abort.h"
#include "ns3/config.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv6-address.h"
#include "ns3/address-utils.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/socket.h"
#include "ns3/udp-socket.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/simple-net-device.h"
#include "ns3/simple-channel.h"
#include "ns3/test.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/uinteger.h"
#include "ns3/point-to-point-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/application-container.h"
#include "ns3/stateless-client-server-helper.h"
#include "ns3/csma-module.h"
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/header.h"
#include "stdlib.h"
#include <iostream>
#include <string>
using namespace ns3;
using namespace std;

class statelessClientServerTestCase : public TestCase
{
public:
	statelessClientServerTestCase();
	virtual ~statelessClientServerTestCase();

	virtual void DoRun (void);
};

statelessClientServerTestCase::statelessClientServerTestCase()
  : TestCase ("Test out stateless stack")
{
}

statelessClientServerTestCase::~statelessClientServerTestCase()
{
}

void statelessClientServerTestCase::DoRun(void)
{	
  NodeContainer n;
  n.Create (2);

  InternetStackHelper internet;
  internet.Install (n);

  // link the two nodes
  Ptr<SimpleNetDevice> txDev = CreateObject<SimpleNetDevice> ();
  Ptr<SimpleNetDevice> rxDev = CreateObject<SimpleNetDevice> ();
  n.Get (0)->AddDevice (txDev);
  n.Get (1)->AddDevice (rxDev);
  Ptr<SimpleChannel> channel1 = CreateObject<SimpleChannel> ();
  rxDev->SetChannel (channel1);
  txDev->SetChannel (channel1);
  NetDeviceContainer d;
  d.Add (txDev);
  d.Add (rxDev);

  Ipv4AddressHelper ipv4;

  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign (d);

  uint16_t port = 4000;
  statelessClientHelper client(i.GetAddress(0),i.GetAddress(1),port);
  ApplicationContainer apps = client.Install(n.Get(0));
  apps.Start(Seconds (1.0));
  apps.Stop (Seconds (10.0));
  
  statelessServerHelper server (i.GetAddress(1),i.GetAddress(0),port);
  apps = server.Install (n.Get(1));
  apps.Start(Seconds (2.0));
  apps.Stop (Seconds (10.0));

  Simulator::Run();
  Simulator::Destroy();
  }

  int main(){
    statelessClientServerTestCase test1;
    test1.DoRun();
    return 0;
  }