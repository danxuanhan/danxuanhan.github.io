//
//  LoadandRetrieveData.h
//  SimpleCurlMac
//
//  Created by apple on 2018/12/11.
//  Copyright © 2018 Song Tang. All rights reserved.
//

#ifndef LoadandRetrieveData_h
#define LoadandRetrieveData_h
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <locale>
#include <iomanip>
#include <map>
#include <algorithm>
#include "curl/curl.h"
#include "Data.hpp"

int GetYahooData(map<string, Stock>& RetrieveInputMap, vector<string>& NULL_tickers,
                 vector<string>& BeatTickers_, vector<string>& MeetTickers_,
                 vector<string>& MissTickers_, bool benchmark);

#endif /* LoadandRetrieveData_h */
