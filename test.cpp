#include <re2/re2.h>
#include <string>
#include <string_view>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <cctype>
#include <unordered_set>
#include <boost/asio.hpp>
#include <thread>
#include <functional>
#include <algorithm>
#include <filesystem>

namespace io = boost::asio;
namespace fs = std::filesystem;

class RequestHandler{
private:
	std::string FormatNumber(const std::string& number){
		
		std::string result;
		result.reserve(9);
	
		if(number.size() < 9) result += "8495";
	
		for(char x: number){
			if(std::isdigit(x)) result.push_back(x);
		}		
		result[0] = '8';
	
		return result;
	}
	
	void CountNumbers(std::string website){
	
		auto dir_copy = dir;
		dir_copy /= "file";
		std::thread::id id = std::this_thread::get_id();	
		std::string file = dir_copy.string() +std::to_string(*static_cast<unsigned int*>(static_cast<void*>(&id))) + ".txt";
		std::string request = "wget --quiet -p --output-document=";
		request += file + " " + website;

		std::system(request.c_str());
		
		RE2 myRegex("((((8)|(\\+\\s{0,1}7))(\\s|-){0,1}\\({0,1}[0-9]{3}\\){0,1}((\\s|-){0,1}\\d){7})|(([0-9]{2}(\\s|-){0,1})){3})");
			
		std::ifstream t(file);
		std::stringstream buffer;
		buffer << t.rdbuf();
		std::string content(buffer.str());
		absl::string_view input(content);
		std::string word;
	
		std::unordered_set<std::string> numbers;
	
		while(RE2::FindAndConsume(&input, myRegex, &word)){
			numbers.insert(FormatNumber(word));
		}
	
		for(const std::string& num: numbers){
			std::cout << num << std::endl;
		}
		std::error_code error;
		fs::remove(file, error);	
	}
	
	
public:
	
	RequestHandler():thread_pool(std::max(2u, std::thread::hardware_concurrency())), dir(fs::current_path()){
		dir /= "temp_new_dir";
		std::error_code error;
		fs::create_directory(dir,error);
	}

	void ProcessRequests(){
		std::string url;
		while(std::cin){
			std::cin >> url;
			io::post(thread_pool, std::bind(&RequestHandler::CountNumbers, this, url));
		}
	} 
	
	~RequestHandler(){
		thread_pool.join();
		std::error_code error;
		fs::remove_all(dir, error);
	}
private:
	io::thread_pool thread_pool;
	fs::path dir;
};

int main(int argc, char* agrv[]){

	RequestHandler srv;
	srv.ProcessRequests();
}

