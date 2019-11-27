#include <iostream>
#include "activemq/core/ActiveMQConnectionFactory.h"
#include "activemq/library/ActiveMQCPP.h"
#include "cms/ConnectionFactory.h"

int main()
{
	activemq::library::ActiveMQCPP::initializeLibrary();
	cms::Connection* connection{nullptr};
	cms::Session* session{nullptr};
	cms::Queue* queue{nullptr};
	cms::MessageProducer* producer{nullptr};
	int num_messages{5};
	std::string broker_url{"tcp://127.0.0.1:61616"};
	std::string queue_name{"test_queue"};
	
	try{
		activemq::core::ActiveMQConnectionFactory acf{broker_url};
		connection = acf.createConnection();
		connection->start();
		
		session = connection->createSession(cms::Session::SESSION_TRANSACTED);
		queue = session->createQueue(queue_name);
		producer = session->createProducer(queue);
		for(int i{0}; i < num_messages; ++i){
			cms::TextMessage* msg{session->createTextMessage("message-" + std::to_string(i))};
			producer->send(msg);
//			session->commit();
			delete msg;
		}
		session->commit();
		std::cout << "send done" << std::endl;
		
		if(queue != nullptr){
			delete queue;
			queue = nullptr;
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
