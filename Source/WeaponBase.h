/*
 *  WeaponBase.h
 *  SDL04_Station
 */

#pragma once

enum	WeaponTypes
{
	SmallProjectile, MediumProjectile, LargeProjectile,
	Rocket, Missile, Drone, 
	Flame, Electrical, Chemical, ElectroChemical,
	Phaze, // diplaces the object in time and/or space
	Plasma, // superheated mass of protons
	Lazer, // highly charged photons
	NeutronBeam, // kills all life and weakens metals. 
};

enum	WeaponDamageType
{
	MassDamage,		// takes out a chunck, explodes, etc
	HeatDamage,		// fire, melting, etc
	ShieldDamage,	// attacks the shields directly
	ElectricalDamage,// may affect systems only. Probably does no physical damage
	ChemicalDamage, // rust, disolving, possibly melting
	TimeDamage,		// this might set the victim back in time or forward a few seconds, apparently disappearing
	LocativeDamage	// the victim is displaced by some small distance.
};

enum	WeaponResistanceType
{
	BlocksPercentage, // 10% applied to 80 damage means that only 72 is done (must be between 0 and 1)
	BlocksFirstAmount, // first 10 applied means that only 70 is done
	BlocksLastAmount,	// same as BlocksFirstAmount except in the case of multiple resistances
						// in which this resistance is applied last as a value (non-percentage).
};

struct WeaponResistance
{
	WeaponTypes				Weapon;
	WeaponDamageType		Damage;
	WeaponResistanceType	Resistance;
	float					Value;
};

struct DamageTaken 
{
	WeaponTypes				Weapon;
	WeaponDamageType		Damage;
	float					Points;
};

//class