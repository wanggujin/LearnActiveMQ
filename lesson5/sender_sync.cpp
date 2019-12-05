#include <iostream>
#include <chrono>
#include "activemq/core/ActiveMQConnectionFactory.h"
#include "activemq/library/ActiveMQCPP.h"
#include "cms/ConnectionFactory.h"

class MyAsyncCallback : public cms::AsyncCallback{
public:
	void onSuccess(){
//		std::cout << "message send success" << std::endl;
		delete this;
	}
	
	void onException(const cms::CMSException& ex){
		std::cout << "message send failed: " << ex.what() << std::endl;
		delete this;
	}
};

int main()
{
	activemq::library::ActiveMQCPP::initializeLibrary();
	cms::Connection* connection{nullptr};
	cms::Session* session{nullptr};
	cms::Queue* queue{nullptr};
	cms::MessageProducer* producer{nullptr};
	int num_messages{50000};
	std::string broker_url{"tcp://127.0.0.1:61616"};
	std::string queue_name{"test_queue"};
	
	try{
		activemq::core::ActiveMQConnectionFactory acf{broker_url};
		connection = acf.createConnection();
		connection->start();
		
		session = connection->createSession(cms::Session::AUTO_ACKNOWLEDGE);
		queue = session->createQueue(queue_name);
		producer = session->createProducer(queue);
		
		auto start{std::chrono::system_clock::now()};
		for(int i{0}; i < num_messages; ++i){
			cms::TextMessage* msg{session->createTextMessage("message-" + std::to_string(i))};
			producer->send(msg);
		}
		auto end{std::chrono::system_clock::now()};
		std::cout << "time span: " << 
								 std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << 
								 "ms" << std::endl;
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
