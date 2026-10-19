#include "global.h"
#include "battle.h"
#include "event_data.h"
#include "caps.h"
#include "pokemon.h"
#include "constants/battle_ai.h"
#include "constants/region_map_sections.h"

static u8 GetCurrentBadgeCount(void)
{
    u16 i;
    u8 badgeCount = 0;
    for (i = FLAG_BADGE01_GET; i < FLAG_BADGE01_GET + NUM_BADGES; i++)
    {
        if (FlagGet(i))
            badgeCount++;
    }
    return badgeCount;
}

#if IS_HNS
// Hoenn badges live in their own flag run (see FLAG_BADGE17_GET in flags_hns.h).
static u8 GetHoennBadgeCount(void)
{
    u16 i;
    u8 badgeCount = 0;
    for (i = FLAG_BADGE_HOENN_START; i < FLAG_BADGE_HOENN_START + NUM_BADGES_HOENN; i++)
    {
        if (FlagGet(i))
            badgeCount++;
    }
    return badgeCount;
}
#endif

// Indexed by the number of badges held. Rows 0-8 are the Johto progression. Rows 9-24 only come into
// play once at least one Hoenn badge is held: they let total badges (Johto + Kanto + Hoenn) raise the
// cap, so the player can move between regions in any order and keep levelling up.
static const u8 sLevelCapTable_Normal[] =
{
    [0] = 11,  // no badges
    [1] = 16,  // badge 1
    [2] = 21,  // badge 2
    [3] = 25,  // badge 3
    [4] = 31,  // badge 4
    [5] = 36,  // badge 5
    [6] = 38,  // badge 6
    [7] = 45,  // badge 7
    [8] = 56,  // badge 8
    [9] = 58,
    [10] = 60,
    [11] = 62,
    [12] = 64,
    [13] = 66,
    [14] = 68,
    [15] = 69,
    [16] = 70,
    [17] = 72,
    [18] = 74,
    [19] = 76,
    [20] = 78,
    [21] = 80,
    [22] = 85,
    [23] = 90,
    [24] = 100,
};

static const u8 sLevelCapTable_Hard[] =
{
    [0] = 8,
    [1] = 15,
    [2] = 20,
    [3] = 23,
    [4] = 29,
    [5] = 33,
    [6] = 37,
    [7] = 42,
    [8] = 54,
    [9] = 56,
    [10] = 58,
    [11] = 60,
    [12] = 62,
    [13] = 64,
    [14] = 66,
    [15] = 67,
    [16] = 68,
    [17] = 70,
    [18] = 72,
    [19] = 74,
    [20] = 76,
    [21] = 78,
    [22] = 83,
    [23] = 88,
    [24] = 98,
};

static u32 GetLevelCapForBadgeIndex(u8 challengeLevelCap, u8 badgeIndex)
{
    if (challengeLevelCap == 1)
        return sLevelCapTable_Normal[badgeIndex];
    else
        return sLevelCapTable_Hard[badgeIndex];
}

// Post-league caps, matching HnS: beating the Johto Elite Four raises the cap to
// 70 for the Kanto half, and only beating the Kanto league removes it entirely.
#define KANTO_MAX_LEVEL 70

