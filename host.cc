#include "host.h"


TypeId
myHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MyHeader")
    .SetParent<Header> ()
    .AddConstructor<myHeader> ()
  ;
  return tid;
}
TypeId
myHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void
myHeader::Print (std::ostream &os) const
{
  // This method is invoked by the packet printing
  // routines to print the content of my header.
  //os << "data=" << m_data << std::endl;
  // os << "data=" << m_data<<std::endl;
}
uint32_t
myHeader::GetSerializedSize (void) const
{
  // we reserve 2 bytes for our header.
  return 40;
}
void
myHeader::Serialize (Buffer::Iterator start) const
{
  // we can serialize two bytes at the start of the buffer.
  // we write them in network byte order.
  start.WriteHtonU32 (flag);
  start.WriteHtonU32 (lenth);
  start.WriteHtonU32 (sequence);
  start.WriteHtonU64 (src_addr);
  start.WriteHtonU64 (dst_addr);
  start.WriteHtonU32 (src_port);
  start.WriteHtonU32 (dst_port);
  start.WriteHtonU32 (allocate);
}
uint32_t
myHeader::Deserialize (Buffer::Iterator start)
{
  // we can deserialize two bytes from the start of the buffer.
  // we read them in network byte order and store them
  // in host byte order.
  flag = start.ReadNtohU32 ();
  lenth = start.ReadNtohU32 ();
  sequence = start.ReadNtohU32 ();
  src_addr = start.ReadNtohU64 ();
  dst_addr = start.ReadNtohU64 ();
  src_port = start.ReadNtohU32 (); 
  dst_port = start.ReadNtohU32 ();
  allocate = start.ReadNtohU32 ();

  // we return the number of bytes effectively read.
  return 40;
}



//---------------------------------------------------------------------------------------------

Host::Host(){
	counter=0;
	head=NULL;
	end=NULL;
	cq_head=NULL;
	cq_end=NULL;
}


Host:: ~Host(){
	delete head;
	delete cq_head;
	delete end;
	delete cq_end;
}

void Host::getkind(bool k){
	kind=k;
}

void Host::getptr(Ptr<Socket> s){
	sock=s;
}

void Host::run(uint64_t address1,uint64_t address2,uint32_t length){

	Simulator::Schedule (Seconds(3.0), &Host::write ,this ,address1,address2,length);

}

void Host::write(uint64_t address1,uint64_t address2,uint32_t length){
	    cout<<"write"<<endl;
		// wqe_node * newnode;
		// newnode = new wqe_node;
		// cout<<"1"<<endl;
		// newnode->len=length;
		// cout<<"2"<<endl;
		// newnode->source_address=address1;
		// cout<<"3"<<endl;
		// newnode->destination_address=address2;
		// cout<<"4"<<endl;
		// newnode->next = NULL;
		// if(end==NULL){
		// 	end = newnode;
		// }
		
		// else{
		// 	end->next=newnode;
		// 	end = newnode;
		// }
		// cout<<"6"<<endl;
		// fflush(stdout);
		// if(head==NULL){
		// 	head = newnode;
		// }
		// cout<<"7"<<endl;
		// fflush(stdout);
		// uint32_t sequence=0;
		// cout<<"8"<<endl;
		// fflush(stdout);
		// WQE_send(sequence++,length,address1,address2,0,1,sock);	
		cout<<"wqe_send start"<<endl;
		fflush(stdout);
		std::cout <<" Server send a WQE at " << Simulator::Now ().GetSeconds () <<"s"<< std::endl;
		uint32_t xc_lenth=length;
		uint32_t xc_sequence = 0;
		uint64_t xc_src_addr = address1;
		uint64_t xc_dst_addr = address2;
		uint32_t xc_src_port=0;
		uint32_t xc_dst_port=1;

		myHeader header;
		header.flag = WQE;
		header.lenth = xc_lenth;
		header.sequence = xc_sequence;
		header.src_addr = xc_src_addr;
		header.dst_addr = xc_dst_addr;
		header.src_port = xc_src_port;
		header.dst_port = xc_dst_port;
		Ptr<Packet> packet = Create<Packet>(0);
		cout<<"Create packet finished"<<endl;
		// packet->AddHeader(header);
		cout<<"send"<<endl;
		sock->Send(packet);
		cout<<"send finished"<<endl;	
}

void Host::read(uint64_t address1,uint64_t address2,uint32_t length){
		wqe_node newnode;
		newnode.len=length;
		newnode.source_address=address1;
		newnode.destination_address=address2;
		*end->next=newnode;
		end->flag=2;
		end=&newnode;
		if(head==NULL){
			head=&newnode;
		}
}

void Host::generatecq(uint32_t number){
	cq_node newnode;
	newnode.no=number;
	*cq_end->next=newnode;
	cq_end=&newnode;
	if(cq_head==NULL){
		cq_head=&newnode;
	}
	counter++;
}

bool Host::checkcq(){
	if(counter>0)
		return 1;
	return 0;
}

wqe_node* Host::gethead(){
	return head;
}

wqe_node* Host::getend(){
	return end;
}


