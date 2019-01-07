//
//  Data.hpp
//  SimpleCurlMac
//
//  Created by apple on 2018/12/10.
//  Copyright © 2018 Song Tang. All rights reserved.
//

#ifndef Data_hpp
#define Data_hpp

#include <stdio.h>
#include <fstream>
#include <string>
#include <map>
#include "Calculation.hpp"
#include "OperatorOverloading.hpp"
#include "ShowData.hpp"
typedef vector<vector<double>> Vector;
typedef vector<Vector> matrix;


class Stock
{
private:
    string startdate;
    string enddate;
    double Act_EPS;
    double Est_EPS;
    vector<double> Price_data;
    vector<string> Pricing_date;
    double est_persent; // Percent of Estimated EPS compared to Actual EPS
    string Group;
    vector<double> Return;
    vector<double> AR;
    
public:
    Stock(){}
    Stock(string startdate_, string enddate_,
          double Act_EPS_, double Est_EPS_, double est_persent_, string Group_);
    string GetStartDate(){return startdate;}
    string GetEndDate(){return enddate;}
    string GetGroup(){return Group;}
    double GetEstEPS(){return Est_EPS;}
    double GetActEPS(){return Act_EPS;}
    vector<double> GetPrice(){return Price_data;}
    vector<string> GetPricedate(){return Pricing_date;}
    vector<double> GetReturn(){return Return;}
    vector<double> GetAR(){return AR;}
    void SetPricingdata(vector<double> Price_data_)
    {Price_data = Price_data_;}
    void SetPricingdate(vector<string> Pricing_date_)
    {Pricing_date = Pricing_date_;}
    void ShowPricingdata(string ticker_name);
    void SetReturn(vector<double> Return_){Return = Return_;}
    void SetAR(vector<double> AR_){AR = AR_;}
    
};


class StocksInfo
{
public:
    map<string, Stock> AllStocksMap; // stock pool's info
    map<string, Stock> IWB; // bechmark info
    matrix Metrics; // AAR and CAAR final info matrix
    
    vector<string> MissingEPSTickers;
    
    vector<string> MeetTickers;
    vector<string> MissTickers;
    vector<string> BeatTickers;
    
    
    StocksInfo(string EPSfile, string NoEPSfile);
    
    void CalculateRandAR();
    
    Stock GetInfoOneStock(string ticker){return AllStocksMap[ticker];}
    
    Vector CalculateMetrics(vector<string> tickerpool);
    
    void Sampling();
};




#endif /* Data_hpp */
