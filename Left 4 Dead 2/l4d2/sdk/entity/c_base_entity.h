﻿#pragma once

#include "../client/c_base_client.h"
#include "../client/c_collideable.h"
#include "../../game_utilities/game_utilities.h"
#include "../vec/vec.h"

#include "../../netvar_manager/netvar_manager.h"

#define	LIFE_ALIVE				0 // alive
#define	LIFE_DYING				1 // playing death animation or still falling off of a ledge waiting to hit ground
#define	LIFE_DEAD				2 // dead. lying still.
#define LIFE_RESPAWNABLE		3
#define LIFE_DISCARDBODY		4

#define is_sub_machine_gun(_id)		    (_id == WeaponId_SubMachinegun || _id == WeaponId_Silenced || _id == WeaponId_MP5)
#define is_shotgun(_id)				    (_id == WeaponId_PumpShotgun || _id == WeaponId_Chrome || _id == WeaponId_AutoShotgun || _id == WeaponId_SPAS)
#define is_assault_rifle(_id)			(_id == WeaponId_AssaultRifle || _id == WeaponId_AK47 || _id == WeaponId_Desert || _id == WeaponId_SG552 || _id == WeaponId_M60)
#define is_sniper(_id)				    (_id == WeaponId_SniperRifle || _id == WeaponId_Military || _id == WeaponId_Scout || _id == WeaponId_AWP)
#define is_pistol(_id)				    (_id == WeaponId_Pistol || _id == WeaponId_MagnumPistol)
#define is_med(_id)				        (_id == WeaponId_FirstAidKit || _id == WeaponId_ItemDefibrillator || _id == WeaponId_PainPills || _id == WeaponId_Adrenaline)
#define is_ammo_pack(_id)				(_id == WeaponId_ItemAmmoPack || _id == WeaponId_ItemUpgradePackExplosive || _id == WeaponId_ItemUpgradePackIncendiary)
#define is_melee(_id)				    (_id == WeaponId_TerrorMeleeWeapon || _id == WeaponId_Chainsaw)
#define is_weapon_t1(_id)				(is_sub_machine_gun(_id) || _id == WeaponId_PumpShotgun || _id == WeaponId_Chrome || _id == WeaponId_Pistol)
#define is_weapon_t2(_id)				(_id == WeaponId_AutoShotgun || _id == WeaponId_SPAS || _id == WeaponId_AssaultRifle || _id == WeaponId_AK47 || _id == WeaponId_Desert || _id == WeaponId_SG552 || _id == WeaponId_MagnumPistol || IsSniper(_id))
#define is_weapon_t3(_id)				(_id == WeaponId_M60 || _id == WeaponId_GrenadeLauncher)
#define is_not_weapon_gun(_id)			(is_grenade_weapon(_id) || is_med_weapon(_id) || is_pills_weapon(_id) || is_carry_weapon(_id) || _id == Weapon_Melee || _id == Weapon_Chainsaw)
#define is_gun_weapon(_id)			    (is_sub_machine_gun(_id) || is_shotgun(_id) || is_assault_rifle(_id) || is_sniper(_id) || is_pistol(_id))
#define is_grenade_weapon(_id)		    (_id == Weapon_Molotov || _id == Weapon_PipeBomb || _id == Weapon_Vomitjar)
#define is_med_weapon(_id)		        (_id == Weapon_FirstAidKit || _id == Weapon_Defibrillator || _id == Weapon_FireAmmo || _id == Weapon_ExplodeAmmo)
#define is_pills_weapon(_id)			(_id == Weapon_PainPills || _id == Weapon_Adrenaline)
#define is_carry_weapon(_id)			(_id == Weapon_Gascan || _id == Weapon_Fireworkcrate || _id == Weapon_Propanetank || _id == Weapon_Oxygentank || _id == Weapon_Gnome || _id == Weapon_Cola)

