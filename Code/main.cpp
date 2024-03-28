#include "Common.h"
#include "Market.h"
#include "GMOriginal.h"
#include "GMDerived.h"
#include "MarketSimulation.h"
#include <iostream>
#include <ctime>
#include <vector>

int main()
{
    std::cout.precision(3);

    // Initializing a Random Generator
	std::default_random_engine generator;
	generator.seed(20222023);

    // Parameters for the Market
    unsigned int level = 6;
    double initial_price = 100, v_h = 150, v_l = 50;

    // Parameter for the Trader
    double pi = 0.3;
    
    // Number of Loops to be Ran
    unsigned int n = 30;

    // Results Variables
    std::vector<double> uninformed_average_profit;
    std::vector<double> informed_average_profit;
    std::vector<double> convergence;
    std::vector<double> uninformed_average_profit_derived;
    std::vector<double> informed_average_profit_derived;
    std::vector<double> convergence_derived;

    // Results Related Variables
    double cutoff = 0.01;

    for (unsigned int i = 0; i < n; i++) {
            // Running for Original GM Model
            Market market(initial_price, level, v_h, v_l, &generator);

            TraderOriginal trader(pi, &generator);

            MarketMakerOriginal mm(market.get_v_h(), market.get_v_l(), pi);

            MarketSimulationGMOriginal ms(&market, &trader, &mm);
            ms.run_simulation();

            // Calculating Results for GM Model
            const std::vector<double>* ptr = market.get_all_spread();
            convergence.push_back(double(get_cut_off(ptr, cutoff)));
            uninformed_average_profit.push_back(trader.realizing_uninformed_profit(&market));
            informed_average_profit.push_back(trader.realizing_informed_profit(&market));

            // Running for Derived Model
            Market market_2(initial_price, level, v_h, v_l, &generator);

            TraderDerived trader_2(pi, &generator);

            MarketMakerDerived mm_2(market_2.get_v_h(), market_2.get_v_l(), pi);

            MarketSimulationGMDerived ms_2(&market_2, &trader_2, &mm_2);
            ms_2.run_simulation();

            // Calculating Results for Derived Model
            const std::vector<double>* ptr_2 = market_2.get_all_spread();
            convergence_derived.push_back(double(get_cut_off(ptr_2, cutoff)));
            uninformed_average_profit_derived.push_back(trader_2.realizing_uninformed_profit(&market_2));
            informed_average_profit_derived.push_back(trader_2.realizing_informed_profit(&market_2));
    }

    // Outputing the Final Results of n Loops
    std::cout << std::endl;
    std::cout << "[MESSAGE] Results of " << n << " runs" << std::endl;
    std::cout << std::endl;

    std::cout << "[MESSAGE] Uninformed Average Profit (Original): " << get_vector_average(&uninformed_average_profit) << std::endl;
    print_vector(&uninformed_average_profit);
    std::cout << std::endl;

    std::cout << "[MESSAGE] Informed Average Profit (Original): " << get_vector_average(&informed_average_profit) << std::endl;
    print_vector(&informed_average_profit);
    std::cout << std::endl;

    std::cout << "[MESSAGE] Convergence of Spread (Original): " << get_vector_average(&convergence) << std::endl;
    print_vector(&convergence);
    std::cout << std::endl;

    std::cout << "[MESSAGE] Uninformed Average Profit (Derived): " << get_vector_average(&uninformed_average_profit_derived) << std::endl;
    print_vector(&uninformed_average_profit_derived);
    std::cout << std::endl;

    std::cout << "[MESSAGE] Informed Average Profit (Derived): " << get_vector_average(&informed_average_profit_derived) << std::endl;
    print_vector(&informed_average_profit_derived);
    std::cout << std::endl;

    std::cout << "[MESSAGE] Convergence of Spread (Derived): " << get_vector_average(&convergence_derived) << std::endl;
    print_vector(&convergence_derived);
    std::cout << std::endl;

    return 0;
}