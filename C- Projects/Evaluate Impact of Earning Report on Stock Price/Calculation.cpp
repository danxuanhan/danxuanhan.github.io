//
//  Calculation.cpp
//  SimpleCurlMac
//
//  Created by apple on 2018/12/10.
//  Copyright © 2018 Song Tang. All rights reserved.
//

#include "Calculation.hpp"
#include <math.h>
using namespace std;

vector<string> sampling(vector<string> stockticker)
{
    vector<string> returnticker;
    srand(time(NULL));
    for (int i = 0; i < 100; i++)      //use 3 for testing, change to 100 for project
    {
        int c = rand() % stockticker.size();          //get a random index
        returnticker.push_back(stockticker[c]);       //copy the ticker to the new vector
        stockticker.erase(stockticker.begin() + c);   //delete the ticker in the original vector
    }
    return returnticker;
}

vector<double> R(vector<double> stock_prices){
    int vector_size = stock_prices.size();
    vector<double> return_vector(vector_size-1);
    for (int i=1; i <= vector_size; i++){
        if(stock_prices[i-1]==0)
        {return_vector[i-1]=0.0;}
        else
        {return_vector[i-1] = (stock_prices[i]-stock_prices[i-1])/stock_prices[i-1];}
    }
    return return_vector;
}

vector<double> operator-(const vector<double> vector1, const vector<double> vector2)
{
    int size = vector1.size();
    vector<double> return_vec;
    for (int i=0;i<size;i++){
        return_vec.push_back(vector1[i] - vector2[i]);
    }
    return return_vec;
}

// find subset of Rm corresponding to Ri and then, return vector of AR
vector<double> AR(const vector<double> Ri, string start_date,
                  const vector<double> Rm, vector<string> m_price_date)
{
    ptrdiff_t pos = distance(m_price_date.begin(),
                             find(m_price_date.begin(), m_price_date.end(), start_date));
    
    vector<double> new_Rm(Rm.begin()+pos,Rm.begin()+pos+240);
    
    vector<double> AR_return = Ri - new_Rm;
    
    return AR_return;
}


