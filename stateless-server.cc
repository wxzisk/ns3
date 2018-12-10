#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/host.h"


#include "ns3/seq-ts-header.h"
#include "ns3/stateless-server.h"

namespace ns3{

NS_LOG_COMPONENT_DEFINE ("statelessserver");
NS_OBJECT_ENSURE_REGISTERED (statelessserver);

TypeId
statelessserver::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::statelessserver")
    .SetParent<Application> ()
    .SetGroupName("Applications")
    .AddConstructor<statelessserver> ()
    .AddAttribute ("Port",
                   "Port on which we listen for incoming packets.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&statelessserver::m_port),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("RemoteAddress",
                   "The destination Address of the outbound packets",
                   AddressValue (),
                   MakeAddressAccessor (&statelessserver::m_peerAddress),
                   MakeAddressChecker ())
    .AddAttribute("LocateAddress",
                  "The Locate Address of the outbound packets",
                  AddressValue(),
                  MakeAddressAccessor (&statelessserver::m_locaAddress),
                  MakeAddressChecker())
  ;
  return tid;
}

statelessserver::statelessserver()
{
	NS_LOG_FUNCTION (this);
}

statelessserver::~statelessserver()
{
	NS_LOG_FUNCTION (this);
}

void
statelessserver::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void
statelessserver::StartApplication (void)
{
	NS_LOG_FUNCTION (this);

	 if (m_socket == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket = Socket::CreateSocket (GetNode (), tid);
      // InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (),
      //                                              m_port);
      InetSocketAddress local= InetSocketAddress (Ipv4Address::ConvertFrom(m_locaAddress), m_port);
      InetSocketAddress peer = InetSocketAddress (Ipv4Address::ConvertFrom(m_peerAddress), 1000);
      if (Ipv4Address::IsMatchingType(m_peerAddress) == true)
        {
          cout<<"statelessserver 79"<<endl;
          m_socket->Bind (local);
          m_socket->Connect (peer);
        }
      
    }
   
    m_socket->SetRecvCallback (MakeCallback (&statelessserver::wuRecv, this));
    m_socket->SetAllowBroadcast (true);
    // Host A;
    // A.getptr(m_socket);
    // A.run(123,456,10);
    myHeader header;
    header.flag = WQE;
    header.lenth = 10;
    header.sequence = 0;
    header.src_addr = 123;
    header.dst_addr = 456;
    header.src_port = 0;
    header.dst_port = 1;
    Ptr<Packet> packet = Create<Packet>(0);
    packet->AddHeader(header);
    cout<<"send"<<endl;
    m_socket->Send(packet);
    cout<<"send finished"<<endl;
}

void
statelessserver::StopApplication ()
{
  NS_LOG_FUNCTION (this);

  if (m_socket != 0)
    {
      m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }
}


void
statelessserver::wuRecv(Ptr<Socket> sock)
{
  cout<<"wu"<<endl;
  Ptr<Packet> packet;
  while(packet = sock->Recv())
  {
    myHeader header;
    packet->PeekHeader(header);
    switch(header.flag)
    {
      case 3:cout<<"wu recv a GED"<<endl; GED_recv(header,sock);break;
      case 5:ACK_recv(packet);break;
      default:cout<<"recv error!"<<endl;
    }

  }
}


}//namespace ns3