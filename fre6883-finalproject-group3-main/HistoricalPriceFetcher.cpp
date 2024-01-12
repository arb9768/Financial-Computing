#include <stdio.h>
#include <string.h>
#include <cstring>
#include <string> 
#include <iostream>
#include <sstream>  
#include <vector>
#include <locale>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <functional>
#include <map>
#include "curl/curl.h"
#include "HistoricalPriceFetcher.h"

using namespace std;
namespace fre {
    vector<string> GetDates(map<string, double> priceMap){
        vector<string> dates;
        for (map<string, double>::iterator itr = priceMap.begin(); itr != priceMap.end(); itr++){
            dates.push_back(itr->first);
        }
        return dates;
    }
    
    vector<double> GetPrices(map<string, double> priceMap){
        vector<double> prices;
        for (map<string, double>::iterator itr = priceMap.begin(); itr != priceMap.end(); itr++){
            prices.push_back(itr->second);
        }
        return prices;
    }

    vector<double> CalcBenchmarkReturn(string startDate, string endDate, map<string, double>& benchmark){
        vector<double> prices;
        bool inRange = false;
	    
	    for (map<string, double>::iterator itr = benchmark.begin(); itr !=benchmark.end(); itr++){
	        string date = itr->first;
	        
	        if ((!inRange) && (date == startDate)) {
	            inRange = true;
	        }
	        
	        if (!inRange) {
	            continue;
	        } else {
	            prices.push_back(itr->second);
	        }
	        
	        if (itr->first == endDate) {
	            break;
	        }
	    }
	    
	    vector<double> returns;
	    int size = prices.size();
        for (int i = 0; i < size - 1; i++) {
            returns.push_back(log(prices[i+1]/prices[i]));
        }
        
	    return returns;
    }
    
    string startDay(vector<string> tradingDays, string day0, int N){
        vector<string>::iterator itr = find(tradingDays.begin(), tradingDays.end(), day0);
        if(itr == tradingDays.end()){
            cout << "Given date is not a trading day "<< day0 << endl;
        }
        else{
            if(distance(tradingDays.begin(), itr) < N){
                cout << "Not enough data available" << endl;
                return "Skip";
            }
            else{
                itr = itr - N;
            }
        }
        string startDate = *itr;
        return startDate;
    }
    
    string endDay(vector<string> tradingDays, string day0, int N){
        vector<string>::iterator itr = find(tradingDays.begin(), tradingDays.end(), day0);
        if(itr == tradingDays.end()){
            cout << "Given date is not a trading day "<< day0 << endl;
        }
        for (int n = 1; n <= N; n++){
            itr++;
            if(itr == tradingDays.end()) {
                return "Skip";
            }
        }
	    
        string endDate = *itr;
        return endDate;
    }
    
    int month_num(string month){
        string months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
        "Aug", "Sep", "Oct", "Nov", "Dec"};
        
        for (int i = 0; i < 12; i++){
            if (months[i] == month){
                return i + 1;
            }
        }
        