u32 GetCurrentLevelCap(void)
{
    static const u32 sLevelCapFlagMap[][2] =
    {
        {FLAG_BADGE01_GET, 15},
        {FLAG_BADGE02_GET, 19},
        {FLAG_BADGE03_GET, 24},
        {FLAG_BADGE04_GET, 29},
        {FLAG_BADGE05_GET, 31},
        {FLAG_BADGE06_GET, 33},
        {FLAG_BADGE07_GET, 42},
        {FLAG_BADGE08_GET, 46},
        {FLAG_IS_CHAMPION, 58},
    };

    u8 challengeLevelCap = gSaveBlock3Ptr->challengeSettings.tx_Challenges_LevelCap;

    if (challengeLevelCap != 0)
    {
        u8 badgeCount = GetCurrentBadgeCount();
        if (badgeCount > 8)
            badgeCount = 8;

#if IS_HNS
        u32 cap;
        u8 hoennBadges;

        if (FlagGet(FLAG_IS_KANTO_CHAMPION))
            return MAX_LEVEL;

        if (FlagGet(FLAG_IS_CHAMPION))
            cap = KANTO_MAX_LEVEL;
        else
            cap = GetLevelCapForBadgeIndex(challengeLevelCap, badgeCount);

        // Hoenn badges can only raise the cap, never lower it. Without any Hoenn badge this is
        // exactly the original Johto/Kanto behaviour.
        hoennBadges = GetHoennBadgeCount();
        if (hoennBadges != 0)
        {
            u32 totalBadges = GetCurrentBadgeCount() + hoennBadges;
            u32 totalCap;

            if (totalBadges > NUM_BADGES_TOTAL)
                totalBadges = NUM_BADGES_TOTAL;

            totalCap = GetLevelCapForBadgeIndex(challengeLevelCap, totalBadges);
            if (totalCap > cap)
                cap = totalCap;
        }

        return cap;
#else
        if (FlagGet(FLAG_IS_CHAMPION))
            return MAX_LEVEL;

        return GetLevelCapForBadgeIndex(challengeLevelCap, badgeCount);
#endif
    }

    u32 i;

    if (B_LEVEL_CAP_TYPE == LEVEL_CAP_FLAG_LIST)
    {
        for (i = 0; i < ARRAY_COUNT(sLevelCapFlagMap); i++)
        {
            if (!FlagGet(sLevelCapFlagMap[i][0]))
                return sLevelCapFlagMap[i][1];
        }
    }
    else if (B_LEVEL_CAP_TYPE == LEVEL_CAP_VARIABLE)
    {
        return VarGet(B_LEVEL_CAP_VARIABLE);
    }

    return MAX_LEVEL;
}

u32 GetSoftLevelCapExpValue(u32 level, u32 expValue)
{
    static const u32 sExpScalingDown[5] = { 4, 8, 16, 32, 64 };
    static const u32 sExpScalingUp[5]   = { 16, 8, 4, 2, 1 };

    u32 levelDifference;
    u32 currentLevelCap = GetCurrentLevelCap();

    if (B_EXP_CAP_TYPE == EXP_CAP_NONE)
        return expValue;

    if (level < currentLevelCap)
    {
        if (B_LEVEL_CAP_EXP_UP)
        {
            levelDifference = currentLevelCap - level;
            if (levelDifference > ARRAY_COUNT(sExpScalingUp) - 1)
                return expValue + (expValue / sExpScalingUp[ARRAY_COUNT(sExpScalingUp) - 1]);
            else
                return expValue + (expValue / sExpScalingUp[levelDifference]);
        }
        else
        {
            return expValue;
        }
    }
    else if (B_EXP_CAP_TYPE == EXP_CAP_HARD)
    {
        return 0;
    }
    else if (B_EXP_CAP_TYPE == EXP_CAP_SOFT)
    {
        levelDifference = level - currentLevelCap;
        if (levelDifference > ARRAY_COUNT(sExpScalingDown) - 1)
            return expValue / sExpScalingDown[ARRAY_COUNT(sExpScalingDown) - 1];
        else
            return expValue / sExpScalingDown[levelDifference];
    }
    else
    {
       return expValue;
    }
}

u32 GetCurrentEVCap(void)
{
    static const u16 sEvCapFlagMap[][2] = {
        // Define EV caps for each milestone
        {FLAG_BADGE01_GET, MAX_TOTAL_EVS *  1 / 17},
        {FLAG_BADGE02_GET, MAX_TOTAL_EVS *  3 / 17},
        {FLAG_BADGE03_GET, MAX_TOTAL_EVS *  5 / 17},
        {FLAG_BADGE04_GET, MAX_TOTAL_EVS *  7 / 17},
        {FLAG_BADGE05_GET, MAX_TOTAL_EVS *  9 / 17},
        {FLAG_BADGE06_GET, MAX_TOTAL_EVS * 11 / 17},
        {FLAG_BADGE07_GET, MAX_TOTAL_EVS * 13 / 17},
        {FLAG_BADGE08_GET, MAX_TOTAL_EVS * 15 / 17},
        {FLAG_IS_CHAMPION, MAX_TOTAL_EVS},
    };

    if (B_EV_CAP_TYPE == EV_CAP_FLAG_LIST)
    {
        for (u32 evCap = 0; evCap < ARRAY_COUNT(sEvCapFlagMap); evCap++)
        {
            if (!FlagGet(sEvCapFlagMap[evCap][0]))
                return sEvCapFlagMap[evCap][1];
        }
    }
    else if (B_EV_CAP_TYPE == EV_CAP_VARIABLE)
    {
        return VarGet(B_EV_CAP_VARIABLE);
    }
    else if (B_EV_CAP_TYPE == EV_CAP_NO_GAIN)
    {
        return 0;
    }

    return MAX_TOTAL_EVS;
}

