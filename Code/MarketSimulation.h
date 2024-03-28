#pragma once
#include "GMOriginal.h"
#include "GMDerived.h"

// Base Class for Market Simulation
class MarketSimulationBase {
public:
	virtual void run_simulation() = 0;
};

// Market Simulation for Original GM Model
class MarketSimulationGMOriginal: public MarketSimulationBase {
private:
	Market* const m_ptr;
	TraderOriginal* const t_ptr;
	MarketMakerOriginal* const mm_ptr;

public:
	MarketSimulationGMOriginal(Market* const m_ptr, TraderOriginal* const t_ptr, MarketMakerOriginal* const mm_ptr);
	void run_simulation() override;
};

// Market Simulation for Derived Model
class MarketSimulationGMDerived : public MarketSimulationBase {
private:
	Market* const m_ptr;
	TraderDerived* const t_ptr;
	MarketMakerDerived* const mm_ptr;

public:
	MarketSimulationGMDerived(Market* const m_ptr, TraderDerived* const t_ptr, MarketMakerDerived* const mm_ptr);
	void run_simulation() override;
};