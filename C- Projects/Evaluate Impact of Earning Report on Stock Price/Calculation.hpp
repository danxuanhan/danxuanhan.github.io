//
//  Calculation.hpp
//  SimpleCurlMac
//
//  Created by apple on 2018/12/10.
//  Copyright © 2018 Song Tang. All rights reserved.
//

#ifndef Calculation_hpp
#define Calculation_hpp
#include <stdio.h>
#include <iostream>
#include <vector>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include "Data.hpp"
using namespace std;

//select 100 tickers randomly from tickers pool
vector<string> sampling(vector<string> stockticker);

vector<double> R(vector<double> stock_prices);

vector<double> AR(const vector<double> Ri, string start_date,
                  const vector<double> Rm, vector<string> m_price_date);

#endif /* Calculation_hpp */
