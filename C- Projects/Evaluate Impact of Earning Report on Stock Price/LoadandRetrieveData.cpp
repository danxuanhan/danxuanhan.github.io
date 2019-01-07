#include "SimpleCurlMac/Data.hpp"
#include "SimpleCurlMac/LoadandRetrieveData.h"

using namespace std;

vector<string> split(const string& s, char delimiter)
{
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}


size_t write_data(void *ptr, int size, int nmemb, FILE *stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}

struct MemoryStruct {
	char *memory;
	size_t size;
};

void *myrealloc(void *ptr, size_t size)
{
	/* There might be a realloc() out there that doesn't like reallocing
	NULL pointers, so we take care of it here */
	if (ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}


size_t write_data2(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)data;

	mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory) {
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
	}
	return realsize;
}

string getTimeinSeconds(string Time)
{
	std::tm t = {0};
    std::istringstream ssTime(Time);
	char time[100];
	memset(time, 0, 100);
    if (ssTime >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S"))
    {
//        cout << std::put_time(&t, "%c %Z") << "\n"
//                  << std::mktime(&t) << "\n";
		sprintf (time, "%ld", mktime(&t));
		return string(time);
    }
    else
    {
        cout << "Parse failed\n";
		return "";
    }
}


//input a map, first column is vector<string> startTimeList, second column is
//vector<string> endTimeList, third column is vector<string> symbolList
int GetYahooData(map<string, Stock>& RetrieveInputMap, vector<string>& NULL_tickers,
                 vector<string>& BeatTickers_, vector<string>& MeetTickers_,
                 vector<string>& MissTickers_, bool benchmark)
{
    map<string, Stock>::iterator itr = RetrieveInputMap.begin();
    
    struct MemoryStruct data;
    data.memory = NULL;
    data.size = 0;
    
    //file pointer to create file that store the data
    FILE *fp;
    
    //name of files
    const char outfilename[FILENAME_MAX] = "Output.txt";
//    const char resultfilename[FILENAME_MAX] = "Results.txt";
    
    /* declaration of an object CURL */
    CURL *handle;
    
    /* result of the whole process */
    CURLcode result;
    
    /* the first functions */
    /* set up the program environment that libcurl needs */
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* curl_easy_init() returns a CURL easy handle that you're gonna reuse in other easy functions*/
    handle = curl_easy_init();
    
    if (handle)
    {
        char* icon = ","; // used in the last step to split the line in data
        
        string sCookies, sCrumb;
        if (sCookies.length() == 0 || sCrumb.length() == 0)
        {
            fp = fopen(outfilename, "w");
            //curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history?p=AMZN");
            curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
            
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
            curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
            curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");
            
            curl_easy_setopt(handle, CURLOPT_ENCODING, "");
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
            result = curl_easy_perform(handle);
            fclose(fp);
            
            /* Check for errors */
            if (result != CURLE_OK)
            {
                /* if errors have occurred, tell us what is wrong with 'result'*/
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }
            
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
            
            /* perform, then store the expected code in 'result'*/
            result = curl_easy_perform(handle);
            
            /* Check for errors */
            if (result != CURLE_OK)
            {
                /* if errors have occured, tell us what is wrong with 'result'*/
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }
            
            char cKey[] = "CrumbStore\":{\"crumb\":\"";
            char *ptr1 = strstr(data.memory, cKey);
            char *ptr2 = ptr1 + strlen(cKey);
            char *ptr3 = strstr(ptr2, "\"}");
            if ( ptr3 != NULL )
                *ptr3 = NULL;
            
            sCrumb = ptr2;
            
            fp = fopen("cookies.txt", "r");
            char cCookies[100];
            if (fp) {
                while (fscanf(fp, "%s", cCookies) != EOF);
                fclose(fp);
            }
            else
                cerr << "cookies.txt does not exists" << endl;
            
            sCookies = cCookies;
//            free(data.memory);
//            data.memory = NULL;
//            data.size= 0;
        }
        
        
        string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
        string urlB = "?period1=";
        string urlC = "&period2=";
        string urlD = "&interval=1d&events=history&crumb=";
        
        const char * cookies = sCookies.c_str();
        curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
        
        while (true)
        {
            free(data.memory);
            data.memory = NULL;
            data.size= 0;
            
            if (itr == RetrieveInputMap.end())
                break;
            
            string symbol = itr->first;
            string startTime = getTimeinSeconds((itr->second).GetStartDate());
            string endTime = getTimeinSeconds((itr->second).GetEndDate());
            
            string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;
            
            const char * cURL = url.c_str();
            curl_easy_setopt(handle, CURLOPT_URL, cURL);
//            fp = fopen(resultfilename, "ab");
//            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
//            curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
            result = curl_easy_perform(handle);
//            fclose(fp);
            
            /* Check for errors */
            if (result != CURLE_OK)
            {
                /* if errors have occurred, tell us what is wrong with 'result'*/
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
            result = curl_easy_perform(handle);
            
            /* Check for errors */
            if (result != CURLE_OK)
            {
                /* if errors have occurred, tell us what is wrong with 'result'*/
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }
            stringstream sData;
            sData.str(data.memory);
            string line;
            
            vector<string> Pricingdate;
            vector<double> Pricingdata;
            
            for (; getline(sData, line); ){
                
                size_t found = line.find("error");
                if (found!=string::npos){
                    NULL_tickers.push_back(itr->first);
                    break;
                }
                
                vector<string> result = split(line, *icon);
                
                if (result[5]=="null"){
                    NULL_tickers.push_back(itr->first);
                    break;
                }
                else if (result[0]!="Date")
                {
                  
                    Pricingdata.push_back(stod(result[5]));
                    Pricingdate.push_back(result[0]);
                    
                }
            }
            
            if (Pricingdata.size()<241) {NULL_tickers.push_back(itr->first);}
            
            (itr->second).SetPricingdata(Pricingdata);
            (itr->second).SetPricingdate(Pricingdate);
            
            itr++;
        
        }
        
        int v_size = NULL_tickers.size();
        for (int i=0; i<v_size; i++){
            RetrieveInputMap.erase(NULL_tickers[i]);
        }
        
        
        if(!benchmark){
            for(map<string, Stock>::iterator itr2 = RetrieveInputMap.begin();
                itr2 != RetrieveInputMap.end(); itr2++)
            {
                if ((itr2->second).GetGroup()=="beat")
                {BeatTickers_.push_back(itr2->first);}
                else if ((itr2->second).GetGroup()=="meet")
                {MeetTickers_.push_back(itr2->first);}
                else{MissTickers_.push_back(itr2->first);}
            }
        }
        
        free(data.memory);
        data.size= 0;
        
    }
    else
    {
        fprintf(stderr, "Curl init failed!\n");
        return 1;
    }
    
    
    /* cleanup since you've used curl_easy_init */
    curl_easy_cleanup(handle);
    
    /* this function releases resources acquired by curl_global_init() */
    curl_global_cleanup();
    
    return 0;
}





