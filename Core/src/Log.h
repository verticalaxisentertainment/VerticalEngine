#pragma once

#ifdef PLATFORM_WINDOWS
	#define ERROR(x) SetConsoleTextAttribute(hcc,0x07);std::cout<<"[";SetConsoleTextAttribute(hcc,4);std::cout<<"error";SetConsoleTextAttribute(hcc,0x07);std::cout<<"] "<<x<<"\n"
	#define INFO(x)SetConsoleTextAttribute(hcc,0x07);std::cout<<"[";SetConsoleTextAttribute(hcc,1);std::cout<<"info";SetConsoleTextAttribute(hcc,0x07);std::cout<<"] "<<x<<"\n"
	#define WARNING(x)SetConsoleTextAttribute(hcc,0x07);std::cout<<"[";SetConsoleTextAttribute(hcc,6);std::cout<<"warning";SetConsoleTextAttribute(hcc,0x07);std::cout<<"] "<<x<<"\n"

	/*#define ERROR(x) std::cout<<"(ERROR): "<<x<<"\n";
	#define INFO(x) std::cout<<"(INFO): "<<x<<"\n";
	#define WARNING(x) std::cout<<"(WARNING): "<<x<<"\n";*/
#endif
