#include <iostream>
#include "activemq/core/ActiveMQConnectionFactory.h"
#include "activemq/library/ActiveMQCPP.h"

int main()
{
	activemq::library::ActiveMQCPP::initializeLibrary();
	
	cms::Connection* connection{nullptr};
	cms::Session* session{nullptr};
	cms::Topic* topic{nullptr};
	cms::MessageConsumer* consumer{nullptr};
	std::string broker_url{"vm://localhost"};
	std::string topic_name{"test_topic"};
	
	try{
		activemq::core::ActiveMQConnectionFactory acf{broker_url}; 
		connection = acf.createConnection();
		connection->start();
		
		session = connection->createSession(cms::Session::AUTO_ACKNOWLEDGE);
		topic = session->createTopic(topic_name);
		consumer = session->createConsumer(topic);
		
		while(true){
			cms::Message* message{consumer->receive()};
			if(message != nullptr){
				cms::TextMessage* msg{dynamic_cast<cms::TextMessage*>(message)};
				if(msg != nullptr){
					std::cout << "receiver: " << msg->getText() << std::endl;
				}else{
					std::cout << "not a text message" << std::endl;
				}
			}else{
				break;
			}
			delete message;
		}
		
		if(topic != nullptr){
			delete topic;
			topic = nullptr;
		}
		if(consumer != nullptr){
			delete consumer;
			consumer = nullptr;
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