#define BONE_CALCULATE_MASK			0x1F
#define BONE_PHYSICALLY_SIMULATED	0x01	// bone is physically simulated when physics are active
#define BONE_PHYSICS_PROCEDURAL		0x02	// procedural when physics is active
#define BONE_ALWAYS_PROCEDURAL		0x04	// bone is always procedurally animated
#define BONE_SCREEN_ALIGN_SPHERE	0x08	// bone aligns to the screen, not constrained in motion.
#define BONE_SCREEN_ALIGN_CYLINDER	0x10	// bone aligns to the screen, constrained by it's own axis.

#define BONE_USED_MASK				0x0007FF00
#define BONE_USED_BY_ANYTHING		0x0007FF00
#define BONE_USED_BY_HITBOX			0x00000100	// bone (or child) is used by a hit box
#define BONE_USED_BY_ATTACHMENT		0x00000200	// bone (or child) is used by an attachment point
#define BONE_USED_BY_VERTEX_MASK	0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0	0x00000400	// bone (or child) is used by the toplevel model via skinned vertex
#define BONE_USED_BY_VERTEX_LOD1	0x00000800	
#define BONE_USED_BY_VERTEX_LOD2	0x00001000  
#define BONE_USED_BY_VERTEX_LOD3	0x00002000
#define BONE_USED_BY_VERTEX_LOD4	0x00004000
#define BONE_USED_BY_VERTEX_LOD5	0x00008000
#define BONE_USED_BY_VERTEX_LOD6	0x00010000
#define BONE_USED_BY_VERTEX_LOD7	0x00020000
#define BONE_USED_BY_BONE_MERGE		0x00040000	// bone is available for bone merge to occur against it

#define is_special_infected(_id)		(_id == ET_BOOMER || _id == ET_HUNTER || _id == ET_SMOKER || _id == ET_SPITTER || _id == ET_JOCKEY || _id == ET_CHARGER || _id == ET_TANK)

enum m_team_id
{
	team_unassigned,
	team_spectator,
	team_survivor,
	team_zomby,
	team_unk
};

enum m_hitgroup
{
	hitgroup_all = 0,
	hitgroup_head = 1,
	hitgroup_chest = 2,
	hitgroup_stomach = 3,
	hitgroup_left_arm = 4,
	hitgroup_right_arm = 5,
	hitgroup_left_leg = 6,
	hitgroup_right_leg = 7,
	hitgroup_gear = 10,
};

enum m_weapon_id : int
{
	Weapon_Pistol = 1,			
	Weapon_ShotgunPump = 3,			
	Weapon_ShotgunAuto = 4,			
	Weapon_SniperHunting = 6,		
	Weapon_ShotgunChrome = 8,		
	Weapon_SniperMilitary = 10,		
	Weapon_ShotgunSpas = 11,		
	Weapon_PistolMagnum = 32,		
	Weapon_SniperAWP = 35,			
	Weapon_SniperScout = 36,		

	Weapon_Melee = 19,				
	Weapon_GrenadeLauncher = 21,	
	Weapon_Chainsaw = 20,			

	Weapon_FirstAidKit = 12,		
	Weapon_PainPills = 15,			
	Weapon_Defibrillator = 24,		
	Weapon_Adrenaline = 23,			
	Weapon_Molotov = 13,			
	Weapon_PipeBomb = 14,		
	Weapon_Vomitjar = 25,		
	Weapon_FireAmmo = 30,			
	Weapon_ExplodeAmmo = 31,		
	Weapon_Gascan = 16,				
	Weapon_Fireworkcrate = 29,	
	Weapon_Propanetank = 17,
	Weapon_Oxygentank = 18,
	Weapon_Gnome = 27,
	Weapon_Cola = 28,

