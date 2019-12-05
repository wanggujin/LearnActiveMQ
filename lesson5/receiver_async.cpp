#include <iostream>
#include <thread>
#include "activemq/core/ActiveMQConnectionFactory.h"
#include "activemq/core/policies/DefaultPrefetchPolicy.h"
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
		activemq::core::policies::DefaultPrefetchPolicy dpp;
		dpp.setQueuePrefetch(1);
		acf.setPrefetchPolicy(&dpp);
		acf.setDispatchAsync(false);
		connection = acf.createConnection();		
		connection->start();
		session = connection->createSession(cms::Session::AUTO_ACKNOWLEDGE);
		queue = session->createQueue(queue_name);
		consumer = session->createConsumer(queue);
		
		while(true){
			cms::Message* message{consumer->receive()};
			if(message != nullptr){
				cms::TextMessage* msg{dynamic_cast<cms::TextMessage*>(message)};
				if(msg != nullptr){
					std::cout << "receiver: " << msg->getText() << std::endl;
				}else{
					std::cout << "not a text message" << std::endl;
				}
				delete msg;
				std::this_thread::sleep_for(std::chrono::seconds(1));
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
