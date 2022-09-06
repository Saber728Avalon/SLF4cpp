模仿JAVA的 SLF4J。形成一个公共的日志框架。但是不提供日志具体实现。仅仅实现日志的抽象框架，方便进行日志框架的、切换        

1.使用方法
#include <slf4cpp.h>

SLF4CPP_INIT(0);      
SLF4CPP_PREFFIX("[yyyy-MM-dd HH:mm:ss.SSS] {level} [tid] ");      
......       
SLF4CPP(DEBUG)("{}", 1234);     
SLF4CPP(INFO)("{}", "asdfawertqeart");           
SLF4CPP(WARN)("{}", "2345463546");         
SLF4CPP(ERROR)("{}", "hgukigyu fdgthysd fty")           

SLF4CPP(INFO)("{} {}", "asdfawertqeart", 1234);  
SLF4CPP(INFO)("a={} b={}", a, b);  

2.增加添加日志框架支持       
直接继承 slf_log_provider     
     
class MyLogPlatform : public slf_log_provider    
......     

auto ptr = new MyLogPlatform();    
replace_log_provider(ptr);     
    

