//
//  menu.cpp
//  SimpleCurlMac
//
//  Created by apple on 2018/12/11.
//  Copyright © 2018 Song Tang. All rights reserved.
//

#include <stdio.h>
#include "Data.hpp"
#include "LoadandRetrieveData.h"
#include "ShowData.hpp"
#include "gnuplot.hpp"
#include <exception>

int main()
{
    int number;
    string enterstring, enterstring2; // used to enter something
    vector<string> NULL_tickers;
    
    StocksInfo Russell1000("BloombergData.txt", "NoEPSTickers.txt");
    
    do
    {
        cout<< "Main Menu"<<endl;
        cout << "Please enter a number from 1 to 5\n";
        cout << "1 - Retrieve historical price data\n";
        cout << "2 - Pull information for one stock from one group\n";
        cout << "3 - Show AAR or CAAR for one group\n";
        cout << "4 - Show graph of CAAR\n";
        cout << "5 - Quit the program\n";
        cin >> number;
        
        switch (number)
        {
            case 1:
            {
                time_t now = time(0);
                string dt = ctime(&now);
                
                GetYahooData(Russell1000.AllStocksMap,NULL_tickers,Russell1000.BeatTickers,
                             Russell1000.MeetTickers,Russell1000.MissTickers,false);
                GetYahooData(Russell1000.IWB, NULL_tickers, Russell1000.BeatTickers,
                             Russell1000.MeetTickers, Russell1000.MissTickers, true);
                Russell1000.CalculateRandAR();
                Russell1000.Sampling();
                PrintToExcel(Russell1000.Metrics);
                
                time_t then = time(0);
                string dt2 = ctime(&then);
                cout << "start time: " << dt <<endl;
                cout << "end time: " << dt2 <<endl;
                
                break;
            }
            case 2:
            {
                cout << "Enter the stock ticker: " << endl;
                cin >> enterstring;

                auto itr_stockinfo = Russell1000.AllStocksMap.find(enterstring);
                vector<string>::iterator itr_null = find(NULL_tickers.begin(),
                                                         NULL_tickers.end(),
                                                         enterstring);
                vector<string>::iterator itr_noeps = find(
                                                          Russell1000.MissingEPSTickers.begin(),
                                                          Russell1000.MissingEPSTickers.end(),
                                                          enterstring);
                if(itr_null != NULL_tickers.end())
                {
                    cout << "Yahoo Finance couldn't get the data for corresponding trading days. " << endl;
                }
                else if( itr_noeps != Russell1000.MissingEPSTickers.end())
                {
                    cout << "Doesn't have EPS for 2017 2nd quarter in Bloomberg. " << endl;
                }
                else if(itr_stockinfo != Russell1000.AllStocksMap.end())
                {
                    Stock pickedstock = Russell1000.AllStocksMap[enterstring];
                    (itr_stockinfo->second).ShowPricingdata(enterstring);
                }
                else
                {
                    cout << "Given ticker is not in Russell1000's constituents. We have ticked BRK-A, ARD, VVV and W since we think they are outliers."
                         << endl;
                }

                break;

            }
            case 3:
            {
                
                cout << "Enter the group name: Beat, Meet or Miss" << endl; cin >> enterstring;
                cout << "Print AAR or CAAR？ "<<endl; cin >> enterstring2;
                if (enterstring=="Beat")
                {
                    if (enterstring2=="AAR")
                    {Print(Russell1000.Metrics[0][0]);}
                    else if(enterstring2=="CAAR")
                    {Print(Russell1000.Metrics[0][1]);}
                    else {cout << "Please choose between CAAR and AAR" << endl;}
                }
                else if (enterstring=="Meet")
                {
                    if (enterstring2=="AAR")
                    {Print(Russell1000.Metrics[1][0]);}
                    else if(enterstring2=="CAAR")
                    {Print(Russell1000.Metrics[1][1]);}
                    else {cout << "Please choose between CAAR and AAR" << endl;}
                }
                else if (enterstring=="Miss")
                {
                    if (enterstring2=="AAR")
                    {Print(Russell1000.Metrics[2][0]);}
                    else if(enterstring2=="CAAR")
                    {Print(Russell1000.Metrics[2][1]);}
                    else {cout << "Please choose between CAAR and AAR" << endl;}
                }
                
                break;
                
            }
            case 4:
            {
                cout<<"Now let's draw the graph"<< endl;
                plotResults(Russell1000.Metrics[0][1], Russell1000.Metrics[1][1],
                            Russell1000.Metrics[2][1]);
                break;
            }
            case 5:
            {
                cout<<"End of Program."<<endl;
                break;
            }
            default:
            {
                cout<<"Not a valid number"<<endl;
                cout<<"Please choose from 1 to 5"<<endl;
                break;
            }
        }
        
    }while (number !=5);

    
    return 0;
}




