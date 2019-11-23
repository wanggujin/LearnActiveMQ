#include <iostream>
#include <thread>
#include "activemq/core/ActiveMQConnectionFactory.h"
#include "activemq/library/ActiveMQCPP.h"

void ConsumerThread(){
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
		
		while(true){
			cms::Message* message{consumer->receive(5000)};
			if(message != nullptr){
				cms::TextMessage* msg{dynamic_cast<cms::TextMessage*>(message)};
				if(msg != nullptr){
					std::cout << "receiver: " << msg->getText() << std::endl;
				}else{
					std::cout << "not text message" << std::endl;
				}
			}else{
				break;
			}
		}
	}catch(cms::CMSException& e){
		e.printStackTrace();
	}
}

int main()
{
	activemq::library::ActiveMQCPP::initializeLibrary();
	
	std::thread t{ConsumerThread};
	
	//处理其他逻辑
	
	t.join();
	
	activemq::library::ActiveMQCPP::shutdownLibrary();
	return 0;
}