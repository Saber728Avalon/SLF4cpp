模仿JAVA的 SLF4J。形成一个公共的日志框架。但是不提供日志具体实现（内容里面有一个简单的stdout,和写文件）。仅仅实现日志的抽象框架，方便进行日志框架的、切换        
               
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
直接继承 slf_log_provider,可以参考simple目录     
     
class MyLogPlatform : public slf_log_provider    
......     
       
auto ptr = new MyLogPlatform();    
replace_log_provider(ptr);     
    
3.日志末尾是否增加\n宏
主要是解决日志是否需要换行问题，如果日志框架不换行可以启动该功能 	
LOG_END_RETURN    
        
           
4.日志编码切换宏    
目前支持四种编码     
ASC : SLF4CPP_USE_CHAR   也是默认编码, utf8 或者ascii使用这种编码      
UTF16/Unicode: SLF4CPP_USE_UTF16  用来支持windows的unicode编码      
UTF32 : SLF4CPP_USE_UTF32 Linux对于unicode编码的支持     
           
		   
5.日志时间格式化		   
[yyyy-MM-dd HH:mm:ss.SSS] {level} [tid]		   
默认是不添加日志的时间前缀的		   
		   
5.支持VS2010及其以上		   