	WeaponId_WeaponCSBase = 0,
	WeaponId_AssaultRifle = 5,
	WeaponId_AutoShotgun = 4,
	WeaponId_BaseBackpackItem = 0,
	WeaponId_BoomerClaw = 41,
	WeaponId_CarriedProp = 16,
	WeaponId_Chainsaw = 20,
	WeaponId_ChargerClaw = 40,
	WeaponId_ColaBottles = 28,
	WeaponId_FireworkCrate = 29,
	WeaponId_FirstAidKit = 12,
	WeaponId_GasCan = 16,
	WeaponId_Gnome = 27,
	WeaponId_GrenadeLauncher = 21,
	WeaponId_HunterClaw = 39,
	WeaponId_Adrenaline = 23,
	WeaponId_ItemAmmoPack = 22,
	WeaponId_ItemDefibrillator = 24,
	WeaponId_ItemUpgradePackExplosive = 31,
	WeaponId_ItemUpgradePackIncendiary = 30,
	WeaponId_VomitJar = 25,
	WeaponId_JockeyClaw = 44,
	WeaponId_Molotov = 13,
	WeaponId_OxygenTank = 18,
	WeaponId_PainPills = 15,
	WeaponId_PipeBomb = 14,
	WeaponId_Pistol = 1,
	WeaponId_MagnumPistol = 32,
	WeaponId_PropaneTank = 17,
	WeaponId_PumpShotgun = 3,
	WeaponId_AK47 = 26,
	WeaponId_Desert = 9,
	WeaponId_M60 = 37,
	WeaponId_SG552 = 34,
	WeaponId_Chrome = 8,
	WeaponId_SPAS = 11,
	WeaponId_MP5 = 33,
	WeaponId_Silenced = 7,
	WeaponId_SmokerClaw = 42,
	WeaponId_SniperRifle = 6,
	WeaponId_AWP = 35,
	WeaponId_Military = 10,
	WeaponId_Scout = 36,
	WeaponId_SpitterClaw = 43,
	WeaponId_SubMachinegun = 2,
	WeaponId_TankClaw = 38,
	WeaponId_TerrorMeleeWeapon = 19,
	WeaponId_WeaponSpawn = 8
};

enum m_classes
{
	ET_INVALID = -1,
	ET_WORLD = 261,
	ET_TerrorGameRulesProxy = 229,
	ET_TerrorPlayerResource = 233,
	ET_PlayerResource = 133,
	ET_CSGameRulesProxy = 47,
	ET_GameRulesProxy = 93,
	ET_FogController = 75,
	ET_CTERRORPLAYER = 232,
	ET_SURVIVORBOT = 275,
	ET_INFECTED = 264,
	ET_WITCH = 277,
	ET_BOOMER = 0,
	ET_TANK = 276,
	ET_JOCKEY = 265,
	ET_SPITTER = 272,
	ET_CHARGER = 99,
	ET_HUNTER = 263,
	ET_SMOKER = 270,
	ET_TankRock = 13,
	ET_VomitJarProjectile = 252,
	ET_SpitterProjectile = 176,
	ET_PipeBombProjectile = 130,
	ET_MolotovProjectile = 119,
	ET_GrenadeProjectile = 97,
	ET_DoorCheckpoint = 143,
	ET_SurvivorRescue = 186,
	ET_SurvivorDeathModel = 184,
	ET_PropHealthCabinet = 144,
	ET_CTongue = 246,
	ET_Particle = 124,
	ET_WeaponMountedGun = 146,
	ET_WeaponMinigun = 145,
	ET_WeaponAmmoSpawn = 256,
	ET_WeaponSpawn = 260,
	ET_ScavengeItemSpawn = 259,
	ET_BaseUpgradeItem = 29,
	ET_WeaponMP5 = 165,
	ET_WeaponSilenced = 166,
	ET_WeaponAuto = 2,
	ET_WeaponSpas = 163,
	ET_WeaponChrome = 162,
	ET_WeaponPump = 148,
	ET_WeaponAK47 = 152,
	ET_WeaponDesert = 153,
	ET_WeaponSG552 = 155,
	ET_WeaponRifle = 1,
	ET_WeaponM60 = 154,
	ET_WeaponGrenadeLauncher = 96,
	ET_WeaponScout = 171,
	ET_WeaponMilitary = 170,
	ET_WeaponAWP = 169,
	ET_WeaponMagnum = 116,
	ET_WeaponPistol = 131,
	ET_WeaponChainsaw = 39,
	ET_WeaponMelee = 231,
	ET_WeaponPipeBomb = 129,
	ET_WeaponMolotov = 118,
	ET_WeaponVomitjar = 106,
	ET_ProjectilePipeBomb = 130,
	ET_ProjectileMolotov = 119,
	ET_ProjectileVomitJar = 252,
	ET_ProjectileSpitter = 176,
	ET_ProjectileGrenadeLauncher = 97,
	ET_ProjectileGrenade = 13,
	ET_WeaponIncendiary = 111,
	ET_WeaponExplosive = 110,
	ET_WeaponDefibrillator = 109,
	ET_WeaponFirstAidKit = 73,
	ET_WeaponAmmoPack = 107,
	ET_WeaponPainPills = 121,
	ET_WeaponAdrenaline = 105,
	ET_WeaponOxygen = 120,
	ET_WeaponGnome = 95,
	ET_WeaponGascan = 94,
	ET_WeaponFirework = 72,
	ET_WeaponCola = 44,
	ET_PhysicsProp = 127,
	ET_WeaponPropaneTank = 142,
};

