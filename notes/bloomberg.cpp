#include <stdlib.h>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <iostream>
#include <string>
#include <vector>
#include <string>

using namespace std;

/*
  Answer for bonus question. 
  To handle a large volume of ticks, we need to change the design and use distributed system.
  Instead of having one process/host handling all the incoming data, we can have a 
  master to dispatch the ticks to multiple slaves. Each slave is responsible for
  a group of tickers. For example, all the ticks for GOLD and OIL can be dispatched to
  slave 1, and all the ticks for EUR and AUD can be dispatched to slave 2, etc.
  The scheme of assigning the tickers to a slave can be varied. The purpose is to keep 
  all the slaves with balanced load of ticks. 

  For queries like allBanksForTicker and bestReturnsForTickerInTimeRange, the master only
  need to query one slave because the query only involves one ticker.

  For queries like allTickersForAssest, the master will query all the slaves and combine
  their responses into one response. This is easy to do because there are no duplicate
  tickers among the responses. 

  Multiple masters might be used to reduce load on a master and avoid single point of failure.
  This may require some changes in the software architecture. 

  We may also use Hadoop/MapReduce if needed. 
 */

// A class for storing tick time. 
class TickTime {
public:
    TickTime(const string & t );
    ~TickTime(){};

    int getHour() const {return hour;}
    int getMinute() const {return minute;}
    int getSecond() const {return second;}
private:
    int hour;
    int minute;
    int second;
};

TickTime::TickTime(const string&t) {
    // Need throw exception if t is not in HH:MM:SS format or with good time values.
    hour = atoi(t.substr(0, 2).c_str());
    minute = atoi(t.substr(3, 2).c_str());
    second = atoi(t.substr(6, 2).c_str());
}

// A class for storing tick info. 
class Tick {
public:
    Tick(const string& vtime, const string& vticker, const string& vasset,
         const string& vbank, float vbid, float vask)
        : timeStr(vtime), time(TickTime(vtime)), ticker(vticker), asset(vasset), bank(vbank), bid(vbid), ask(vask) {}
    
    ~Tick(){}

    const string& timeString() const {return timeStr;}
    const TickTime & getTime() const {return time;}
    const string& getTicker() const {return ticker;}
    const string& getBank() const {return bank;}
    float getBid() const {return bid;}
    float getAsk() const {return ask;}
private:
    string timeStr; // store time in string format
    TickTime time; // store time in TickTime format 
    string ticker;
    string asset;
    string bank;
    float bid;
    float ask;
};

// comparison for TickTime. 
inline bool operator < (const TickTime& time1, const TickTime & time2) {
    if (time1.getHour() < time2.getHour())
        return true;
    else if (time1.getHour() == time2.getHour()) {
        if (time1.getMinute() < time2.getMinute())
            return true;
        else if (time1.getMinute() == time2.getMinute())
            return time1.getSecond() < time2.getSecond();
        else
            return false;
    }
    else
        return false;
}

// comparison for TickTime. 
inline bool operator == (const TickTime& time1, const TickTime & time2) {
    return time1.getHour() == time2.getHour() &&time1.getMinute() == time2.getMinute() &&  time1.getSecond() == time2.getSecond();
}

// comparison for TickTime. 
inline bool operator <= (const TickTime& time1, const TickTime & time2) {
    return time1<time2 || time1 == time2;
}

// The main class to process market data and answer queries. 
class MarketDataSystem {
public:
    MarketDataSystem() {}
    ~MarketDataSystem() {}
    
    // Read input from stdin. Return false if there is an error. Otherwise, return true.
    bool readInput();

    // handle queries. Return false if there is an error. Otherwise, return true.
    bool handleQueries();

private:
    // parse a line with a tick and store in allTicks and hash tables.
    // Return false if there is a parsing error. Otherwise, return true. 
    bool processTick(const string & tickStr);

    // handle the query of allTickersForAsset. 
    void handleQueryOfAllTickersForAsset(const string& asset);

    // handle the query of allBanksForTicker
    void handleQueryOfAllBanksForTicker(const string & ticker);

    // handle the query of bestReturns
    void handleQueryOfBestReturns(const string& ticker, const string & start, const string &end);

    // Store all the input ticks. 
    vector<Tick> allTicks;

    // Store all the queries. 
    vector<string> allQueries;

    // Use asset as the key and a list of its ticks as value. The ticks are represented
    // by their indices in the vector allTicks. 
    unordered_map<string, vector<int>> assetTicks;

    // Use ticker as the key and a list of its ticks as value. The ticks are represented
    // by their indices in the vector allTicks. 
    unordered_map<string, vector<int>> tickerTicks;
};

int main () {
    MarketDataSystem mds;
    if (!mds.readInput())
        return 1;
    
    if (!mds.handleQueries()) {
        cout << "Query error" <<endl;
        return 2;
    }
    
    return 0;
}

