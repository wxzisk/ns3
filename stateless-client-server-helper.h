#ifndef STATELESS_CLIENT_SERVER_HELPER_H
#define STATELESS_CLIENT_SERVER_HELPER_H
#include <stdint.h>
#include "ns3/application-container.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/ipv4-address.h"
#include "ns3/stateless-server.h"
#include "ns3/stateless-client.h"
#include "ns3/ptr.h"
namespace ns3 {

class statelessServerHelper
{
public:
  statelessServerHelper ();
  statelessServerHelper (Address addr1,Address addr2,uint16_t port);
  void SetAttribute (std::string name, const AttributeValue &value);
  ApplicationContainer Install (NodeContainer c);

private:
  ObjectFactory m_factory; //!< Object factory.
};

class statelessClientHelper
{
public:
  statelessClientHelper ();
  statelessClientHelper (Address addr1,Address addr2, uint16_t port);
  statelessClientHelper (Address addr);
  void SetAttribute (std::string name, const AttributeValue &value);
  ApplicationContainer Install (NodeContainer c);
private:
  ObjectFactory m_factory; //!< Object factory.
};

} // namespace ns3

#endif