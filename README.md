# KillReward Expansion Market Fix

A lightweight DayZ server mod that bridges **FS-Kaliber/DayZ-KillReward** with the **DayZ Expansion Market ATM banking system**.

Instead of spawning physical currency items in a player's inventory when they earn a kill reward, this mod deposits the money directly into their Expansion ATM bank account balance.

---

## Features

- ✅ All kill reward payouts (zombies, players, animals) go straight to ATM bank
- ✅ Negative PvP penalties are deducted from ATM bank balance
- ✅ Auto-creates ATM account for players who have never visited an ATM
- ✅ Respects the server's `MaxDepositMoney` cap (clamps reward, never crashes)
- ✅ Safe fallback to physical cash if Expansion Market is unavailable
- ✅ KillReward-only patch — does not affect other mods

---

## Requirements

| Mod | Steam Workshop ID |
|-----|-------------------|
| Community Framework (CF) | [1559212036](https://steamcommunity.com/sharedfiles/filedetails/?id=1559212036) |
| KillReward | [FS-Kaliber/DayZ-KillReward](https://github.com/FS-Kaliber/DayZ-KillReward) |
| DayZ Expansion Bundle | [2572331007](https://steamcommunity.com/sharedfiles/filedetails/?id=2572331007) |

---

## Load Order

```
-mod=@CF;@KillReward;@DayZ-Expansion-Bundle;@KillRewardExpansionFix
```

> ⚠️ `@KillRewardExpansionFix` **must be listed last** — it modifies both KillReward and Expansion.

---

## Expansion Settings

In `profiles/ExpansionMod/Settings/MarketSettings.json`, ensure:

```json
"ATMSystemEnabled": 1,
"MaxDepositMoney": 1000000,
"DefaultDepositMoney": 0
```

If `ATMSystemEnabled` is `0`, no ATM data exists for players and the mod will fall back to spawning physical cash.

---

## How to Build

1. Open **DayZ Tools → AddonBuilder**
2. Source directory: `KillRewardExpansionFix/`
3. Destination: `@KillRewardExpansionFix/Addons/`
4. Click **Pack**
5. Copy `@KillRewardExpansionFix` to your server and add to `-mod=`

---

## What Changed vs. Original KillReward

| Event | Original Behaviour | With This Mod |
|---|---|---|
| Zombie / animal / player kill milestone | Spawns cash in inventory | Deposits to ATM bank |
| Negative PvP penalty | Removes cash from inventory | Deducts from ATM bank |
| ATM data missing | *(N/A)* | Auto-creates record, then deposits |
| ATM at max cap | *(N/A)* | Clamps reward, logs warning |
| Expansion Market not loaded | *(N/A)* | Falls back to physical cash |

---

## Author

**LOOTGOBBLIN** — [GitHub](https://github.com/lootgobblin4help-design)