bool MarketDataSystem::processTick(const string & tickStr) {
    if (tickStr.size() < 13) // not enough length for time. 
        return false;

    string time(tickStr.substr(5, 8));

    // find ticker
    if (tickStr[13] != '|')
        return false;
    size_t pos1 = 14;
    size_t pos2 = tickStr.find('|', pos1);
    if (pos2 == string::npos || pos1 == pos2)
        return false;
    string ticker = tickStr.substr(pos1, pos2-pos1);

    // find asset
    pos1 = pos2+1;
    pos2 = tickStr.find('|', pos1);
    if (pos2 == string::npos || pos1 == pos2)
        return false;
    string asset = tickStr.substr(pos1, pos2-pos1);

    // find bank
    pos1 = pos2+1;
    pos2 = tickStr.find('|', pos1);
    if (pos2 == string::npos || pos1 == pos2)
        return false;
    string bank = tickStr.substr(pos1, pos2-pos1);

    // find bid
    pos1 = pos2+1;
    pos2 = tickStr.find('|', pos1);
    if (pos2 == string::npos || pos1 == pos2)
        return false;
    float bid = atof(tickStr.substr(pos1, pos2-pos1).c_str());

    // find ask
    pos1 = pos2+1;
    if (pos1 >=tickStr.size() || tickStr.find('|', pos1) != string::npos)
        return false;
    float ask = atof(tickStr.substr(pos1, string::npos).c_str());

    allTicks.push_back(Tick(time, ticker, asset, bank, bid, ask));
    int index = allTicks.size()-1;

    assetTicks[asset].push_back(index);
    tickerTicks[ticker].push_back(index);
    
    return true;
}

void MarketDataSystem::handleQueryOfAllTickersForAsset(const string& asset) {
    vector<int> & ticks = assetTicks[asset];
    unordered_set<string> visited; // Use a visited hash table to remove duplicate

    cout << "RESULT: allTickersForAsset(\"" << asset << "\") - ";
    string result;
    for (const auto & t : ticks) {
        const string & ticker = allTicks[t].getTicker();
        if (visited.find(ticker) != visited.end())
            continue;
        else {
            result += ticker +",";
            visited.insert(ticker);
        }
    }
    
    result.erase(result.end()-1, result.end());
    cout<<result<<endl;
}

void MarketDataSystem::handleQueryOfAllBanksForTicker(const string & ticker) {
    vector<int> & ticks = tickerTicks[ticker];
    unordered_set<string> visited; // Use a visited hash table to remove duplicate


    cout << "RESULT: allBanksForTicker(\"" << ticker << "\") - ";
    string result;
    for (const auto & t : ticks) {
        const string & bank = allTicks[t].getBank();
        if (visited.find(bank) != visited.end())
            continue;
        else {
            result += bank +",";
            visited.insert(bank);
        }
    }
    
    result.erase(result.end()-1, result.end());
    cout<<result<<endl;
}

void MarketDataSystem::handleQueryOfBestReturns(const string& ticker, const string & start, const string &end) {
    TickTime startTime(start);
    TickTime endTime(end);
    
    // May use binary search for startTime and endTime to improve performance.
    // Use the easier linear search for now :)
    // The ticks in the vector are already sorted by time as in the input order.
    vector<int> & ticks = tickerTicks[ticker];
    int i;
    for (i=0; i<ticks.size(); ++i) {
        if (!(allTicks[ticks[i]].getTime()<startTime))
            break;
    }

    // error checking
    if (i==ticks.size()) {
        cout << "RESULT: bestReturnsForTickerInTimeRange(\"" << ticker << "\",\""<<start<<"\",\""<<end<<"\") - No result!" <<endl;
        return;
    }
    
    int startTick = i;

    int minAskIndex = startTick; // keep the index of the minimum ask price. 
    int bestStart = startTick; // start tick for the best return
    int bestEnd = startTick; // end tick for the best return
    float minAsk = allTicks[ticks[bestStart]].getAsk(); // minimum ask price so far

    // best return so far
    float bestReturn = allTicks[ticks[bestEnd]].getBid() - allTicks[ticks[bestStart]].getAsk(); 
    for (i=startTick; i<ticks.size() && allTicks[ticks[i]].getTime()<=endTime; ++i) {
        float bid = allTicks[ticks[i]].getBid(); // sell price
        float ask = allTicks[ticks[i]].getAsk(); // buy price

        if (allTicks[ticks[i]].getAsk() < minAsk) {
            minAsk = allTicks[ticks[i]].getAsk();
            minAskIndex = i;
        }

        float ret = allTicks[ticks[i]].getBid() - minAsk;
        if (ret > bestReturn) {
            bestReturn = ret;
            bestStart = minAskIndex;
            bestEnd = i;
        }
    }

    if (bestReturn>0)
        cout << "RESULT: bestReturnsForTickerInTimeRange(\"" << ticker << "\",\""<<start<<"\",\""<<end<<"\") - BUY:"<<allTicks[ticks[bestStart]].timeString()<<"|"<<allTicks[ticks[bestStart]].getAsk()<<"|"<<allTicks[ticks[bestStart]].getBank()<<"; SELL:"<<allTicks[ticks[bestEnd]].timeString()<<"|"<<allTicks[ticks[bestEnd]].getBid()<<"|"<<allTicks[ticks[bestEnd]].getBank()<<endl;
    else
        cout << "RESULT: bestReturnsForTickerInTimeRange(\"" << ticker << "\",\""<<start<<"\",\""<<end<<"\") - No possible profit"<<endl;
}

