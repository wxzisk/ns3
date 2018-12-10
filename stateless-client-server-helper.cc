#include "stateless-client-server-helper.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"

namespace ns3 {

statelessServerHelper::statelessServerHelper ()
{
}

statelessServerHelper::statelessServerHelper (Address addr1,Address addr2,uint16_t port)
{
  m_factory.SetTypeId (statelessserver::GetTypeId ());
  SetAttribute ("LocateAddress", AddressValue (addr1));
  SetAttribute ("RemoteAddress", AddressValue (addr2));
  SetAttribute ("Port", UintegerValue (port));
}

void
statelessServerHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}


ApplicationContainer
statelessServerHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<statelessserver> server = m_factory.Create<statelessserver> ();
      node->AddApplication (server);
      apps.Add (server);

    }
  return apps;
}

statelessClientHelper::statelessClientHelper ()
{
}

statelessClientHelper::statelessClientHelper (Address addr1,Address addr2, uint16_t port)
{
  m_factory.SetTypeId (statelessclient::GetTypeId ());
  SetAttribute ("LocateAddress", AddressValue (addr1));
  SetAttribute ("RemoteAddress", AddressValue (addr2));
  SetAttribute ("RemotePort", UintegerValue (port));
}

statelessClientHelper::statelessClientHelper (Address address)
{
  m_factory.SetTypeId (statelessclient::GetTypeId ());
  SetAttribute ("RemoteAddress", AddressValue (address));
}

void
statelessClientHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
statelessClientHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<statelessclient> client = m_factory.Create<statelessclient> ();
      node->AddApplication (client);
      apps.Add (client);
    }
  return apps;
}

}//ns3 namespace