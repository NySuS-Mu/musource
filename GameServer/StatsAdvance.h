
#pragma once


struct STATS_ADVANCE_DATA
{
	int Level;
	int MasterLevel;
	int Reset;
	int GrandReset;
	int TotalDamageReflect;
	int FullDamageReflectRate;
	int CriticalDamageRate;
	int CriticalDamagePower;
	int ExellentDamageRate;
	int ExellentDamagePower;
	int DoubleDamageRate;
	int TripleDamageRate;
	int DamageReductionRate;
	int ShieldSkillDamageReductionRate;
	int IgnoreDefenceRate;
	int IncreaseDamagePvP;
	int IncreaseDefencePvP;
	int ResistDoubleDamageRate;
	int ResistIgnoreDefenceRate;
	int ResistCriticalDamageRate;
	int ResistExellentDamageRate;
	int ResistStumRate;
	int ResistIce;
	int ResistPoison;
	int ResistLighting;
	int ResistFire;
	int ResistEarth;
	int ResistWind;
	int ResistWater;
	int FullHPRestoreRate;
	int FullMPRestoreRate;
	int FullSDRestoreRate;
};

struct PMSG_STATS_ADVANCE
{
	PSBMSG_HEAD h;
	BYTE Result;
	STATS_ADVANCE_DATA StatInfo;
};


class CStatsAdvance
{
public:
	CStatsAdvance();
	~CStatsAdvance();

	void Load();
	void Send(int aIndex);
	void Recv(PMSG_STATS_ADVANCE* lpMsg);
	bool m_Enable;
	bool m_Loaded;
	STATS_ADVANCE_DATA m_Data;
};

extern CStatsAdvance g_StatsAdvance;
