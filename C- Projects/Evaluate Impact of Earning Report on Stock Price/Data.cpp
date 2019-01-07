//
//  Data.cpp
//  SimpleCurlMac
//
//  Created by apple on 2018/12/10.
//  Copyright © 2018 Song Tang. All rights reserved.
//

#include "Data.hpp"
#include <math.h>

Stock::Stock(string startdate_, string enddate_,
             double Act_EPS_, double Est_EPS_, double est_persent_, string Group_)
{
    Act_EPS = Act_EPS_;
    Est_EPS = Est_EPS_;
    Group = Group_;
    est_persent = est_persent_;
    startdate = startdate_;
    enddate = enddate_;
    
}

void Stock::ShowPricingdata(string ticker_name)
{
    cout << "**************" + ticker_name + " Info**************" << endl;
    cout << "Stock Ticker: " << ticker_name << endl;
    cout << "Start date: " << GetStartDate() << endl;
    cout << "End date: " << GetEndDate() << endl;
    cout << "Estimated EPS: " << GetEstEPS() << endl;
    cout << "Actual EPS: " << GetActEPS() << endl;
    cout << "Group: " << GetGroup() << endl;
    cout << "Stock prices: " << endl;
    int size_v = Pricing_date.size();
    for(int i=0; i<size_v; i++)
    {cout << Pricing_date[i] + "  " << Price_data[i] << endl;}
    cout << "Return: "<< endl;
    Print(Return);
    cout << "AR: " << endl;
    Print(AR);
}


StocksInfo::StocksInfo(string EPSfile, string NoEPSfile)
{
    string ticker, startTime, endTime, group;
    double Act_EPS, Est_EPS, est_persent;
    
    ifstream NoEPSdata;
    NoEPSdata.open(NoEPSfile);
    while(NoEPSdata>>ticker){
        MissingEPSTickers.push_back(ticker);
    }
    
    ifstream BloombergGroup;
    BloombergGroup.open(EPSfile);
    
    vector<string> startdays;
    vector<string> enddays;
    while(BloombergGroup>>ticker>>startTime>>endTime>>Act_EPS>>Est_EPS){
        est_persent = (Act_EPS-Est_EPS)/Est_EPS;
        if (est_persent > 0.07){group = "beat";}
        else if(est_persent < 0.01){group = "miss";}
        else {group = "meet";}
        
        startdays.push_back(startTime);
        enddays.push_back(endTime);
        
        Stock singleStock(startTime, endTime, Act_EPS, Est_EPS, est_persent, group);
        AllStocksMap[ticker] = singleStock;
        
    }
    
    BloombergGroup.close();
    
    sort(startdays.begin(),startdays.end());
    sort(enddays.begin(), enddays.end());
    Stock benchmark(startdays[0],enddays.back(), 0.0, 0.0, 0.0, "null");
    IWB["IWB"] = benchmark;
    
}

void StocksInfo::CalculateRandAR()
{
    vector<double> IWB_return = R(IWB["IWB"].GetPrice());
    IWB["IWB"].SetReturn(IWB_return);
    
    for (map<string, Stock>::iterator itr = AllStocksMap.begin();
         itr != AllStocksMap.end(); itr++)
    {
        vector<double> R_vector = R((itr->second).GetPrice());
        (itr->second).SetReturn(R_vector);
        
        vector<double> AR_vector = AR(R_vector,(itr->second).GetStartDate(),
                                      IWB_return, IWB["IWB"].GetPricedate());
//        for(int i=0;i<AR_vector.size();i++){
//            if(AR_vector[i]>7){
//                cout << "*************out lier************" << endl;
//                cout << itr->first << endl;
//                cout << AR_vector[i] << endl;
//            }
//        }
        
        (itr->second).SetAR(AR_vector);
    }
}

vector<double> AAR(vector<string> tickers, int n_tradingdays_minus,
                   map<string, Stock> StocksMap)
{
    int v_size = tickers.size();
    vector<double> result(n_tradingdays_minus, 0.0);
    for (int i=0; i<n_tradingdays_minus; i++)
    {
        for (int j = 0; j< v_size; j++)
        {
            result[i] = (double(j)*result[i]+StocksMap[tickers[j]].GetAR()[i])/(double(j)+1.0);
        }
        
    }
    return result;
}

vector<double> CAAR(vector<double> AAR_vector)
{
    int n_tradingdays_minus = AAR_vector.size();
    vector<double> result;
    for (int i=1; i<=n_tradingdays_minus ; i++)
    {
        result.push_back(accumulate(AAR_vector.begin(), AAR_vector.begin()+i, 0.0));
    }
    
    return result;
}

Vector StocksInfo::CalculateMetrics(vector<string> tickerpool)
{
    Vector result;
    
    vector<string> seletions = sampling(tickerpool);
    vector<double> AAR_ = AAR(seletions, 240, AllStocksMap);
    vector<double> CAAR_ = CAAR(AAR_);
    result.push_back(AAR_);
    result.push_back(CAAR_);
    
    return result;
}


void StocksInfo::Sampling()
{
    
    vector<double> vec(240,0.0); Vector vec2(2, vec);
    Vector BeatRow = vec2, MeetRow = vec2, MissRow = vec2;
    
    for (int i=0; i<5;i++)
    {
        BeatRow = (1/(i+1.0))*(double(i)*BeatRow + CalculateMetrics(BeatTickers));
        MeetRow = (1/(i+1.0))*(double(i)*MeetRow + CalculateMetrics(MeetTickers));
        MissRow = (1/(i+1.0))*(double(i)*MissRow + CalculateMetrics(MissTickers));
        
    }
    Metrics = {BeatRow, MeetRow, MissRow};
}




