/**
* DataGenerator.cpp
* Generate data cases
*
* 1 Connector
* BondTradeBookingServiceConnector - Read data from "trades.txt"
*
* 1 Listener
* BondTradeBookingServiceListener - BondTradeBookingService listen from BondExecutionService
*
* @Yunze Sun
*/

#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <fstream>
#include <random>
#include "utility.h"

using namespace std;

// Generate trade/inquiry ID
string GenerateRandomId(int length, mt19937 gen)
{
    std::uniform_int_distribution<> U(0, 9);
    string id = "";
    for (int i=0; i<length; i++) id += to_string(U(gen))[0];
    return id;
}

// Generate prices and orderbooks
void genOrderBook(const vector<string>& products, const string& _priceFileName="price.txt", 
    const string& _bookFileName="marketdata.txt", const long T= 1'000)
{
    std::ofstream os_price(_priceFileName);
    std::ofstream os_book(_bookFileName);
    long long t = 0;

    // price file format: Timestamp, CUSIP, Bid, Ask
    os_price << "Timestamp,CUSIP,Bid,Ask" << endl;

    // orderbook file format: Timestamp, CUSIP, Bid1, BidSize1, Ask1, AskSize1, Bid2, BidSize2, Ask2, AskSize2, Bid3, BidSize3, Ask3, AskSize3, Bid4, BidSize4, Ask4, AskSize4, Bid5, BidSize5, Ask5, AskSize5
    os_book << "Timestamp,CUSIP,Bid1,BidSize1,Ask1,AskSize1,Bid2,BidSize2,Ask2,AskSize2,Bid3,BidSize3,Ask3,AskSize3,Bid4,BidSize4,Ask4,AskSize4,Bid5,BidSize5,Ask5,AskSize5" << endl;

    double midPrice = 99.00;
    bool priceIncreasing = true;
    bool spreadIncreasing = true;
    double ticksize = 1.0 / 256.0;
    double spread = 1.0 / 128.0;

    for (long t = 0; t < T; ++t) {

        for (const auto& product : products)
        {
            // generate price data
            double myBidPrice = midPrice - spread / 2.0;
            double myOfferPrice = midPrice + spread / 2.0;

            os_price << setw(12) << std::setfill('0') << t << "," << product << ","
                << Price2Str(myBidPrice) << "," << Price2Str(myOfferPrice) << endl;

            // generate order book data
            os_book << setw(12) << std::setfill('0') << t << "," << product;
            double bidPrice = midPrice - spread / 2.0;
            double askPrice = midPrice + spread / 2.0;
            for (int level = 1; level <= 5; ++level) {
                bidPrice -= ticksize;
                askPrice += ticksize;
                long size = level * 10'000'000;
                os_book << "," << Price2Str(bidPrice) << "," << size << "," << Price2Str(askPrice) << "," << size;
            }
            os_book << endl;
        }

        // oscillate mid price
        if (priceIncreasing) {
            midPrice += 1.0 / 256.0;
            if (midPrice >= 101.0) {
                priceIncreasing = false;
            }
        }
        else {
            midPrice -= 1.0 / 256.0;
            if (midPrice <= 99.0) {
                priceIncreasing = true;
            }
        }

        // oscillate book_spread
        if (spreadIncreasing) {
            spread += 1.0 / 128.0;
            if (spread >= 1.0 / 32.0) {
                spreadIncreasing = false;
            }
        }
        else {
            spread -= 1.0 / 128.0;
            if (spread <= 1.0 / 128.0) {
                spreadIncreasing = true;
            }
        }

    }

    os_price.close();
    os_book.close();
}

// Generate trades data
void genTrades(const vector<string>& products, const string& tradeFile= "trades.txt", long long seed=0) 
{
    vector<string> books = { "TRSY1", "TRSY2", "TRSY3" };
    vector<long> quantities = { 1000000, 2000000, 3000000, 4000000, 5000000 };
    std::ofstream os(tradeFile);
    std::mt19937 gen(seed);

    for (const auto& product : products) {
        for (int i = 0; i < 10; ++i) {
            string side = (i % 2 == 0) ? "BUY" : "SELL";

            // generate random trade id
            string id = GenerateRandomId(12, gen);
            
            std::uniform_real_distribution<double> dist(side == "BUY" ? 99.0 : 100.0, side == "BUY" ? 100.0 : 101.0);
            double price = dist(gen);
            long quantity = quantities[i % quantities.size()];
            string book = books[i % books.size()];

            os << product << "," << id << "," << Price2Str(price) << "," << book << "," << quantity << "," << side << endl;
        }
    }

    os.close();
}

// Generate inquiry data
void genInquiries(const vector<string>& products, const string& inquiryFile="inquiries.txt", long long seed=12345) 
{
    std::ofstream os(inquiryFile);
    std::mt19937 gen(seed);
    vector<long> quantities = { 1000000, 2000000, 3000000, 4000000, 5000000 };

    for (const auto& product : products) {
        for (int i = 0; i < 10; ++i) {
            string side = (i % 2 == 0) ? "BUY" : "SELL";
            // generate random inquiry id
            string inquiryId = GenerateRandomId(12, gen);
            // generate random buy price 99-100 and random sell price 100-101 with given seed
            std::uniform_real_distribution<double> dist(side == "BUY" ? 99.0 : 100.0, side == "BUY" ? 100.0 : 101.0);
            double price = dist(gen);
            long quantity = quantities[i % quantities.size()];
            string status = "RECEIVED";

            os << inquiryId << "," << product << "," << side << "," << quantity << "," << Price2Str(price) << "," << status << endl;
        }
    }
}



int main()
{
    // const vector<string> products = {"2Y", "3Y", "5Y", "7Y", "10Y", "20Y"};
    vector<string> cusips = {"91282CLY5", "91282CMB4", "91282CMA6", "91282CLZ2", "91282CLW9", "912810UF3", "912810UE6"};
    cout << "Generating orderbooks..." << endl;
    genOrderBook(cusips);
    cout << "Generating trades..." << endl;
    genTrades(cusips);
    cout << "Generating inquiries..." << endl;
    genInquiries(cusips);
    return 0;
}