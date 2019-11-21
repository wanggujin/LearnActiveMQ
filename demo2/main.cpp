#include <iostream>
#include "activemq/core/ActiveMQConnectionFactory.h"
#include "activemq/library/ActiveMQCPP.h"
#include "decaf/lang/Thread.h"

class MyMessageListener : public cms::MessageListener{
public:
	void onMessage(const cms::Message* message){
		cms::TextMessage const* msg{dynamic_cast<cms::TextMessage const*>(message)};
		if(msg != nullptr){
			std::cout << "receiver: " << msg->getText() << std::endl;
		}else{
			std::cout << "not a text message" << std::endl;
		}
	}
};

class MyMessageAvailableListener : public cms::MessageAvailableListener{
	void onMessageAvailable(cms::MessageConsumer* consumer){
		std::cout << "has message coming" << std::endl;
	}
};

class MyExceptionListener : public cms::ExceptionListener{
	void onException(const cms::CMSException& e){
		e.printStackTrace();
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
		cms::ConnectionFactory* cf{cms::ConnectionFactory::createCMSConnectionFactory(broker_url)};//2
		connection = cf->createConnection();
		delete cf;
		cf = nullptr;
		
		MyExceptionListener* exception_listener{new MyExceptionListener()};
		connection->setExceptionListener(exception_listener);
		
		connection->start();
		session = connection->createSession(cms::Session::AUTO_ACKNOWLEDGE);
		queue = session->createQueue(queue_name);
		consumer = session->createConsumer(queue);
#if 1
		MyMessageListener* message_listener{new MyMessageListener()};
		consumer->setMessageListener(message_listener);
		
		decaf::lang::Thread::sleep(1000 * 10);
		delete message_listener;
		message_listener = nullptr;
#else
		MyMessageAvailableListener* message_available_listener{new MyMessageAvailableListener()};
		consumer->setMessageAvailableListener(message_available_listener);
		while(true){
			cms::Message* message{consumer->receive(1000)};
			cms::TextMessage* msg{dynamic_cast<cms::TextMessage*>(message)};
			if(msg != nullptr){
				std::cout << "receiver: " << msg->getText() << std::endl;
			}else{
				std::cout << "not text message" << std::endl;
				break;
			}
			delete message;
		}
#endif
		
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