//-----------------------------------------------------------------------
void 
WQE_send(uint32_t xc_sequence,uint32_t xc_lenth,
	uint64_t xc_src_addr,uint64_t xc_dst_addr, uint32_t xc_src_port, uint32_t xc_dst_port,
	Ptr<Socket> sock)
{
	cout<<"wqe_send start"<<endl;
	fflush(stdout);
	std::cout <<" Server send a WQE at " << Simulator::Now ().GetSeconds () <<"s"<< std::endl;
	myHeader header;
	header.flag = WQE;
	header.lenth = xc_lenth;
	header.sequence = xc_sequence;
	header.src_addr = xc_src_addr;
	header.dst_addr = xc_dst_addr;
	header.src_port = xc_src_port;
	header.dst_port = xc_dst_port;
	Ptr<Packet> packet = Create<Packet>(0);
	packet->AddHeader(header);
	//cout<<"send"<<endl;
	sock->Send(packet);
	//cout<<"send finished"<<endl;
}

void
EA_send(myHeader header, Ptr<Socket> sock)
{
	header.flag = EA;
	Ptr<Packet> packet = Create<Packet>(0);
	packet->AddHeader(header);
	sock->Send(packet);
}

void
Data_send(myHeader xc_header, uint32_t xc_size, Ptr<Socket> sock)
{
	myHeader header;
	header = xc_header;
	header.flag = Data;
	Ptr<Packet> packet = Create<Packet>(xc_size);
	packet->AddHeader(header);
	cout<<"Server send packet with lenth of:"<<xc_size<<"  src_addr="<<header.src_addr<<"  dst_addr="<<header.dst_addr<<endl;
	sock->Send(packet);
}

void
ACK_send(myHeader xc_header, Ptr<Socket> sock)
{
	myHeader header;
	header = xc_header;
	header.flag = ACK;
	Ptr<Packet> packet = Create<Packet>(0);
	packet->AddHeader(header);
	sock->Send(packet);
}

void
GED_send(myHeader xc_header, Ptr<Socket> sock, uint32_t MSS, uint32_t cwnd)
{
	if(cwnd>0)
{
	std::cout << "at=" << Simulator::Now ().GetSeconds () << "s,Client send GED of src_addr:" << xc_header.src_addr << std::endl;
	myHeader header;
	header = xc_header;
	header.lenth = MSS;//复用lenth位
	header.flag = GED;
	Ptr<Packet> packet = Create<Packet>(0);
	packet->AddHeader(header);
	sock->Send(packet);
	cwnd--;
	header.src_addr+=MSS;
	header.dst_addr+=MSS;
	Simulator::Schedule (Seconds (0.5), &GED_send,  header, sock, MSS, cwnd);
}
}

// void
// HCA::complete_recv(myHeader xc_header)
// {
// 	myHeader header;
// 	header = xc_header;
// 	header.flag = CQ;
// 	head->allcate = 0;
// 	head++;
// }

void
WQE_recv(Ptr<Packet> packet, Ptr<Socket> sock)
{
	//std::cout <<" WQE_recv" << Simulator::Now ().GetSeconds () << std::endl;
	myHeader header;
	packet->PeekHeader(header);
	// wqe_node* tail;
	// tail = host->getend();
	// tail->allcate = 1;
	// tail->source_address = header.src_addr;
	// tail->destination_address = header.dst_addr;
	// tail->sequence = header.sequence;
	// tail->dst_port = header.dst_port;
	// tail->src_port = header.src_port;
	//EA_send(header,sock);
	uint32_t MSS=1,cwnd=10;//total_len;
	//total_len = header.lenth;
	myHeader GED_header;
	GED_header = header;
	GED_header.flag = GED;
	GED_send(GED_header,sock,MSS,cwnd);
	// while(total_len>0)
	// {
	// 	for(uint32_t i=0;i<(total_len<cwnd? total_len:cwnd);i++)
	// 	{
	// 		GED_send(GED_header,sock,MSS);
	// 		GED_header.src_addr+=MSS;
	// 		GED_header.dst_addr+=MSS;
	// 	}
	// 	total_len-=cwnd;
	// }

}

void
EA_recv(myHeader xc_header)
{
}

void
Data_recv(Ptr<Packet> packet, Ptr<Socket> sock)
{
	myHeader header;
	packet->PeekHeader(header);
	cout<<"at "<<Simulator::Now ().GetSeconds ()<<"s Client recv packet :"
	<<" src_addr:"<<header.src_addr
	<<" size:"<<packet->GetSize()<<endl;
	Ptr<Packet> ack = Create<Packet>(0);
	header.flag = ACK;
	ack->AddHeader(header);
	sock->Send(ack);
}

void
ACK_recv(Ptr<Packet> packet)
{
	myHeader header;
	packet->PeekHeader(header);
	cout<<"at "<<Simulator::Now ().GetSeconds ()<<"s client recv ack :"
	<<" src_addr:"<<header.src_addr<<endl;
}

void
 GED_recv(myHeader header, Ptr<Socket> sock)
 {
 	//cout<<(Simulator::Now ()).GetSeconds ()<<endl;
 	Data_send(header,header.lenth,sock);
 }

