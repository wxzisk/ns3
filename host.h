#ifndef _HOST_H_
#define _HOST_H_
#include "ns3/ptr.h"
#include "ns3/packet.h"
#include "ns3/header.h"

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


#include <iostream>
#include <string>
using namespace ns3;
using namespace std;

//flag=1 means write,flag=2 means read,flag=0 means it's going to be put into cq
struct wqe_node
{
	int len;
	int sequence;
	uint64_t source_address;
	uint64_t destination_address;
	uint32_t flag;
	uint32_t allcate;
	struct wqe_node *next;
};

struct cq_node
{
	uint32_t no;
	struct cq_node *next;
};

enum 
{	
	WQE,EA,Data,GED,CQ,ACK
};

class Host{
public:
	Host();
	~Host();
	wqe_node* head;
	wqe_node* end;
	cq_node* cq_head;
	cq_node* cq_end;
	bool checkcq();
	void generatecq(uint32_t number);
	void write(uint64_t address1,uint64_t address2,uint32_t length);
	void read(uint64_t address1,uint64_t address2,uint32_t length);
	wqe_node* gethead();
	wqe_node* getend();
	void getkind(bool k);
	void getptr(Ptr<Socket> s);
	void run(uint64_t address1,uint64_t address2,uint32_t length);
private:
	bool kind;
	uint32_t counter;
	Ptr<Socket> sock;
};

class myHeader : public Header
{
public:


  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual uint32_t GetSerializedSize (void) const;

  uint32_t flag;
  uint32_t lenth;
  uint32_t sequence;
  uint64_t src_addr;
  uint64_t dst_addr;
  uint32_t src_port;
  uint32_t dst_port;
  uint32_t allocate;

};


	void WQE_send(uint32_t xc_sequence,uint32_t xc_lenth,
		uint64_t xc_src_addr,uint64_t xc_dst_addr, uint32_t xc_src_port, uint32_t xc_dst_port,
		Ptr<Socket> sock);

	void EA_send(myHeader header, Ptr<Socket> sock);
	void Data_send(myHeader xc_header, uint32_t xc_size, Ptr<Socket> sock);
	void ACK_send(myHeader xc_header, Ptr<Socket> sock);
	void GED_send(myHeader xc_header, Ptr<Socket> sock, uint32_t MSS, uint32_t cwnd);
	void WQE_recv(Ptr<Packet> packet, Ptr<Socket> sock);
	void EA_recv(myHeader xc_header);
	void Data_recv(Ptr<Packet> packet, Ptr<Socket> sock);
	void ACK_recv(Ptr<Packet> packet);
	void GED_recv(myHeader header, Ptr<Socket> sock);

#endif