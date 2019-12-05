# 前言

前面我们介绍了`ActiveMQ`的基本开发方式，包括点对点和发布-订阅模式。而不论哪一种模式，我们在使用连接工厂创建连接到`ActiveMQ`实例，即Broker时，都用到了一个url。之前我们写的都是`tcp://127.0.0.1:61616`，即基于TCP协议从61616端口连接到本机的Broker实例。这里的`tcp`就是`ActiveMQ`所支持的一种传输连接器 `transportConnector`。而根据官网介绍，目前的`ActiveMQ`支持很多种类的传输连接器，我们从`ActiveMQ`默认的配置文件中也能看出一二。默认的配置文件在`ActiveMQ`的安装目录下的`conf`目录，笔者的是`/opt/activemq/conf/activemq.xml`。该文件中，有关一个`transportConnectors`标签，里面包含了该版本的`ActiveMQ`所默认支持的传输器。如下：
```xml
<transportConnectors>
	<transportConnector name="openwire" uri="tcp://0.0.0.0:61616?maximumConnections=1000&amp;wireFormat.maxFrameSize=104857600"/>
	<transportConnector name="amqp" uri="amqp://0.0.0.0:5672?maximumConnections=1000&amp;wireFormat.maxFrameSize=104857600"/>
	<transportConnector name="stomp" uri="stomp://0.0.0.0:61613?maximumConnections=1000&amp;wireFormat.maxFrameSize=104857600"/>
	<transportConnector name="mqtt" uri="mqtt://0.0.0.0:1883?maximumConnections=1000&amp;wireFormat.maxFrameSize=104857600"/>
	<transportConnector name="ws" uri="ws://0.0.0.0:61614?maximumConnections=1000&amp;wireFormat.maxFrameSize=104857600"/>
</transportConnectors>
```

可见，每中传输器的配置，主要包括 传输器的名字`name`、传输器的`uri`及其属性。其实，我们还可以为每一个传输器指定其他配置项，以后的文章中会为大家介绍。

# 传输连接器

除了上面看到的默认配置的传输连接器，还有很多其他的连接器。列举如下：

- `AUTO Transport` 支持协议自检测的传输连接器。 
- `VM Transport` 基于Java虚拟机的传输连接器。
- `AMQP Transport` 基于AMQP协议的传输连接器。
- `MQTT Transport` 基于MQTT协议的传输连接器。
- `TCP Transport` 基于TCP协议的传输连接器。
- `NIO Transport` 基于Java NIO的传输连接器。
- `SSL Transport` 基于SSL的传输连接器。
- `NIO SSL Transport` 基于SSL的NIO传输连接器。
- `Peer Transport` 端到端的传输连接器。
- `UDP Transport` 基于UDP的传输连接器。
- `Multicast Transport` 基于多播的传输连接器。
- `HTTP and HTTPS Transport` 基于HTTP(s)的传输连接器。
- `WebSockets Transport` 基于WebSocket的传输连接器。
- `Stomp Transport` 基于Stomp协议的传输连接器。
- `Failover Transport` 具有自动重连机制的传输连接器。
- `Fanout Transport` 具有自动重连机制和消息复制的传输连接器。
- `Discovery Transport` 具有自动发现可用Broker的传输连接器。
- `ZeroConf Transport` 基于ZeroConf服务发现机制的传输连接器，类似与Discovery。

# AUTO

从5.13.0开始，`ActiveMQ`支持 `OpenWire`、`amqp`、`stomp`、`mqtt`四种协议的自动检测，即客户端在连接Broker时，可以使用这四种协议的任意一种去连接。

比如，我们可以开启基于TCP的自动检测。配置如下：
```xml
<transportConnector name="auto" uri="auto://localhost:5671"/>
```

如果我们将我们`ActiveMQ`的默认的传输器配置删除，只在`transportConnectors`中加入这么这一种配置，那么，我们之前所有的代码仍然可以正常运行。

而且，`AUTO`还可以和其他类型的传输器结合使用。比如：

搭配`SSL`：
```xml
<transportConnector name="auto+ssl" uri="auto+ssl://localhost:5671"/>
```

搭配`NIO`
```xml
<transportConnector name="auto+nio" uri="auto+nio://localhost:5671"/>
```

三者合并：
```xml
<transportConnector name="auto+nio+ssl" uri="auto+nio+ssl://localhost:5671"/>
```

大家可以根据需要，自行选择。至于怎么在连接中使用SSL，后文会介绍。

另外，还可以为`AUTO`类型的传输器添加两个额外配置。

- `protocolDetectionTimeOut` 协议检测的超时时长。当一个客户端连接到Broker，但在此时长内没有发送数据或没有发送足够的数据供协议检测，那么，Broker会终止该连接。
- `maxConnectionThreadPoolSize` 处理连接尝试的线程池大小。限制该值，可以防止同一时间大量的客户端尝试连接导致Broker崩溃。

比如：
```xml
<transportConnector name="auto" uri="auto://localhost:5671?protocolDetectionTimeOut=5000"/>
```

# VM

该传输器允许客户端彼此通过Java虚拟机来进行连接，消除网络开销。这种情况下所使用的连接不是普通的socket连接，而是使用直接方法调用来启用高性能的嵌入式消息传递系统。

第一个使用`VM`连接的客户端会启动一个嵌入式的Broker，后续的连接会`attach`到该Broker。并且，一旦所有到该`VM`的连接被关闭后，该嵌入式Broker会被自动关闭。