enum SolidFlags_t {
	FSOLID_CUSTOMRAYTEST = 0x0001,
	FSOLID_CUSTOMBOXTEST = 0x0002,
	FSOLID_NOT_SOLID = 0x0004,
	FSOLID_TRIGGER = 0x0008,
	FSOLID_NOT_STANDABLE = 0x0010,
	FSOLID_VOLUME_CONTENTS = 0x0020,
	FSOLID_FORCE_WORLD_ALIGNED = 0x0040,
	FSOLID_USE_TRIGGER_BOUNDS = 0x0080,
	FSOLID_ROOT_PARENT_ALIGNED = 0x0100,
	FSOLID_TRIGGER_TOUCH_DEBRIS = 0x0200,

	FSOLID_MAX_BITS = 10
};

class c_base_entity {
public:
	INETVAR("DT_BaseEntity", "m_iTeamNum", team, int);
	INETVAR("DT_BaseEntity", "m_vecOrigin", origin, vec3);
	INETVAR("DT_BaseEntity", "m_vecViewOffset[0]", view_offset, vec3);

	INETVAR("DT_BaseEntity", "m_vecMins", vec_mins, vec3);
	INETVAR("DT_BaseEntity", "m_vecMaxs", vec_maxs, vec3);

	void* networkable();
	collideable_t* get_collideable();
	vec3 get_absolute_origin();
	vec3 get_punch_angle();
	c_client_class* client_class();
	bool is_dormant();
	vec3 get_eye_pos();
	matrix3x4_t* get_bone_matrix();
	vec3 get_bone_position(int bone);
};

class keyvalues {
public:
	char __pad00[36];
};

struct file_weapon_info_t {
private:
	byte pad_0x0000[0x4];
public:
	bool bParsedScript;
	bool bLoadedHudElements;
	char szClassName[80];
	char szPrintName[80];
	char szViewModel[80];
	char szWorldModel[80];
	char szAnimationPrefix[16];
	std::int16_t pad1;
	std::int32_t iSlot;
	std::int32_t iPosition;
	std::int32_t iMaxClip1;
	std::int32_t iMaxClip2;
	std::int32_t iDefaultClip1;
	std::int32_t iDefaultClip12;
	std::int32_t iWeight;
	std::int32_t iRumble;
	bool bAutoSwitchTo;
	bool bAutoSwitchFrom;
	std::int32_t iFlags;
};

static struct weapon_info_t {
	const char* name;
};

