/*
    Copyright 2001 to 2004. The Battle Grounds Team and Contributors

    This file is part of the Battle Grounds Modification for Half-Life.

    The Battle Grounds Modification for Half-Life is free software;
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser General Public License as published by the Free
    Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    The Battle Grounds Modification for Half-Life is distributed in
    the hope that it will be useful, but WITHOUT ANY WARRANTY; without
    even the implied warranty of MERCHANTABILITY or FITNESS FOR A
    PARTICULAR PURPOSE.  See the GNU Lesser General Public License
    for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with The Battle Grounds Modification for Half-Life;
    if not, write to the Free Software Foundation, Inc., 59 Temple Place,
    Suite 330, Boston, MA  02111-1307  USA

    You must obey the GNU Lesser General Public License in all respects for
    all of the code used other than code distributed with the Half-Life
    SDK developed by Valve.  If you modify this file, you may extend this
    exception to your version of the file, but you are not obligated to do so.
    If you do not wish to do so, delete this exception statement from your
    version.
*/

//This file was created on the 6/9/2k+1 at 14:38 by Ben Banfield for the Battlegrounds Modification of Half-Life
//The purpose of this file to handle the class in Battlegrounds
//The actual implementation of this was inspired by a coder going by the name of Wolfman on the Final Judgement modification

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"player.h"

#include	"gamerules.h"
#include	"bg_rules.h"
#include	"bg_class.h"

CBaseClass::CBaseClass(CBasePlayer *pPlayer)
{
}	

CBaseClass::~CBaseClass()
{
}

//BRITISH CLASSES
CLight_B::CLight_B(CBasePlayer *pPlayer) : CBaseClass(pPlayer)
{
	//MODEL CODE
	sprintf(class_code, "%s", szClassCodes[BRITS][LIGHT]);

	char temp[64];
	sprintf(temp, "models/player/%s/%s.mdl", class_code, class_code);

	g_engfuncs.pfnSetClientKeyValue( pPlayer->entindex(), g_engfuncs.pfnGetInfoKeyBuffer( pPlayer->edict() ), "model", class_code);

	//NAME CODE
	sprintf(pPlayer->m_szClassName, "%s", szClassNames[BRITS][LIGHT]);


	//NUMBER CODE
	pPlayer->pev->playerclass = pPlayer->pev->playerclass = LIGHT;


	//WEAPONS CODE
	pPlayer->GiveNamedItem( "weapon_Sabre" );
	pPlayer->GiveNamedItem( "weapon_PistolB" );
	//OTHER CODE - Command set up, any special vguis of anything, etc
}

CMedium_B::CMedium_B(CBasePlayer *pPlayer) : CBaseClass(pPlayer)
{
	//MODEL CODE
	sprintf(class_code, "%s", szClassCodes[BRITS][MEDIUM]);

	char temp[64];
	sprintf(temp, "models/player/%s/%s.mdl", class_code, class_code);

	g_engfuncs.pfnSetClientKeyValue( pPlayer->entindex(), g_engfuncs.pfnGetInfoKeyBuffer( pPlayer->edict() ), "model", class_code);

	//NAME CODE
	sprintf(pPlayer->m_szClassName, "%s", szClassNames[BRITS][MEDIUM]);


	//NUMBER CODE
	pPlayer->pev->playerclass = pPlayer->pev->playerclass = MEDIUM;

	//WEAPONS CODE
	pPlayer->GiveNamedItem( "weapon_BrownBess" );

	//OTHER CODE - Command set up, any special vguis of anything, etc
}

CHeavy_B::CHeavy_B(CBasePlayer *pPlayer) : CBaseClass(pPlayer)
{
	//MODEL CODE
	sprintf(class_code, "%s", szClassCodes[BRITS][HEAVY]);

	char temp[64];
	sprintf(temp, "models/player/%s/%s.mdl", class_code, class_code);

	g_engfuncs.pfnSetClientKeyValue( pPlayer->entindex(), g_engfuncs.pfnGetInfoKeyBuffer( pPlayer->edict() ), "model", class_code);

	//NAME CODE
	sprintf(pPlayer->m_szClassName, "%s", szClassNames[BRITS][HEAVY]);


	//NUMBER CODE;
	pPlayer->pev->playerclass = pPlayer->pev->playerclass = HEAVY;

	//WEAPONS CODE
	pPlayer->GiveNamedItem( "weapon_Knife" );
	pPlayer->GiveNamedItem( "weapon_Pennsylvania" );

	//OTHER CODE - Command set up, any special vguis of anything, etc
}

//AMERICAN CLASSES
CLight_A::CLight_A(CBasePlayer *pPlayer) : CBaseClass(pPlayer)
{
	//MODEL CODE
	sprintf(class_code, "%s", szClassCodes[AMERS][LIGHT]);

	char temp[64];
	sprintf(temp, "models/player/%s/%s.mdl", class_code, class_code);

	g_engfuncs.pfnSetClientKeyValue( pPlayer->entindex(), g_engfuncs.pfnGetInfoKeyBuffer( pPlayer->edict() ), "model", class_code);

	//NAME CODE
	sprintf(pPlayer->m_szClassName, "%s", szClassNames[AMERS][LIGHT]);


	//NUMBER CODE
	pPlayer->pev->playerclass = pPlayer->pev->playerclass = LIGHT;

	//WEAPONS CODE
	pPlayer->GiveNamedItem( "weapon_Sabre" );
	pPlayer->GiveNamedItem( "weapon_PistolA" );

	//OTHER CODE - Command set up, any special vguis of anything, etc
}

CMedium_A::CMedium_A(CBasePlayer *pPlayer) : CBaseClass(pPlayer)
{
	//MODEL CODE
	sprintf(class_code, "%s", szClassCodes[AMERS][MEDIUM]);

	char temp[64];
	sprintf(temp, "models/player/%s/%s.mdl", class_code, class_code);

	g_engfuncs.pfnSetClientKeyValue( pPlayer->entindex(), g_engfuncs.pfnGetInfoKeyBuffer( pPlayer->edict() ), "model", class_code);

	//NAME CODE
	sprintf(pPlayer->m_szClassName, "%s", szClassNames[AMERS][MEDIUM]);


	//NUMBER CODE
	pPlayer->pev->playerclass = pPlayer->pev->playerclass = MEDIUM;

	//WEAPONS CODE
	pPlayer->GiveNamedItem( "weapon_Revolutionnaire" );

	//OTHER CODE - Command set up, any special vguis of anything, etc
}

CHeavy_A::CHeavy_A(CBasePlayer *pPlayer) : CBaseClass(pPlayer)
{
	//MODEL CODE
	sprintf(class_code, "%s", szClassCodes[AMERS][HEAVY]);

	char temp[64];
	sprintf(temp, "models/player/%s/%s.mdl", class_code, class_code);

	g_engfuncs.pfnSetClientKeyValue( pPlayer->entindex(), g_engfuncs.pfnGetInfoKeyBuffer( pPlayer->edict() ), "model", class_code);


	//NAME CODE
	sprintf(pPlayer->m_szClassName, "%s", szClassNames[AMERS][HEAVY]);


	//NUMBER CODE
	pPlayer->pev->playerclass = pPlayer->pev->playerclass = HEAVY;

	//WEAPONS CODE
	pPlayer->GiveNamedItem( "weapon_Charleville" );

	//OTHER CODE - Command set up, any special vguis of anything, etc
}
