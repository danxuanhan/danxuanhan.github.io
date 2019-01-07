//
//  gnuplot.cpp
//  SimpleCurlMac
//
//  Created by apple on 2018/12/14.
//  Copyright © 2018 Song Tang. All rights reserved.
//

#include "gnuplot.hpp"

void plotResults(vector<double>BeatCAAR,vector<double>MeetCAAR,vector<double>MissCAAR)
{
    FILE *gnuplotPipe; // pipe to transfer data
    FILE *temp=fopen("data.temp","w"); // fopen: means to create file
    // Opens an interface that one can use to send commands as if they were typing into the gnuplot command line.
    gnuplotPipe = popen("/opt/local/bin/gnuplot","w");// open a pipe to wirte
    int i = BeatCAAR.size();
    
    for(int k=0; k<i; k++)
    {
        fprintf(temp,"%d %f %f %f \n",k-119, BeatCAAR[k], MeetCAAR[k], MissCAAR[k]);//Write the data to a temporary file
    }
    
    if (gnuplotPipe) // if pipe is not null
    {
        fprintf(gnuplotPipe, "set title 'CAAR for three groups'\n");//"set title 'Observable'\n
        fprintf(gnuplotPipe, "set xlabel 'Time'\n");
        fprintf(gnuplotPipe, "set ylabel 'CAAR Value' \n");
        fprintf(gnuplotPipe, "set xrange[-120:120] \n");// set the x axis range
        fprintf(gnuplotPipe, "set xtics 10 \n");// set the interval of x axis
        fprintf(gnuplotPipe, "plot '%s' using 1:2 title 'Beat' w l lw 2,'%s' using 1:3 title 'Meet' w l lw 2,'%s' using 1:4 title 'Miss' w l lw 2 \n", "data.temp", "data.temp","data.temp");// arguments has to be char*
        
        fflush(gnuplotPipe);
        //used to flush the pipe. why? ran into the problem that the plot doesn't appear until the calling program is closed. To get around this,  add a fflush(gnuplotPipe) after using fprintf to send it your final command.
        
        fclose(temp);
        fprintf(gnuplotPipe,"exit/n");
    }
    
    else
    {
        printf("gnuplot not found...");
    }
    
    pclose(gnuplotPipe);//close the pipe
    
}

