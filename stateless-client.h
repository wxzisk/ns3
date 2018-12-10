#ifndef STATELESS_CLIENT_H
#define STATELESS_CLIENT_H

#include "ns3/application.h"
#include "ns3/event-id.h"

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
#include "ns3/host.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/csma-module.h"
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/header.h"
#include "stdlib.h"
#include <iostream>
#include <string>

namespace ns3{
	class Socket;
	class Packet;

	class statelessclient : public Application
	{
	public:
		static TypeId GetTypeId (void);
		statelessclient ();
		virtual ~statelessclient();
		void SetRemote (Address ip, uint16_t port);
		void SetRemote (Address addr);

	protected:
		virtual void DoDispose (void);
	private:
		virtual void StartApplication (void);
		virtual void StopApplication (void);
		void YouRecv(Ptr<Socket> sock);
		void Send( void );

		Ptr<Socket> m_socket;
		Address m_locaAddress;
		Address m_peerAddress;
		uint16_t m_peerPort;
		EventId m_sendEvent;
	};


}//namespace ns3

#endif