#if IS_HNS
// Where an Emerald run sits, by Hoenn badges held (0-8): roughly the level of the next Gym Leader's ace.
// The scaling below shifts Emerald's levels up by (current cap - this), so with N total badges the
// next Hoenn Gym Leader's ace lands exactly on the level cap, and everyone else keeps their place relative to it.
static const u8 sEmeraldStageLevel[NUM_BADGES_HOENN + 1] = { 15, 19, 24, 29, 31, 33, 42, 46, 58 };

static u32 GetHoennLevelBonus(u32 *capOut)
{
    u8 challenge = gSaveBlock3Ptr->challengeSettings.tx_Challenges_LevelCap;
    u8 hoennBadges = GetHoennBadgeCount();
    u32 totalBadges = GetCurrentBadgeCount() + hoennBadges;
    u32 cap;

    if (totalBadges > NUM_BADGES_TOTAL)
        totalBadges = NUM_BADGES_TOTAL;
    if (hoennBadges > NUM_BADGES_HOENN)
        hoennBadges = NUM_BADGES_HOENN;

    // The cap tables are used even when the level cap challenge is off: they are just the difficulty curve here.
    cap = GetLevelCapForBadgeIndex(challenge == 0 ? 1 : challenge, totalBadges);
    *capOut = cap;
    if (cap <= sEmeraldStageLevel[hoennBadges])
        return 0;
    return cap - sEmeraldStageLevel[hoennBadges];
}

u32 GetHoennTotalBadges(void)
{
    return GetCurrentBadgeCount() + GetHoennBadgeCount();
}

bool32 IsInHoennMap(void)
{
    u32 mapSec = gMapHeader.regionMapSectionId;
    return mapSec >= MAPSEC_LITTLEROOT_TOWN && mapSec <= MAPSEC_SECRET_BASE;
}

u32 ScaleHoennLevel(u32 level)
{
    u32 cap;
    u32 bonus = GetHoennLevelBonus(&cap);
    u32 scaled = level + bonus;
    u32 limit = cap + 6; // sequence-breaking guard: never far above what the player can have

    if (limit > MAX_LEVEL)
        limit = MAX_LEVEL;
    if (scaled > limit)
        scaled = limit;
    if (scaled < level) // never weaker than the original
        scaled = level;
    if (scaled > MAX_LEVEL)
        scaled = MAX_LEVEL;
    return scaled;
}

u64 GetHoennBadgeAiFlags(void)
{
    u32 totalBadges = GetCurrentBadgeCount() + GetHoennBadgeCount();
    u64 flags = 0;

    if (totalBadges >= 4)
        flags |= AI_FLAG_CHECK_BAD_MOVE;
    if (totalBadges >= 8)
        flags |= AI_FLAG_TRY_TO_FAINT;
    if (totalBadges >= 12)
        flags |= AI_FLAG_CHECK_VIABILITY;
    if (totalBadges >= 16)
        flags |= AI_FLAG_SMART_SWITCHING; // also turns on smart mon choices
    if (totalBadges >= 20)
        flags |= AI_FLAG_ASSUME_STAB | AI_FLAG_ASSUME_STATUS_MOVES | AI_FLAG_PP_STALL_PREVENTION;
    if (totalBadges >= 24)
        flags |= AI_FLAG_PREDICT_SWITCH | AI_FLAG_PREDICT_INCOMING_MON;
    return flags;
}
#endif // IS_HNS
