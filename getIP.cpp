#include <boost/asio.hpp>  
#include <iostream>  
  
using boost::asio::ip::tcp;  
  
main() {  
    boost::asio::io_service io_service;  
    tcp::resolver resolver(io_service);  
    tcp::resolver::query query(boost::asio::ip::host_name(), "");  
    tcp::resolver::iterator iter = resolver.resolve(query);  
    tcp::resolver::iterator end; // End marker.  
    while (iter != end)  
    {  
        tcp::endpoint ep = *iter++;  
        std::cout << ep.address().to_string() << std::endl;  
    }  
} 