bool MarketDataSystem::readInput() {
    vector<string> errorInput;
    bool done=false;
    while(!done){
        char in[256];
        cin.getline(in, 256);
        string input(in);
        if (input.find("TICK", 0, 4) != string::npos) {
            if (!processTick(input))
                errorInput.push_back(input);
        } else if (input.find("QUERY", 0, 5) != string::npos) {
            allQueries.push_back(input);
        }
        else
            done = true;
    }

    if (errorInput.empty())
        return true;
    else {
        cout << "Tick parsing ERROR: The following ticks don't match the expected format:" <<endl;
        for (int i=0; i<errorInput.size(); ++i)
            cout<<errorInput[i]<<endl;
        return false;
    }
}

/*
// For testing purpose
bool MarketDataSystem::readInput() {
    vector<string> inputLines;
    inputLines.push_back("TICK:09:03:50|EUR|FX|BANK1|1.2622|1.2623");
    inputLines.push_back("TICK:09:03:59|GOLD|COMMODITY|BANK3|1213.63|1214.01");
    inputLines.push_back("TICK:09:04:00|OIL|COMMODITY|BANK4|89.01|90.45");
    inputLines.push_back("TICK:09:04:05|EUR|FX|BANK5|1.2668|1.2669");
    inputLines.push_back("TICK:09:04:05|GBP|FX|BANK2|1.6185|1.6186");
    inputLines.push_back("TICK:09:04:05|GOLD|COMMODITY|BANK1|1214.32|1214.83");

    inputLines.push_back("TICK:10:08:05|OIL|COMMODITY|BANK4|87.25|88.7");
    inputLines.push_back("TICK:11:00:01|OIL|COMMODITY|BANK1|88.7|89.08");
    inputLines.push_back("TICK:11:15:01|GOLD|COMMODITY|BANK5|1191.25|1191.59");
    inputLines.push_back("TICK:11:30:01|AUD|FX|BANK5|0.8738|0.8738");
    inputLines.push_back("TICK:11:45:01|GOLD|COMMODITY|BANK6|1207.19|1207.4");
    inputLines.push_back("TICK:12:00:01|AUD|FX|BANK1|0.8803|0.8804");

    inputLines.push_back("QUERY:allTickersForAsset(\"FX\")");
    inputLines.push_back("QUERY:allBanksForTicker(\"EUR\")");
    inputLines.push_back("QUERY:bestReturnsForTickerInTimeRange(\"GOLD\",\"11:00:00\",\"14:00:00\")");
    
    for (int i=0; i<inputLines.size(); ++i) {
        string &input=inputLines[i];

        if (input.find("TICK", 0, 4) != string::npos) {
            if (!processTick(input))
                return false;
        } else if (input.find("QUERY", 0, 5) != string::npos) {
            allQueries.push_back(input);
        }
        else {
            return false;
        }
    }

    return true;
}
*/

bool MarketDataSystem::handleQueries()
{
    for (int i=0;i<allQueries.size(); ++i) {
        string & query = allQueries[i];
        if (query.find("allTickersForAsset", 6) != string::npos) {
            if (query.size() <= 25 || query[25] != '"')
                return false;
            size_t pos1= 26;
            size_t pos2 = query.find('"', pos1);
            if (pos2 == string::npos)
                return false;
            string asset = query.substr(pos1, pos2-pos1);
            handleQueryOfAllTickersForAsset(asset);
        } else if (query.find("allBanksForTicker", 6)!=string::npos) {
            if (query.size()<=24 || query[24] != '"')
                return false;
            size_t pos1= 25;
            size_t pos2 = query.find('"', pos1);
            if (pos2 == string::npos)
                return false;
            string ticker = query.substr(pos1, pos2-pos1);
            handleQueryOfAllBanksForTicker(ticker);
        } else if (query.find("bestReturnsForTickerInTimeRange", 6)!=string::npos) {
            if (query.size() <= 38 || query[38] != '"')
                return false;
            size_t pos1= 39;
            size_t pos2 = query.find('"', pos1);

            if (pos2 == string::npos)
                return false;
            string ticker = query.substr(pos1, pos2-pos1);

            // Find time
            pos1 = pos2+3;
            pos2 = pos1+8;
            if (query.size()<=pos2 || query[pos2] != '"')
                return false;
            string start = query.substr(pos1, pos2-pos1);
            pos1 = pos2+3;
            pos2 = pos1+8;
            if (query.size()<=pos2 || query[pos2] != '"')
                return false;
            string end = query.substr(pos1, pos2-pos1);
            handleQueryOfBestReturns(ticker, start, end);
        }
    }
    
    return true;
}
