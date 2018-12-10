#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/stateless-client.h"
#include "ns3/seq-ts-header.h"
#include "ns3/host.h"
#include <cstdlib>
#include <cstdio>

namespace ns3 {
NS_LOG_COMPONENT_DEFINE ("statelessclient");

NS_OBJECT_ENSURE_REGISTERED (statelessclient);

TypeId
statelessclient::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::statelessclient")
    .SetParent<Application> ()
    .SetGroupName("Applications")
    .AddConstructor<statelessclient> ()
    .AddAttribute ("RemoteAddress",
                   "The destination Address of the outbound packets",
                   AddressValue (),
                   MakeAddressAccessor (&statelessclient::m_peerAddress),
                   MakeAddressChecker ())
    .AddAttribute ("RemotePort", "The destination port of the outbound packets",
                   UintegerValue (100),
                   MakeUintegerAccessor (&statelessclient::m_peerPort),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute("LocateAddress",
                  "The Locate Address of the outbound packets",
                  AddressValue(),
                  MakeAddressAccessor (&statelessclient::m_locaAddress),
                  MakeAddressChecker())
    // .AddAttribute ("PacketSize",
    //                "Size of packets generated. The minimum packet size is 12 bytes which is the size of the header carrying the sequence number and the time stamp.",
    //                UintegerValue (1024),
    //                MakeUintegerAccessor (&statelessclient::m_size),
    //                MakeUintegerChecker<uint32_t> (12,1500))
  ;
  return tid;
}
statelessclient :: statelessclient()
{
	NS_LOG_FUNCTION (this);
	m_socket = 0;
	m_sendEvent = EventId();
}

statelessclient ::~statelessclient()
{
	NS_LOG_FUNCTION (this);
}

void
statelessclient::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void
statelessclient::StartApplication (void)
{
	NS_LOG_FUNCTION(this);
	if (m_socket == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket = Socket::CreateSocket (GetNode (), tid);
      if (Ipv4Address::IsMatchingType(m_peerAddress) == true)
        {
          cout<<"stateless-client 80"<<endl;
          InetSocketAddress local= InetSocketAddress (Ipv4Address::ConvertFrom(m_locaAddress),1000);
          InetSocketAddress peer= InetSocketAddress (Ipv4Address::ConvertFrom(m_peerAddress), m_peerPort);
          m_socket->Bind (local);
          m_socket->Connect (peer);
        }
      else if (Ipv6Address::IsMatchingType(m_peerAddress) == true)
        {
          m_socket->Bind6 ();
          m_socket->Connect (Inet6SocketAddress (Ipv6Address::ConvertFrom(m_peerAddress), m_peerPort));
        }
      else if (InetSocketAddress::IsMatchingType (m_peerAddress) == true)
        {
          m_socket->Bind (m_locaAddress);
          m_socket->Connect (m_peerAddress);
        }
      else if (Inet6SocketAddress::IsMatchingType (m_peerAddress) == true)
        {
          m_socket->Bind6 ();
          m_socket->Connect (m_peerAddress);
        }
        else
        {
            NS_ASSERT_MSG (false, "Incompatible address type: " << m_peerAddress);
        }
	}
  
	m_socket->SetRecvCallback(MakeCallback(&statelessclient::YouRecv,this));
	m_socket->SetAllowBroadcast(true);
  // m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
  // m_socket->SetAllowBroadcast (true);

    // myHeader header;
    // header.flag = WQE;
    // header.lenth = 10;
    // header.sequence = 0;
    // header.src_addr = 123;
    // header.dst_addr = 456;
    // header.src_port = 0;
    // header.dst_port = 1;
    // Ptr<Packet> packet = Create<Packet>(0);
    // packet->AddHeader(header);
    
    // m_socket->Send(packet);
    // cout<<"send finished"<<endl;
}

void 
statelessclient::StopApplication(void)
{
	NS_LOG_FUNCTION(this);
	Simulator::Cancel(m_sendEvent);
}

void
statelessclient::YouRecv(Ptr<Socket> sock)
{
  cout<<"YouRecv"<<endl;
  Ptr<Packet> packet;
  while (packet = sock->Recv())
  {
    myHeader header;
    packet->PeekHeader(header);
    switch(header.flag)
    {
      case 0: cout<<"you recv a WQE!"<<endl;WQE_recv(packet, sock); break;
      //case 1: EA_recv(header);break;
      case 2: Data_recv(packet, sock);break;
      case 3: ACK_recv(packet);break;
      default: cout<<"recv error!"<<endl;
    }
  }
}


}//namespace ns3
