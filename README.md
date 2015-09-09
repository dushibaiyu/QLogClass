# QLogClass
		自己用的针对QWidget程序的写日志的类。利用QThread+Qt事件循环实现的。
		单独线程写文件，用Qt事件的形式发送到类，线程安全的。
		支持日志分级。
		使用时：
		先调用 QLOG::QLog::initLog()进行初始化。