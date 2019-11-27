#include <iostream>
#include "activemq/core/ActiveMQConnectionFactory.h"
#include "activemq/library/ActiveMQCPP.h"

int main()
{
	activemq::library::ActiveMQCPP::initializeLibrary();
	
	cms::Connection* connection{nullptr};
	cms::Session* session{nullptr};
	cms::Queue* queue{nullptr};
	cms::MessageConsumer* consumer{nullptr};
	std::string broker_url{"tcp://127.0.0.1:61616"};
	std::string queue_name{"test_queue"};
	
	try{
		activemq::core::ActiveMQConnectionFactory acf{broker_url}; 
		connection = acf.createConnection();
		connection->start();
//		session = connection->createSession(cms::Session::CLIENT_ACKNOWLEDGE);
		session = connection->createSession(cms::Session::INDIVIDUAL_ACKNOWLEDGE);
		queue = session->createQueue(queue_name);
		consumer = session->createConsumer(queue);
		
		int count{0};
		while(true){
			cms::Message* msg{consumer->receive(3000)};
			if(msg != nullptr){
				cms::TextMessage* text_msg{dynamic_cast<cms::TextMessage*>(msg)};
				if(text_msg != nullptr){
					std::cout << "receiver: " << text_msg->getText() << std::endl;
				}else{
					std::cout << "not a text message" << std::endl;
				}
				
				count++;
				if(count % 5 == 0){
					msg->acknowledge();
				}
				
				delete msg;
			}else{
				break;
			}
		}
		
		if(queue != nullptr){
			delete queue;
			queue = nullptr;
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