        return -1;
    }
    
    
    string convert_date(string date) {
        if (date.length() == 8){
            date = "0" + date;
        }
        string day = date.substr(0, 2);
        string month = date.substr(3, 3);
        int m = month_num(month);
        month = to_string(m);
        if (month.size() == 1){
            month = "0" + month;
        }
        
        string year = date.substr(7);
        if (year.size() == 2){
            year = "20" + year;
        }
        
        return year + "-" + month + "-" + day;
        
    }
    
    void* myrealloc(void* ptr, size_t size)
    {
    	if (ptr)
    		return realloc(ptr, size);
    	else
    		return malloc(size);
    }
    
    int write_data2(void* ptr, size_t size, size_t nmemb, void* data)
    {
    	size_t realsize = size * nmemb;
    	struct MemoryStruct* mem = (struct MemoryStruct*)data;
    	mem->memory = (char*)myrealloc(mem->memory, mem->size + realsize + 1);
    	if (mem->memory) {
    		memcpy(&(mem->memory[mem->size]), ptr, realsize);
    		mem->size += realsize;
    		mem->memory[mem->size] = 0;
    	}
    	return realsize;
    }
    
    int FetchPrices(vector<Ticker>& tickers, vector<Ticker>& tickersFullData, map<string, double>& benchmark, int N) {
        CURL* handle;
		CURLcode result;
		
		// set up the program environment that libcurl needs 
    	curl_global_init(CURL_GLOBAL_ALL);
    
		// curl_easy_init() returns a CURL easy handle 
 		handle = curl_easy_init();
 		
 		// if everything's all right with the easy handle...
 		if (handle)
 		{	
 			vector<string> tradingDays;
    		tradingDays = GetDates(benchmark);
    		
 			string url_common = "https://eodhistoricaldata.com/api/eod/";
 			// You must replace this API token with yours 
 			string api_token = "656a140f854ac5.76574891";
 			
 			for (vector<Ticker>::iterator itr = tickers.begin(); itr != tickers.end(); itr++) {
 				//cout << "Ticker: " << itr->GetSymbol() << endl;
 				string day0 = convert_date(itr->GetDate());
 				string startDate = startDay(tradingDays, day0, N);
 				string endDate = endDay(tradingDays, day0, N);
 				if (startDate == "Skip" || endDate == "Skip"){
 				    cout << "Warning: " << itr->GetSymbol() << " does not have enough data for the provided input" << endl;
 				    continue;
 				}
    			else {
        			struct MemoryStruct data;
     				data.memory = NULL;
     				//cout << "Not Skipped" << endl;
     				data.size = 0;
     				string url_request = url_common + itr->GetSymbol() + ".US?" + "from="
    					+ startDate + "&to=" + endDate + "&api_token="
    					+ api_token + "&period=d";
    					
    				curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());
    				//adding a user agent
    				curl_easy_setopt(handle, CURLOPT_USERAGENT, 
    					"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
    				curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0); 
    				curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
    				curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2); 
    				curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data); 
    				result = curl_easy_perform(handle);
    			
    				if (result != CURLE_OK) {
    					// if errors have occured, tell us what is wrong with result
    					cout << "curl_easy_perform() failed: " << curl_easy_strerror(result) << endl;
    					return 1;
    				}
    			
    				stringstream sData;
    				sData.str(data.memory);
    				string sDate;
    				string sAdjustedClose;
    				double dAdjustedClose;
    				string line;
    				
    				map<string, double> historical_date_prices;
    				while (getline(sData, line)) {
    					size_t found = line.find('-');
    					if (found != std::string::npos) {
    						sDate = line.substr(0, line.find_first_of(','));
    						line.erase(line.find_last_of(','));
    				    	sAdjustedClose = line.substr(line.find_last_of(',') + 1);
    				    	dAdjustedClose = strtod(sAdjustedClose.c_str(), NULL);
    				    	historical_date_prices[sDate] = dAdjustedClose;
    					}
    				}
    				itr->SetAdjClosePrices(GetPrices(historical_date_prices));
    				itr->SetCumReturns(itr->CalcCumReturns());
    				vector<double> benchmarkReturns;
    				benchmarkReturns = CalcBenchmarkReturn(startDate, endDate, benchmark);
    				itr->SetAbnormReturns(itr->CalcAbnormReturns(benchmarkReturns));
    				tickersFullData.push_back(*itr);
    				free(data.memory);
    			    data.size = 0;
    			}
				
 			}
 		} else {
 			cout << "Curl not initialized. Cannot fetch prices." << endl;
 		    return -1;
 		}
 		
 		curl_easy_cleanup(handle);
		curl_global_cleanup();
		return 0;
	}
	
	int FetchBenchmarkPrices(string symbol, string start_date, string end_date, map<string, double>& historical_date_prices){
		// declaration of an object CURL 
		CURL* handle;
		CURLcode result;
		
		// set up the program environment that libcurl needs 
    	curl_global_init(CURL_GLOBAL_ALL);
    
		// curl_easy_init() returns a CURL easy handle 
 		handle = curl_easy_init();
 		
 		// if everything's all right with the easy handle...
 		if (handle)
 		{
 			string url_common = "https://eodhistoricaldata.com/api/eod/";
 			// You must replace this API token with yours 
 			string api_token = "656a140f854ac5.76574891";
 			
 			struct MemoryStruct data;
 			data.memory = NULL; 
 			data.size = 0;
 			string url_request = url_common + symbol + ".US?" + "from="
				+ start_date + "&to=" + end_date + "&api_token="
				+ api_token + "&period=d";
			
			curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());
			//adding a user agent
			curl_easy_setopt(handle, CURLOPT_USERAGENT, 
				"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0); 
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2); 
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data); 
			result = curl_easy_perform(handle);
			
			if (result != CURLE_OK) {
				// if errors have occured, tell us what is wrong with result
				cout << "curl_easy_perform() failed: " << curl_easy_strerror(result) << endl;
				return 1;
			}
			
			stringstream sData;
			sData.str(data.memory);
			string sDate;
			string sAdjustedClose;
			double dAdjustedClose;
			string line;
			while (getline(sData, line)) {
				size_t found = line.find('-');
				if (found != std::string::npos) {
					sDate = line.substr(0, line.find_first_of(','));
					line.erase(line.find_last_of(','));
				    sAdjustedClose = line.substr(line.find_last_of(',') + 1);
				    dAdjustedClose = strtod(sAdjustedClose.c_str(), NULL);
				    historical_date_prices[sDate] = dAdjustedClose;
				    // cout << 1 << endl;
				}
			}
			free(data.memory);
			data.size = 0;
 		} else {
 			cout << "Curl not initialized. Cannot fetch prices." << endl;
 		    return -1;
 		}
 		
 		// cleanup since you've used curl_easy_init 
 		curl_easy_cleanup(handle);
		// release resources acquired by curl_global_init() 
		curl_global_cleanup();
		return 0;
	}
    
    
    void populateSymbolVector(vector<Ticker>& tickers, const char* cIWB3000SymbolFile, map<string, Ticker>& miss, map<string, Ticker>& meet, map<string, Ticker>& exceed) {
        ifstream fin;
        fin.open(cIWB3000SymbolFile, ios::in);
        Ticker ticker;
        string line, symbol, date, end_period, estimate_string, reported_string, surprise_string, surprise_percent_string;
        // string benchmark = "IWV";
        double surprise_percent, estimate, reported, surprise;
        getline(fin, line);
        while (!fin.eof()) {
            getline(fin, line);
            stringstream sin(line);
            getline(sin, symbol, ',');
            getline(sin, date, ',');
            getline(sin, end_period, ',');
            getline(sin, estimate_string, ',');
            estimate = strtod(estimate_string.c_str(), NULL);
            getline(sin, reported_string, ',');
            reported = strtod(reported_string.c_str(), NULL);
            getline(sin, surprise_string, ',');
            surprise = strtod(surprise_string.c_str(), NULL);
            getline(sin, surprise_percent_string);
            surprise_percent = strtod(surprise_percent_string.c_str(), NULL);
    
            if (surprise_percent >= 10) {
                ticker.updateTicker(symbol, date, end_period, estimate, reported, surprise, surprise_percent, "Beat");
                tickers.push_back(ticker);
                exceed.insert({ticker.GetSymbol(), ticker});
            }
        
            if (surprise_percent >= 0 && surprise_percent < 10) {
                ticker.updateTicker(symbol, date, end_period, estimate, reported, surprise, surprise_percent, "Meet");
                tickers.push_back(ticker);
                meet.insert({ticker.GetSymbol(), ticker});
            }
            
            if (surprise_percent < 0) {
                ticker.updateTicker(symbol, date, end_period, estimate, reported, surprise, surprise_percent, "Miss");
                tickers.push_back(ticker);
                miss.insert({ticker.GetSymbol(), ticker});
            }
        }
    }
}