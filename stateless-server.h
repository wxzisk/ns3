#ifndef STATELESS_SERVER_H
#define STATELESS_SERVER_H

#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv6-address.h"
#include "ns3/address-utils.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/socket.h"
#include "ns3/udp-socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/uinteger.h"

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/csma-module.h"
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/header.h"
#include "stdlib.h"

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/address.h"
#include "ns3/host.h"

#include <iostream>
#include <string>
using namespace ns3;
using namespace std;

namespace ns3{
class statelessserver : public Application
{
public:
	static TypeId  GetTypeId (void);
	statelessserver ();
	virtual ~statelessserver();

protected:
	virtual void DoDispose(void);
private:
	virtual void StartApplication (void);
	virtual void StopApplication (void);

	void wuRecv (Ptr<Socket> socket);
	Address m_locaAddress;
	Address m_peerAddress;
	uint16_t m_port;
	Ptr<Socket> m_socket;
};

}

#endif