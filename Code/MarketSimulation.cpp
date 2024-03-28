#include "MarketSimulation.h"

MarketSimulationGMOriginal::MarketSimulationGMOriginal(Market* const m_ptr, TraderOriginal* const t_ptr, MarketMakerOriginal* const mm_ptr)
: m_ptr(m_ptr), t_ptr(t_ptr), mm_ptr(mm_ptr) {

}

void MarketSimulationGMOriginal::run_simulation() {

	std::cout << "\n[MESSAGE] Begin Trading" << std::endl;
	for (; m_ptr->end_of_trading() == false; m_ptr->time_add()) {
		std::cout << std::endl;
		std::cout << "[MESSAGE] Current Period: " << m_ptr->get_current_period() << std::endl;

		// Market Maker Submits Limit Order
		std::cout << "[MESSAGE] Market Maker Taking Action" << std::endl;
		mm_ptr->read_market_info(m_ptr);
		mm_ptr->submit_order(m_ptr);

		// Trader Reacts and Submits Market Order
		std::cout << "[MESSAGE] Trader Taking Action" << std::endl;
		t_ptr->refresh();
		t_ptr->read_market_info(m_ptr);
		t_ptr->submit_order(m_ptr);

		// Trade Matching
		std::cout << "[MESSAGE] Trade Taking Place" << std::endl;
		m_ptr->trade_matching();
		std::cout << "[MESSAGE] The New Spread is: " << m_ptr->get_current_spread() << std::endl;
	}

	// Operations for Final Period
	std::cout << std::endl;
	std::cout << "[MESSAGE] End of All Trading Periods, Revealing Final Value of Securities" << std::endl;
	std::cout << "[MESSAGE] Last Price of Securities: " << m_ptr->get_last_price() << std::endl;
	std::cout << "[MESSAGE] Final Value of Securities: " << m_ptr->get_final_value() << std::endl;

	return;
}

MarketSimulationGMDerived::MarketSimulationGMDerived(Market* const m_ptr, TraderDerived* const t_ptr, MarketMakerDerived* const mm_ptr)
	: m_ptr(m_ptr), t_ptr(t_ptr), mm_ptr(mm_ptr) {

}

void MarketSimulationGMDerived::run_simulation() {

	std::cout << "\n[MESSAGE] Begin Trading" << std::endl;
	for (; m_ptr->end_of_trading() == false; m_ptr->time_add()) {
		std::cout << std::endl;
		std::cout << "[MESSAGE] Current Period: " << m_ptr->get_current_period() << std::endl;
		std::cout << "[MESSAGE] Remaining Period: " << m_ptr->get_total_period() - m_ptr->get_current_period() + 1 << std::endl;
		std::cout << "[MESSAGE] Last Price: " << m_ptr->get_last_price() << std::endl;

		// Market Maker Submits Limit Order
		std::cout << "[MESSAGE] Market Maker Taking Action" << std::endl;
		mm_ptr->read_market_info(m_ptr);
		mm_ptr->submit_order(m_ptr);

		// Trader Reacts and Submits Market Order
		std::cout << "[MESSAGE] Trader Taking Action" << std::endl;
		t_ptr->refresh();
		t_ptr->read_market_info(m_ptr);
		t_ptr->submit_order(m_ptr);

		// Trade Matching
		std::cout << "[MESSAGE] Trade Taking Place" << std::endl;
		m_ptr->trade_matching();
		std::cout << "[MESSAGE] The New Spread is: " << m_ptr->get_current_spread() << std::endl;
	}

	// Operations for Final Period
	std::cout << std::endl;
	std::cout << "[MESSAGE] End of All Trading Periods, Revealing Final Value of Securities" << std::endl;
	std::cout << "[MESSAGE] Last Price of Securities: " << m_ptr->get_last_price() << std::endl;
	std::cout << "[MESSAGE] Final Value of Securities: " << m_ptr->get_final_value() << std::endl;

	return;
}