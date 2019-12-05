#include <iostream>
#include "activemq/core/ActiveMQConnectionFactory.h"
#include "activemq/library/ActiveMQCPP.h"
#include "activemq/core/ActiveMQConnection.h"

int main()
{
	activemq::library::ActiveMQCPP::initializeLibrary();
	cms::Connection* connection{nullptr};
	cms::Session* session{nullptr};
	cms::Topic* topic{nullptr};
	cms::MessageProducer* producer{nullptr};
	int num_messages{5};
	std::string broker_url{"vm://localhost"};
	std::string topic_name{"test_topic"};
	
	try{
		activemq::core::ActiveMQConnectionFactory acf{broker_url};
		connection = acf.createConnection();
		connection->start();        
		
		session = connection->createSession(cms::Session::AUTO_ACKNOWLEDGE);
		topic = session->createTopic(topic_name);
		producer = session->createProducer(topic);
		for(int i{0}; i < num_messages; ++i){
			std::unique_ptr<cms::TextMessage> msg{session->createTextMessage("message-" + std::to_string(i))};
			producer->send(msg.get());
		}
		std::cout << "send done" << std::endl;
		
		if(topic != nullptr){
			delete topic;
			topic = nullptr;
		}
		if(producer != nullptr){
			delete producer;
			producer = nullptr;
		}
		if(session != nullptr){
			session->close();
			delete session;
			session = nullptr;
		}
		if(connection != nullptr){
			connection->close();
			delete connection;
			connection = nullptr;
		}
	}catch(cms::CMSException& e){
		e.printStackTrace();
	}
	
	activemq::library::ActiveMQCPP::shutdownLibrary();
	
	return 0;
}
