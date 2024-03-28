#pragma once
#include "Market.h"
#include <cstdlib>

class MarketParticipants {
public:

	// Function to Read the Market Information
	virtual void read_market_info(const Market* m_ptr) = 0;

	// Function to Submit a Order
	virtual void submit_order(Market* m_ptr) = 0;
};

class MarketMakerBase : public MarketParticipants {
public:
	// Member variables
	double theta_plus;
	double theta_minus;
	double v_h;
	double v_l;
	double ask_price;
	double bid_price;
	double pi;

	MarketMakerBase(double v_h, double v_l, double pi)
		: theta_plus(0.5), theta_minus(0.5), v_h(v_h), v_l(v_l), ask_price(0), bid_price(0), pi(pi) {
	}

	// Calculates the bid and ask prices based on the current price of the stock and the probabilities of the high and low values of the stock
	virtual void calculate_bid_ask() = 0;

	// Reads the current price of the stock from the market, calculates the bid and ask prices, and prints them to the console
	virtual void read_market_info(const Market* const market) = 0;

	// Submits a limit order to the market based on the current bid and ask prices
	virtual void submit_order(Market* const market) = 0;
};

class TraderBase : public MarketParticipants {
public:
	// Member variables
	bool informed;
	double pi;
	int value;
	double uninformed_profit;
	int uninformed_quantity;
	double informed_profit;
	int informed_quantity;
	unsigned int transaction;
	std::default_random_engine* const gen_ptr;

	TraderBase(double pi, std::default_random_engine* const gen_ptr)
		:pi(pi), value(0), gen_ptr(gen_ptr), informed(false), uninformed_profit(0), informed_profit(0), uninformed_quantity(0), informed_quantity(0), transaction(0) {
	}

	// Submits a market order to the market based on the current period and trader type (informed or uninformed)
	void submit_order(Market* const m_ptr) {
		if (value > 0) { // Buy
			std::cout << "[MESSAGE] Trader is buying " << value << " shares." << std::endl;
			informed ? informed_profit -= m_ptr->get_current_ask() * value : uninformed_profit -= m_ptr->get_current_ask() * value;
		}
		else if (value < 0) { // Sell
			std::cout << "[MESSAGE] Trader is selling " << -value << " shares." << std::endl;
			informed ? informed_profit -= m_ptr->get_current_bid() * value : uninformed_profit -= m_ptr->get_current_bid() * value;
		}
		else { // Do Nothing
			std::cout << "[MESSAGE] Trader is not trading." << std::endl;
		}
		informed ? informed_quantity += value : uninformed_quantity += value; // Updating the Current Holdings
		value != 0 ? transaction += 1 : transaction += 0; // Updating the Total Number of Transactions Done

		// Submit the order to the market
		m_ptr->submit_market_order(value);
	}

	// Reads the current probability of the high and low values of the stock from the market and updates the trader's probabilities of buying and selling
	virtual void read_market_info(const Market* const m_ptr) = 0;

	// Refresh
	void refresh() {
		std::uniform_real_distribution<double> distribution(0, 1);

		// Determining if the Trader is Informed
		double num = distribution(*gen_ptr);
		if (num <= pi) {
			informed = true;
		}
		else {
			informed = false;
		}
	}

	// Returning Uninformed Profit per Transaction
	double realizing_uninformed_profit(const Market* const m_ptr) {
		return (uninformed_quantity * m_ptr->get_final_value() + uninformed_profit) / transaction; // PnL when Positions are Closed at Last
	}

	// Returning Informed Profit per Transaction
	double realizing_informed_profit(const Market* const m_ptr) {
		return (informed_quantity * m_ptr->get_final_value() + informed_profit) / transaction; // PnL when Positions are Closed at Last
	}
};