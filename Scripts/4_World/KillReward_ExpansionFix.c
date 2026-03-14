// ============================================================
// KillReward → Expansion Market Fix
// 
// KillReward calls increasePlayerCurrency() which is a
// Dr. Jones Trader function. Expansion Market never defines it
// so the call does nothing. This mod defines the missing
// function and redirects it to Expansion's own SpawnMoney().
// 
// SpawnMoney() handles denominations, stacking, and
// inventory-full fallback automatically.
// 
// Author: LOOTGOBBLIN
// Verified against: salutesh/DayZ-Expansion-Scripts
// ============================================================

modded class PlayerBase
{
	// This function is called by KillReward but only exists
	// in Dr. Jones Trader. We define it here for Expansion.
	void increasePlayerCurrency(int amount)
	{
		// Server only
		if (!GetGame().IsServer())
			return;

		if (!this || amount <= 0)
			return;

		string playerName = GetIdentity().GetName();

		Print("[KillReward Fix] Rewarding $" + amount + " to: " + playerName);

		// Get Expansion Market module instance
		ExpansionMarketModule marketModule = ExpansionMarketModule.GetInstance();

		if (!marketModule)
		{
			Print("[KillReward Fix] ERROR: ExpansionMarketModule not found for: " + playerName);
			return;
		}

		// SpawnMoney handles denominations, existing stacks,
		// and inventory-full overflow automatically
		EntityAI parent = this;
		marketModule.SpawnMoney(this, parent, amount);

		// CheckSpawn handles any items that couldn't fit
		// in inventory (drops to ground near player)
		marketModule.CheckSpawn(this, parent);

		Print("[KillReward Fix] SUCCESS: Spawned $" + amount + " for: " + playerName);
	}
}