static weapon_info_t weap_list[] =
{
	"",
	"pistol",
	"uzi",
	"pump shotgun",
	"auto shotgun",
	"m16a1",
	"hunting rifle",
	"mac10",
	"chrome shotgun",
	"scar",
	"military sniper",
	"spas",
	"firstaid",
	"molotov",
	"pipebomb",
	"pills",
	"gascan",
	"propane tank",
	"oxygen tank",
	"",
	"chainsaw",
	"grenade launcher",
	"",
	"adrenaline",
	"defibrillator",
	"vomitjar",
	"ak47",
	"",
	"",
	"firework crate",
	"incendiary ammo",
	"explosive ammo",
	"deagle",
	"mp5",
	"sg552",
	"awp",
	"scout",
	"m60",
	""
};

static weapon_info_t weap_icon_list[] =
{
	"",
	"C",
	"W",
	"M",
	"b",
	"Z",
	"O",
	"U",
	"b",
	"f",
	"X",
	"a",
	"v",
	"l",
	"p",
	"r",
	"o",
	"",
	"",
	"",
	"K",
	"T",
	"",
	"u",
	"s",
	"m",
	"Y",
	"",
	"",
	"",
	"R",
	"S",
	"D",
	"V",
	"c",
	"e",
	"d",
	"g",
	""
};

class c_base_weapon : public c_base_entity {
private:
	template <typename T>
	T& read(uintptr_t offset) {
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

public:

	INETVAR("DT_BaseCombatWeapon", "m_flNextPrimaryAttack", next_primary_attack, float);
	INETVAR("DT_BaseCombatWeapon", "m_flNextSecondaryAttack", next_secondary_attack, float);
	INETVAR("DT_BasePlayer", "m_bInReload", is_reloading, int);

	bool reloading()
	{
		return is_reloading() == 1;
	}
	int get_weapon_id();
	m_weapon_id get_item_definition_index();
	bool is_allowed_to_switch();
	bool can_be_selected();
	bool visible_in_weapon_selection();
	bool has_ammo();
	file_weapon_info_t* get_weapon_data();
};

class c_base_player : public c_base_entity {
private:
	template <typename T>
	T& read(uintptr_t offset) {
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	template <typename T>
	void write(uintptr_t offset, T data) {
		*reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset) = data;
	}
public:
	INETVAR("DT_BasePlayer", "m_iHealth", get_health, int);
	INETVAR("DT_BasePlayer", "m_lifeState", get_life_state, int);
	INETVAR("DT_BasePlayer", "m_fFlags", get_flags, int);
	INETVAR("DT_BasePlayer", "m_vecOrigin", get_vec_origin, vec3);
	INETVAR("DT_BasePlayer", "m_vecViewOffset[0]", get_view_offset, vec3);
	INETVAR("DT_BasePlayer", "m_isDying", get_life_dying, int);
	INETVAR("DT_BasePlayer", "deadflag", get_deadflag, int);
	INETVAR("DT_BasePlayer", "m_iClip1", get_clip1, int);
	INETVAR("DT_BasePlayer", "m_iClip2", get_clip2, int);
	INETVAR("DT_BasePlayer", "m_nTickBase", get_tick_base, int);
	INETVAR("DT_BasePlayer", "m_nSequence", get_sequence, int);
	INETVAR("DT_BasePlayer", "m_iMaxHealth", get_maxhealth, int);
	INETVAR("DT_Local", "m_vecPunchAngle", get_punch_angle, vec3);

	bool is_alive() // only works for survivors and special infected I think
	{
		return get_life_state() == LIFE_ALIVE;
	}
	bool is_dying()
	{
		return *(int*)((uintptr_t)this + 0xE0) != 0;
	}
	bool is_dead() // this checks ONLY FOR INFECTED! if it's dead
	{
		return *(int*)((uintptr_t)this + 0x144) == 0;
	}
	bool rip_limb()
	{
		return *(int*)((uintptr_t)this + 0x664) == 1;
	}
	int shot_count() // this shows how many times an infected got shot
	{
		return *(int*)((uintptr_t)this + 0x66C) > 2;
	}
	vec3 get_eye_pos();
	c_base_weapon* get_active_weapon();
	bool SetupBones(matrix3x4_t * out, int maxBones, int boneMask, float curTime);
};