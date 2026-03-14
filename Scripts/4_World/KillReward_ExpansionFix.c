// ============================================================
// KillReward → Expansion Market Fix (Bank Version)
// 
// Redirects KillReward's increasePlayerCurrency() directly
// into the player's Expansion ATM bank account balance.
// No physical cash is spawned — money goes straight to bank.
// 
// Author: LOOTGOBBLIN
// ============================================================

modded class PlayerBase
{
	void increasePlayerCurrency(int amount)
	{
		// Server only
		if (!GetGame().IsServer())
			return;

		if (!this || amount <= 0)
			return;

		PlayerIdentity identity = GetIdentity();
		if (!identity)
			return;

		string playerID   = identity.GetId();
		string playerName = identity.GetName();

		Print("[KillReward Fix] Depositing $" + amount + " to bank for: " + playerName);

		// Get Expansion Market module
		ExpansionMarketModule marketModule = ExpansionMarketModule.GetInstance();
		if (!marketModule)
		{
			Print("[KillReward Fix] ERROR: ExpansionMarketModule not found for: " + playerName);
			return;
		}

		// Get or create the player's ATM data
		ExpansionMarketATM_Data atmData = marketModule.GetPlayerATMData(playerID);
		if (!atmData)
		{
			// Player has never used the ATM — create their record
			marketModule.CreateATMData(identity);
			atmData = marketModule.GetPlayerATMData(playerID);
		}

		if (!atmData)
		{
			Print("[KillReward Fix] ERROR: Could not get/create ATM data for: " + playerName);
			return;
		}

		// Add money directly to bank balance and save
		atmData.AddMoney(amount);
		atmData.Save();

		Print("[KillReward Fix] SUCCESS: Deposited $" + amount + " to bank for: " + playerName);
	}
}