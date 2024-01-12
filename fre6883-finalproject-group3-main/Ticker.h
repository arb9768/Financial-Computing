#pragma once
#include <algorithm>  // Add this header for std::sort
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <cstring>
#include <cmath>
#include <string> 
#include <locale>
#include <iomanip>
#include "curl/curl.h"
#include "Matrix.h"

using namespace std;

namespace fre {
    
    class Ticker {
        private:
            string symbol;
            string date;
            string end_period;
            double estimate;
            double reported;
            double surprise;
            double surprise_percent;
            string group;
            Vector adjClosePrices;
            Vector returns;
            Vector cumReturns;
            Vector abnormReturns;
        
        public:
            Ticker() {}
            Ticker(string symbol_, string date_, string end_period_, double estimate_, 
            double reported_, double surprise_, double surprise_percent_, string group_): 
            symbol(symbol_), date(date_), end_period(end_period_), estimate(estimate_), 
            reported(reported_), surprise(surprise_), surprise_percent(surprise_percent_), group(group_) {}
            
            void updateTicker(string symbol_, string date_, string end_period_, double estimate_,
            double reported_, double surprise_, double surprise_percent_, string group_){
                symbol = symbol_; date = date_; end_period = end_period_; estimate = estimate_;
                reported = reported_; surprise = surprise_; surprise_percent = surprise_percent_; group = group_;
            }
            string GetSymbol() const {return symbol;}
            string GetDate() const {return date;}
            string GetEndPeriod() const {return end_period;}
            double GetEstimate() const {return estimate;}
            double GetReported() const {return reported;}
            double GetSurprise() const {return surprise;}
            double GetSurprisePercent() const {return surprise_percent;}
            string GetGroup() const {return group;}
            Vector GetAdjClosePrices() const {return adjClosePrices;}
            Vector GetReturns() const {return returns;}
            Vector GetCumReturns() const {return cumReturns;}
            Vector GetAbnormReturns() const {return abnormReturns;}
            
            void SetAdjClosePrices(Vector adjClosePrices_){adjClosePrices = adjClosePrices_;}
            Vector CalcReturns();
            void SetReturns(Vector returns_){returns = returns_;}
            Vector CalcCumReturns();
            void SetCumReturns(Vector cumReturns_){cumReturns = cumReturns_;}
            Vector CalcAbnormReturns(Vector& benchmarkReturns);
            void SetAbnormReturns(Vector abnormReturns_){abnormReturns = abnormReturns_;}
            
    };
}