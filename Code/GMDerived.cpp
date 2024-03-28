#include "GMDerived.h"

// Constructor for MarketMaker class
MarketMakerDerived::MarketMakerDerived(double v_h, double v_l, double pi)
	: MarketMakerBase(v_h, v_l, pi), last_price(0), theta_prior(0.5) {
}


// Calculates the bid and ask prices based on the current price of the stock and the probabilities of the high and low values of the stock
void MarketMakerDerived::calculate_bid_ask() {
	double p = std::max(std::min((last_price - v_l) / (v_h - v_l), 0.99), 0.01); // Naive uniform distribution assumption
	double log_odd = log(p / (1 - p)); // Use logit function to map the original probability to log odds
	theta_prior = (0.5 * (1 + erf(log_odd / pow(2, 0.5)))); // Translate log odds and map it onto normal distribution assumption on probability

	// Formulas from GM Paper
	theta_plus = ((1 + pi) / 2) / (pi * theta_prior + (1 - pi) / 2) * theta_prior;
	theta_minus = ((1 - pi) / 2) / (pi * (1 - theta_prior) + (1 - pi) / 2) * theta_prior;

	ask_price = theta_plus * v_h + (1 - theta_plus) * v_l;
	bid_price = theta_minus * v_h + (1 - theta_minus) * v_l;
}

// Reads the current price of the stock from the market, calculates the bid and ask prices, and sends them to the market
void MarketMakerDerived::read_market_info(const Market* const m_ptr) {
	last_price = m_ptr->get_last_price();
}

// Submits a limit order to the market based on the current bid and ask prices
void MarketMakerDerived::submit_order(Market* const m_ptr) {
	calculate_bid_ask(); // Updating the Bid Ask Internally
	std::cout << "[MESSAGE] Market Maker: Ask price is " << ask_price << std::endl;
	std::cout << "[MESSAGE] Market Maker: Bid price is " << bid_price << std::endl;
	m_ptr->submit_limit_order(bid_price, ask_price);
}

// Constructor for Trader class
TraderDerived::TraderDerived(double pi, std::default_random_engine* const gen_ptr)
	: TraderBase(pi, gen_ptr) {
}

// Reads the current probability of the high and low values of the stock from the market and updates the trader's probabilities of buying and selling
void TraderDerived::read_market_info(const Market* const m_ptr) {
	if (informed) {
		if (m_ptr->get_current_ask() <= m_ptr->get_final_value()) {
			value = 1; // Buy for Informed
		}
		else if (m_ptr->get_current_bid() >= m_ptr->get_final_value()) {
			value = -1; // Sell for Informed
		}
		else {
			value = 0; // Nothing for Informed
		}
	}
	else {
		std::uniform_real_distribution<double> distribution(0, 1.5);
		double num = distribution(*gen_ptr);
		double ratio = (m_ptr->get_last_price() - m_ptr->get_v_l()) / (m_ptr->get_v_h() - m_ptr->get_v_l()); // Uniform Assumption for Uninformed Traders (More Likely to Buy if Price is High)

		// 1/3 Chance to Do Nothing (Random Number > 1)
		// Remaining 2/3 Chance is Buying or Selling, Depending on the Cutoff Based on Uniform Assumption on Price Distribution
		num > ratio ? num > 1 ? value = 0 : value = 1 : value = -1;
	}
}