#pragma once
#include "MarketParticipantsBase.h"

class MarketMakerDerived : public MarketMakerBase {
public:
	// Constructor for MarketMaker class
	MarketMakerDerived(double v_h, double v_l, double pi);

	// Reads the current price of the stock from the market, calculates the bid and ask prices, and prints them to the console
	void read_market_info(const Market* const market) override;

	// Submits a limit order to the market based on the current bid and ask prices
	void submit_order(Market* const market) override;

private:
	double last_price;
	double theta_prior;

	// Calculates the bid and ask prices based on the current price of the stock and the probabilities of the high and low values of the stock
	void calculate_bid_ask() override;
};

class TraderDerived : public TraderBase {
public:
	// Constructor for Trader class
	TraderDerived(double pi, std::default_random_engine* const gen_ptr);

	// Reads the current probability of the high and low values of the stock from the market and updates the trader's probabilities of buying and selling
	void read_market_info(const Market* const m_ptr) override;
};