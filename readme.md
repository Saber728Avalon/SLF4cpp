模仿JAVA的 SLF4J。形成一个公共的日志框架。但是不提供日志具体实现（内容里面有一个简单的stdout,和写文件）。仅仅实现日志的抽象框架，方便进行日志框架的、切换。        
支持将所有的日志移除，在特殊需要时候，需要移除所有日志信息（比如避免被分析）。启用宏REMOVE_LOG_INFO就可以将所有的日志移除               
               
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
                  
输出代码的函数名和代码行数      
SLF4CPPL(DEBUG)("{}", "TEst Line");         
SLF4CPPL(WARN)("{}", "TEst Line");          
SLF4CPPL(INFO)("{}", "TEst Line");          
SLF4CPPL(ERROR)("{}", "TEst Line");         
				  
			   
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
yyy-MM-dd HH:mm:ss.SSS  日志时间     
level 日志级别     
tid 线程号      
          
6.支持VS2010及其以上		   
             
          
            
7.日志样板         
下面是一个日志的简单输出样式，方便大家查看       
[2022-09-08 10:25:43.343] {DEBUG} [21052] xxxx14789 25565 37889 -1xxxx        
[2022-09-08 10:25:43.343] {DEBUG} [21052] xxxx14789 -1 yyyyy  aaaa 99             
[2022-09-08 10:25:43.343] {DEBUG} [21052]              
[2022-09-08 10:25:43.343] {DEBUG} [21052] 1234             
[2022-09-08 10:25:43.343] { INFO} [21052] asdfawertqeart               
[2022-09-08 10:25:43.343] { WARN} [21052] 2345463546           
[2022-09-08 10:25:43.343] {ERROR} [21052] hgukigyu fdgthysd fty            
[2022-09-08 10:28:44.524] {DEBUG} [21628] [main(193)] TEst Line           
[2022-09-08 10:28:44.524] {DEBUG} [21628] [main(194)] TEst Line                 

