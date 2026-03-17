// ============================================================
// KillReward → Expansion Market ATM Fix
//
// Redirects KillReward's increasePlayerCurrency() and
// deductPlayerCurrency() / LoseMoney() calls directly into
// the player's DayZ Expansion ATM bank account balance.
// No physical cash is spawned — money goes straight to bank.
//
// Confirmed API (salutesh/DayZ-Expansion-Scripts):
//   CF_ModuleCoreManager.Get(ExpansionMarketModule)
//   ExpansionMarketModule::GetPlayerATMData(string playerID)
//   ExpansionMarketModule::CreateATMData(PlayerIdentity ident)
//   ExpansionMarketATM_Data::AddMoney(int amount)
//   ExpansionMarketATM_Data::RemoveMoney(int amount)
//   ExpansionMarketATM_Data::Save()
//
// Load order: @KillReward -> @DayZ-Expansion-Bundle -> @KillRewardExpansionFix
//
// Author: LOOTGOBBLIN
// ============================================================

#ifdef EXPANSIONMODMARKET

// ------------------------------------------------------------
// Internal helper: deposit into ATM bank (server-side only).
// Returns true on success, false on any failure.
// ------------------------------------------------------------
static bool KRFix_ATM_Deposit(PlayerBase player, int amount)
{
	if (!player || !player.GetIdentity() || amount <= 0)
		return false;

	ExpansionMarketModule marketModule = ExpansionMarketModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarketModule));
	if (!marketModule)
	{
		Print("[KillReward Fix] ERROR: ExpansionMarketModule not found!");
		return false;
	}

	PlayerIdentity identity = player.GetIdentity();
	string playerID = identity.GetId();
	string playerName = identity.GetName();

	ExpansionMarketATM_Data atmData = marketModule.GetPlayerATMData(playerID);
	if (!atmData)
	{
		marketModule.CreateATMData(identity);
		atmData = marketModule.GetPlayerATMData(playerID);
	}

	if (!atmData)
	{
		Print("[KillReward Fix] ERROR: Could not get/create ATM data for: " + playerName);
		return false;
	}

	int maxDeposit = GetExpansionSettings().GetMarket().MaxDepositMoney;
	int space = maxDeposit - atmData.MoneyDeposited;

	if (space <= 0)
	{
		Print("[KillReward Fix] INFO: ATM full for " + playerName + " (cap: " + maxDeposit + "). Reward skipped.");
		return false;
	}

	if (amount > space)
	{
		Print("[KillReward Fix] INFO: Clamping deposit " + amount + " to " + space + " for " + playerName);
		amount = space;
	}

	atmData.AddMoney(amount);
	atmData.Save();

	Print("[KillReward Fix] Deposited $" + amount + " to ATM bank for: " + playerName);
	return true;
}

// ------------------------------------------------------------
// Internal helper: deduct from ATM bank (server-side only).
// Clamps to current balance so it never goes below zero.
// ------------------------------------------------------------
static bool KRFix_ATM_Deduct(PlayerBase player, int amount)
{
	if (!player || !player.GetIdentity() || amount <= 0)
		return false;

	ExpansionMarketModule marketModule = ExpansionMarketModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarketModule));
	if (!marketModule)
	{
		Print("[KillReward Fix] ERROR: ExpansionMarketModule not found!");
		return false;
	}

	PlayerIdentity identity = player.GetIdentity();
	string playerID = identity.GetId();
	string playerName = identity.GetName();

	ExpansionMarketATM_Data atmData = marketModule.GetPlayerATMData(playerID);
	if (!atmData)
	{
		Print("[KillReward Fix] WARNING: No ATM data for " + playerName + " - cannot deduct.");
		return false;
	}

	if (amount > atmData.MoneyDeposited)
	{
		Print("[KillReward Fix] INFO: Clamping deduction " + amount + " to " + atmData.MoneyDeposited + " for " + playerName);
		amount = atmData.MoneyDeposited;
	}

	if (amount <= 0)
		return false;

	atmData.RemoveMoney(amount);
	atmData.Save();

	Print("[KillReward Fix] Deducted $" + amount + " from ATM bank for: " + playerName);
	return true;
}

// ------------------------------------------------------------
// Modded KillReward - override LoseMoney (negative PvP penalty)
// Original: killerVictim.deductPlayerCurrency(LoseMoney)
// Fixed:    deduct from ATM bank instead
// ------------------------------------------------------------
modded class KillReward
{
	override void LoseMoney(PlayerIdentity killerIdentity, PlayerBase killerVictim)
	{
		int amount = g_Game.KRconfig.KillRewardNEGATIVE.LoseMoney;

		if (!KRFix_ATM_Deduct(killerVictim, amount))
		{
			Print("[KillReward Fix] LoseMoney fallback to deductPlayerCurrency for " + killerIdentity.GetId());
			killerVictim.deductPlayerCurrency(amount);
		}
	}
}

// ------------------------------------------------------------
// Modded PlayerBase - override increasePlayerCurrency so all
// KillReward milestone payouts deposit to ATM instead of
// spawning physical cash in the player's inventory.
//
// SCOPE: Server-side only. Client calls pass through untouched.
// SAFE:  Falls back to physical cash if ATM is unavailable.
// ------------------------------------------------------------
modded class PlayerBase
{
	override void increasePlayerCurrency(int amount)
	{
		if (!GetGame().IsServer())
		{
			super.increasePlayerCurrency(amount);
			return;
		}

		if (!KRFix_ATM_Deposit(this, amount))
		{
			Print("[KillReward Fix] increasePlayerCurrency fallback to physical cash (" + amount + ")");
			super.increasePlayerCurrency(amount);
		}
	}
}

#endif // EXPANSIONMODMARKET