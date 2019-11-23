#include <iostream>
#include "activemq/core/ActiveMQConnectionFactory.h"
#include "activemq/library/ActiveMQCPP.h"

class MyMessageListener : public cms::MessageListener{
public:
	void onMessage(const cms::Message* message){
		if(message != nullptr){
			cms::TextMessage const* msg{dynamic_cast<const cms::TextMessage*>(message)};
			if(msg != nullptr){
				std::cout << "callback receiver: " << msg->getText() << std::endl;
			}else{
				std::cout << "not text message" << std::endl;
			}
		}else{
			std::cout << "message is null" << std::endl;
		}
	}
};

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
		cms::ConnectionFactory* cf{cms::ConnectionFactory::createCMSConnectionFactory(broker_url)};
		connection = cf->createConnection();
		delete cf;
		cf = nullptr;
		
		connection->start();
		session = connection->createSession(cms::Session::AUTO_ACKNOWLEDGE);
		queue = session->createQueue(queue_name);
		consumer = session->createConsumer(queue);
		
		MyMessageListener* message_listener{new MyMessageListener};
		consumer->setMessageListener(message_listener);
		
		decaf::lang::Thread::sleep(1000 * 10);
		delete message_listener;
		message_listener = nullptr;
		
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