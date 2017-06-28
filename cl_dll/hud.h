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

/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

//			
//  hud.h
//
// class CHud declaration
//
// CHud handles the message, calculation, and drawing the HUD
//

#define FOG_LIMIT 30000

#define RGB_YELLOWISH 0x00FFA000 //255,160,0
#define RGB_REDISH 0x00FF1010 //255,16,16
#define RGB_GREENISH 0x0000A000 //0,160,0
#define RGB_WHITEISH 0x00FFFFFF //0, 0, 0
#define RGB_BLUEISH 0x004273F7 //66,115,247

#include "wrect.h"
#include "cl_dll.h"
#include "ammo.h"

#define DHN_DRAWZERO 1
#define DHN_2DIGITS  2
#define DHN_3DIGITS  4
#define DHN_10DIGITS  10
#define DHN_PREFIXZERO 8 //Baer---clock
#define MIN_ALPHA	 100	

#define		HUDELEM_ACTIVE	1

typedef struct {
	int x, y;
} POSITION;

enum player_info
{ 
	MAX_PLAYERS = 64,
};

typedef struct {
	unsigned char r,g,b,a;
} RGBA;

typedef struct cvar_s cvar_t;


#define HUD_ACTIVE	1
#define HUD_INTERMISSION 2

#define MAX_PLAYER_NAME_LENGTH		32

#define	MAX_MOTD_LENGTH				1536

//
//-----------------------------------------------------
//
class CHudBase
{
public:
	POSITION  m_pos;
	int   m_type;
	int	  m_iFlags; // active, moving, 
	virtual		~CHudBase() {}
	virtual int Init( void ) {return 0;}
	virtual int VidInit( void ) {return 0;}
	virtual int Draw(float flTime) {return 0;}
	virtual void Think(void) {return;}
	virtual void Reset(void) {return;}
	virtual void InitHUDData( void ) {}		// called every time a server is connected to
};

struct HUDLIST {
	CHudBase	*p;
	HUDLIST		*pNext;
};



//
//-----------------------------------------------------
//
#include "..\game_shared\voice_status.h"
#include "hud_spectator.h"


//
//-----------------------------------------------------
//
class CHudAmmo: public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw(float flTime);
	void Think(void);
	void Reset(void);
	int DrawWList(float flTime);
	int MsgFunc_CurWeapon(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_WeaponList(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_AmmoX(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_AmmoPickup( const char *pszName, int iSize, void *pbuf );
	int MsgFunc_WeapPickup( const char *pszName, int iSize, void *pbuf );
	int MsgFunc_ItemPickup( const char *pszName, int iSize, void *pbuf );
	int MsgFunc_HideWeapon( const char *pszName, int iSize, void *pbuf );

	void SlotInput( int iSlot );
	void _cdecl UserCmd_Slot1( void );
	void _cdecl UserCmd_Slot2( void );
	void _cdecl UserCmd_Slot3( void );
	void _cdecl UserCmd_Slot4( void );
	void _cdecl UserCmd_Slot5( void );
	void _cdecl UserCmd_Slot6( void );
	void _cdecl UserCmd_Slot7( void );
	void _cdecl UserCmd_Slot8( void );
	void _cdecl UserCmd_Slot9( void );
	void _cdecl UserCmd_Slot10( void );
	void _cdecl UserCmd_Close( void );
	void _cdecl UserCmd_NextWeapon( void );
	void _cdecl UserCmd_PrevWeapon( void );

	WEAPON* ReturnWeapon() { if(m_pWeapon) return m_pWeapon; else return NULL; }

private:
	float m_fFade;
	RGBA  m_rgba;
	WEAPON *m_pWeapon;
	int	m_HUD_bucket0;
	int m_HUD_selection;

};

//
//-----------------------------------------------------
//

class CHudAmmoSecondary: public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	void Reset( void );
	int Draw(float flTime);

	int MsgFunc_SecAmmoVal( const char *pszName, int iSize, void *pbuf );
	int MsgFunc_SecAmmoIcon( const char *pszName, int iSize, void *pbuf );

private:
	enum {
		MAX_SEC_AMMO_VALUES = 4
	};

	int m_HUD_ammoicon; // sprite indices
	int m_iAmmoAmounts[MAX_SEC_AMMO_VALUES];
	float m_fFade;
};


#include "health.h"

#define FADE_TIME 100

//
//-----------------------------------------------------
//
class CHudGeiger: public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw(float flTime);
	int MsgFunc_Geiger(const char *pszName, int iSize, void *pbuf);
	
private:
	int m_iGeigerRange;

};

//
//-----------------------------------------------------
//
class CHudTrain: public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw(float flTime);
	int MsgFunc_Train(const char *pszName, int iSize, void *pbuf);

private:
	HSPRITE m_hSprite;
	int m_iPos;

};


//
//-----------------------------------------------------
//
// REMOVED: Vgui has replaced this.
//
/*
class CHudMOTD : public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw( float flTime );
	void Reset( void );

	int MsgFunc_MOTD( const char *pszName, int iSize, void *pbuf );

protected:
	static int MOTD_DISPLAY_TIME;
	char m_szMOTD[ MAX_MOTD_LENGTH ];
	float m_flActiveRemaining;
	int m_iLines;
};
*/

//
//-----------------------------------------------------
//
class CHudStatusBar : public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw( float flTime );
	void Reset( void );
	void ParseStatusString( int line_num );

	int MsgFunc_StatusText( const char *pszName, int iSize, void *pbuf );
	int MsgFunc_StatusValue( const char *pszName, int iSize, void *pbuf );

protected:
	enum { 
		MAX_STATUSTEXT_LENGTH = 128,
		MAX_STATUSBAR_VALUES = 8,
		MAX_STATUSBAR_LINES = 2,
	};

	char m_szStatusText[MAX_STATUSBAR_LINES][MAX_STATUSTEXT_LENGTH];  // a text string describing how the status bar is to be drawn
	char m_szStatusBar[MAX_STATUSBAR_LINES][MAX_STATUSTEXT_LENGTH];	// the constructed bar that is drawn
	int m_iStatusValues[MAX_STATUSBAR_VALUES];  // an array of values for use in the status bar

	int m_bReparseString; // set to TRUE whenever the m_szStatusBar needs to be recalculated

	// an array of colors...one color for each line
	float *m_pflNameColors[MAX_STATUSBAR_LINES];
};

struct extra_player_info_t 
{
	bool dead;
	signed long int damage_done;
	signed long int capture_points;
	short deaths;
	short playerclass;
	short teamnumber;
	char teamname[NAME_LENGTH];
};

struct team_info_t 
{
	char name[NAME_LENGTH];
	signed long int damage_done;
	signed long int capture_points;
	short deaths;
	short ping;
	short packetloss;
	bool ownteam;
	short players;
	bool already_drawn;
	int teamnumber;
	signed long int extra_points;
	signed long int extra_damage;
	short extra_deaths;
	bool scores_overridden;
};

extern hud_player_info_t	g_PlayerInfoList[MAX_PLAYERS+1];	   // player info from the engine
extern extra_player_info_t  g_PlayerExtraInfo[MAX_PLAYERS+1];   // additional player info sent directly to the client dll
extern team_info_t			g_TeamInfo[NUM_TEAMS+2];
extern int					g_IsSpectator[MAX_PLAYERS+1];


//
//-----------------------------------------------------
//
class CHudDeathNotice : public CHudBase
{
public:
	int Init( void );
	void InitHUDData( void );
	int VidInit( void );
	int Draw( float flTime );
	int MsgFunc_DeathMsg( const char *pszName, int iSize, void *pbuf );

private:
	int m_HUD_d_skull;  // sprite index of skull icon
};

//
//-----------------------------------------------------
//
class CHudMenu : public CHudBase
{
public:
	int Init( void );
	void InitHUDData( void );
	int VidInit( void );
	void Reset( void );
	int Draw( float flTime );
	int MsgFunc_ShowMenu( const char *pszName, int iSize, void *pbuf );

	void SelectMenuItem( int menu_item );

	int m_fMenuDisplayed;
	int m_bitsValidSlots;
	float m_flShutoffTime;
	int m_fWaitingForMore;
};

//
//-----------------------------------------------------
//
class CHudSayText : public CHudBase
{
public:
	int Init( void );
	void InitHUDData( void );
	int VidInit( void );
	int Draw( float flTime );
	int MsgFunc_SayText( const char *pszName, int iSize, void *pbuf );
	void SayTextPrint( const char *pszBuf, int iBufSize, int clientIndex = -1 );
	void EnsureTextFitsInOneLineAndWrapIfHaveTo( int line );
friend class CHudSpectator;
private:

	struct cvar_s *	m_HUD_saytext;
	struct cvar_s *	m_HUD_saytext_time;
};

//Ben- NO battery in BG


//BP - Hintbox
class CHudHintbox : public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw( float fltime );
	void Reset() {}
	int Set(int preset);
	int m_HUD_hintbox;
	int m_HUD_hintbox_back;
	bool m_bvisible;
	int m_iSetting;
	int m_iSettingPlayed[255];
	float m_fLastTime;
	float StartTime;
	int displaymode;
private:
	HSPRITE m_hSprite;
	HSPRITE m_hSprite_back;
	wrect_t *m_SpriteArea;
	wrect_t *m_SpriteArea_back;
};

//Ben - Hud background
class CHudBackground : public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw( float fltime );
	void Reset() {}
	int m_HUD_back;
private:
	HSPRITE m_hSprite;
	int m_HUD_water;
	wrect_t *m_SpriteArea;
};

//Ben - Stamina
class CHudStamina : public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw( float fltime );
	void Reset() {}
	int m_HUD_stamina;
private:
	HSPRITE m_hSprite;
	wrect_t *m_SpriteArea;
};

//
//------------------------------------------------------
//Baer
class CHudClock : public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw(float flTime);
	void Reset(void);
	int MsgFunc_ClockTime(const char *pszName, int iSize, void *pbuf);
	int m_HUD_Sprite;
	int m_HUD_Colon;

	int m_iCurrentClockTime;
	int m_iClockTime;
	// ben - pause or round time
	int m_iClockState;
	cvar_t *g_ShowClock;
	int m_iLastClockUpdate;
private:
	// ben - all sprites
	HSPRITE m_hColonSprite;
	wrect_t *m_ColonArea;
};
//Baer

struct point_s
{
	// all points
	char name[256];
	int iID;
	int iType;
	int iStatus;
	int iTeamAllowed;
	int iTeamInvolved;

	// capture point specific
	int iPlayersInPoint;
	int iPlayersNeeded;
	int iTimeAdded;
	float flTimeCaptureStarted;
	int iTimeNeededForCapture;

	// destruction point specific
	int iHealthStarted;
	int iHealth;

	// escape point specif
};

//Ben - Declarations of the class which will display the points status
class CHudPoints : public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw( float fltime );
	void Reset() {}
	int MsgFunc_Points(const char *pszName, int iSize, void *pbuf);

	void ResetAllPoints(void);

	void StartClock(int iId);
	void ClearClock(int iId);

	int ReturnStatusById( int iId );
	char *ReturnNameById(int iId);
private:
	void DrawPoint(point_s *point, float flTime, int &x, int &y);

	int m_HUD_IconWhite;
	int m_HUD_IconRed;
	int m_HUD_IconBlue;

	HSPRITE m_hIconWhite;
	HSPRITE m_hIconRed;
	HSPRITE m_hIconBlue;
	HSPRITE m_hClock;
	HSPRITE m_hFillUp;

	wrect_t *m_pAreaIconWhite;
	wrect_t *m_pAreaIconRed;
	wrect_t *m_pAreaIconBlue;
	wrect_t *m_pAreaClock;
	wrect_t *m_pAreaFillUp;

	bool m_bInvolvedInCapture;
	int m_iInvolvedInCaptureId;
	point_s *m_aPoints[MAX_POINTS];
};

class CHudMiddle : public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw( float fltime );
	void Reset() {}
	int m_HUD_Middle;
private:
	HSPRITE m_hSprite;
	wrect_t *m_SpriteArea;
};

struct message_s
{
	char szMessage[256];
	float flTimeAdded;
	float flLifeSpan;
	int x;
	int y;
	int r;
	int g;
	int b;
};

class CHudColourMessage :public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw( float fltime );
	void Reset() {}
	int MsgFunc_Colour(const char *pszName, int iSize, void *pbuf);
	void AddMessage(int r, int g, int b, int x, int y, char *text, float life);
private:
	message_s *m_pMessages[MAX_MESSAGES+1];
};

class CHudCrosshair : public CHudBase
{
public:
        int Init( void );
        int VidInit( void );
        int Draw( float fltime );
        int m_HUD_Crosshair1;
        int m_HUD_Crosshair2;
        int m_HUD_Crosshair3;
        int m_HUD_Crosshair4;
        int m_HUD_Crosshair5;
        int m_HUD_Crosshair6;
		int m_HUD_CrosshairStatic1;
		int m_HUD_CrosshairStatic2;
		int m_HUD_CrosshairStatic3;
		int m_HUD_CrosshairStatic4;
		int m_HUD_CrosshairStatic5;
		int m_HUD_CrosshairStatic6;
		int m_HUD_CrosshairCustom1;
		int m_HUD_CrosshairCustom2;
		int m_HUD_CrosshairCustom3;
private:
        HSPRITE m_hSprite1;
        HSPRITE m_hSprite2;
        HSPRITE m_hSprite3;
        HSPRITE m_hSprite4;
        HSPRITE m_hSprite5;
        HSPRITE m_hSprite6;
		HSPRITE m_hSpriteStatic1;
		HSPRITE m_hSpriteStatic2;
		HSPRITE m_hSpriteStatic3;
		HSPRITE m_hSpriteStatic4;
		HSPRITE m_hSpriteStatic5;
		HSPRITE m_hSpriteStatic6;
		HSPRITE	m_hSpriteCustom1;
		HSPRITE m_hSpriteCustom2;
		HSPRITE m_hSpriteCustom3;

        wrect_t *m_SpriteAreaDyn1;
		wrect_t *m_SpriteAreaStat1;
		wrect_t *m_SpriteAreaStat2;
		wrect_t *m_SpriteAreaStat3;
		wrect_t *m_SpriteAreaStat4;
		wrect_t *m_SpriteAreaStat5;
		wrect_t *m_SpriteAreaStat6;
		wrect_t *m_SpriteAreaCustom1;
		wrect_t *m_SpriteAreaCustom2;
		wrect_t *m_SpriteAreaCustom3;

		float m_flLastTime;
		float m_flMoving;

};

class CHudCommanderMenu : public CHudBase
{
public:
		int Init( void );
		int Die( void );
		int VidInit( void );
		int Draw( float fltime );
		int PreExecute(int special);
		int PostExecute(int special);
		int Fill(int special);
private:
		int m_iPointsPerSecond;
		float m_flTotalPoints;
		float m_flSpendablePoints;
		float m_flLastTime;
		int m_iRush;
		int m_iRush_Max;
		bool m_bAddedRush;
		int m_iArtillery;
		int m_iArtillery_Max;
		bool m_bAddedArtillery;
		bool m_bRushMessage;
		bool m_bArtilleryMessage;
		bool m_bResetTime;
};

class CPredefinedMessage : public CHudBase
{
public:
        int Init( void );
        int VidInit( void );
		int MsgFunc_MakeMessage(const char *pszName, int iSize, void *pbuf);
protected:
		bool m_PointEntered;
		int m_PointIdEntered;
};


//
//-----------------------------------------------------
//
class CHudFlashlight: public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw(float flTime);
	void Reset( void );
	int MsgFunc_Flashlight(const char *pszName,  int iSize, void *pbuf );
	int MsgFunc_FlashBat(const char *pszName,  int iSize, void *pbuf );
	
private:
	HSPRITE m_hSprite1;
	HSPRITE m_hSprite2;
	HSPRITE m_hBeam;
	wrect_t *m_prc1;
	wrect_t *m_prc2;
	wrect_t *m_prcBeam;
	float m_flBat;	
	int	  m_iBat;	
	int	  m_fOn;
	float m_fFade;
	int	  m_iWidth;		// width of the battery innards
};
class CHudArmLegShot: public CHudBase
{
public: 
	int Init( void );
	int VidInit( void ); 
	int Draw(float flTime); 
	int MsgFunc_ArmLegShot(const char *pszName, int iSize, void *pbuf );

private: 
	int m_HUD_armlegshot;
	int m_iShotCount;
	int m_iShots[10];
	HSPRITE m_hSprite;
	wrect_t *m_SpriteArea;
};


//
//-----------------------------------------------------
//
const int maxHUDMessages = 16;
struct message_parms_t
{
	client_textmessage_t	*pMessage;
	float	time;
	int x, y;
	int	totalWidth, totalHeight;
	int width;
	int lines;
	int lineLength;
	int length;
	int r, g, b;
	char text;
	int fadeBlend;
	float charTime;
	float fadeTime;
};

//
//-----------------------------------------------------
//

class CHudTextMessage: public CHudBase
{
public:
	int Init( void );
	static char *LocaliseTextString( const char *msg, char *dst_buffer, int buffer_size );
	static char *BufferedLocaliseTextString( const char *msg );
	char *LookupString( const char *msg_name, int *msg_dest = NULL );
	int MsgFunc_TextMsg(const char *pszName, int iSize, void *pbuf);
};

//
//-----------------------------------------------------
//

class CHudMessage: public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	int Draw(float flTime);
	int MsgFunc_HudText(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_GameTitle(const char *pszName, int iSize, void *pbuf);

	float FadeBlend( float fadein, float fadeout, float hold, float localTime );
	int	XPosition( float x, int width, int lineWidth );
	int YPosition( float y, int height );

	void MessageAdd( const char *pName, float time );
	void MessageAdd(client_textmessage_t * newMessage );
	void MessageDrawScan( client_textmessage_t *pMessage, float time );
	void MessageScanStart( void );
	void MessageScanNextChar( void );
	void Reset( void );

private:
	client_textmessage_t		*m_pMessages[maxHUDMessages];
	float						m_startTime[maxHUDMessages];
	message_parms_t				m_parms;
	float						m_gameTitleTime;
	client_textmessage_t		*m_pGameTitle;

	int m_HUD_title_life;
	int m_HUD_title_half;
};

//
//-----------------------------------------------------
//
#define MAX_SPRITE_NAME_LENGTH	24

class CHudStatusIcons: public CHudBase
{
public:
	int Init( void );
	int VidInit( void );
	void Reset( void );
	int Draw(float flTime);
	int MsgFunc_StatusIcon(const char *pszName, int iSize, void *pbuf);

	enum { 
		MAX_ICONSPRITENAME_LENGTH = MAX_SPRITE_NAME_LENGTH,
		MAX_ICONSPRITES = 4,
	};

	
	//had to make these public so CHud could access them (to enable concussion icon)
	//could use a friend declaration instead...
	void EnableIcon( char *pszIconName, unsigned char red, unsigned char green, unsigned char blue );
	void DisableIcon( char *pszIconName );

private:

	typedef struct
	{
		char szSpriteName[MAX_ICONSPRITENAME_LENGTH];
		HSPRITE spr;
		wrect_t rc;
		unsigned char r, g, b;
	} icon_sprite_t;

	icon_sprite_t m_IconList[MAX_ICONSPRITES];

};

//
//-----------------------------------------------------
//



class CHud
{
private:
	HUDLIST						*m_pHudList;
	HSPRITE						m_hsprLogo;
	int							m_iLogo;
	client_sprite_t				*m_pSpriteList;
	int							m_iSpriteCount;
	int							m_iSpriteCountAllRes;
	float						m_flMouseSensitivity;
	int							m_iConcussionEffect; 

public:

	HSPRITE						m_hsprCursor;
	float m_flTime;	   // the current client time
	float m_fOldTime;  // the time at which the HUD was last redrawn
	double m_flTimeDelta; // the difference between flTime and fOldTime
	Vector	m_vecOrigin;
	Vector	m_vecAngles;
	int		m_iKeyBits;
	int		m_iHideHUDDisplay;
	int		m_iFOV;
	int		m_Teamplay;
	int		m_iRes;
	cvar_t  *m_pCvarStealMouse;
	cvar_t	*m_pCvarDraw;

	int m_iFontHeight;
	int DrawHudNumber(int x, int y, int iFlags, int iNumber, int r, int g, int b, int iSize = 0 );
	int DrawHudString(int x, int y, int iMaxX, char *szString, int r, int g, int b );
	int DrawHudStringReverse( int xpos, int ypos, int iMinX, char *szString, int r, int g, int b );
	int DrawHudNumberString( int xpos, int ypos, int iMinX, int iNumber, int r, int g, int b );
	int GetNumWidth(int iNumber, int iFlags);

	char* HandleServerString(char* szServerMsg);

private:
	// the memory for these arrays are allocated in the first call to CHud::VidInit(), when the hud.txt and associated sprites are loaded.
	// freed in ~CHud()
	HSPRITE *m_rghSprites;	/*[HUD_SPRITE_COUNT]*/			// the sprites loaded from hud.txt
	wrect_t *m_rgrcRects;	/*[HUD_SPRITE_COUNT]*/
	char *m_rgszSpriteNames; /*[HUD_SPRITE_COUNT][MAX_SPRITE_NAME_LENGTH]*/

	char m_szString[2048];

	struct cvar_s *default_fov;
public:
	HSPRITE GetSprite( int index ) 
	{
		return (index < 0) ? 0 : m_rghSprites[index];
	}

	wrect_t& GetSpriteRect( int index )
	{
		return m_rgrcRects[index];
	}

	
	int GetSpriteIndex( const char *SpriteName );	// gets a sprite index, for use in the m_rghSprites[] array

	//Ben
//	CHudRoundTime	m_RoundTime;
	CHudClock	m_Clock; // PAT: this seems to have to match the init call order, or the pointer is whacked
	//ben - bah you beat me to it. was wondering why i couldn't commit this file to cvs.
	// why does it work for m_points though?
//	CHudCommanderMenu m_CommanderMenu; // BP
	CPredefinedMessage m_PredefinedMessage;
	CHudCrosshair	m_Crosshair;
	CHudAmmo		m_Ammo;
	CHudHealth		m_Health;
	CHudSpectator		m_Spectator;
	CHudGeiger		m_Geiger;
	CHudTrain		m_Train;
	CHudFlashlight	m_Flash;
	CHudMessage		m_Message;
	CHudStatusBar   m_StatusBar;
	CHudDeathNotice m_DeathNotice;
	CHudSayText		m_SayText;
	CHudMenu		m_Menu;
	CHudAmmoSecondary	m_AmmoSecondary;
	CHudTextMessage m_TextMessage;
	CHudArmLegShot m_ArmLegShot;//Myristate
	CHudStatusIcons m_StatusIcons;
	CHudPoints	m_Points;
	CHudColourMessage m_ColourMessage;
	CHudHintbox	m_Hintbox;

	void Init( void );
	void VidInit( void );
	void Think(void);
	int Redraw( float flTime, int intermission );
	int UpdateClientData( client_data_t *cdata, float time );

	CHud() : m_iSpriteCount(0), m_pHudList(NULL) {}  
	~CHud();			// destructor, frees allocated memory

	// user messages
	int _cdecl MsgFunc_Damage(const char *pszName, int iSize, void *pbuf );
	int _cdecl MsgFunc_GameMode(const char *pszName, int iSize, void *pbuf );
	int _cdecl MsgFunc_Logo(const char *pszName,  int iSize, void *pbuf);
	int _cdecl MsgFunc_ResetHUD(const char *pszName,  int iSize, void *pbuf);
	void _cdecl MsgFunc_InitHUD( const char *pszName, int iSize, void *pbuf );
	void _cdecl MsgFunc_ViewMode( const char *pszName, int iSize, void *pbuf );
	void _cdecl MsgFunc_Dead( const char *pszName, int iSize, void *pbuf ); //Ben
	int _cdecl MsgFunc_SetFog( const char *pszName, int iSize, void *pbuf ); //LRC
	int _cdecl MsgFunc_SetFOV(const char *pszName,  int iSize, void *pbuf);
	int  _cdecl MsgFunc_Concuss( const char *pszName, int iSize, void *pbuf );
	// Ben Addition to Raiko Contrib - Melee Only
	void _cdecl MsgFunc_MeleeOnly( const char *pszName, int iSize, void *pbuf );
	// Ben End
	// Raiko Contrib - Stationary Reload
	void _cdecl MsgFunc_SReload(const char *pszName, int iSize, void *pbuf);
	// Raiko End
	// BP LaserTag
	void _cdecl MsgFunc_LaserTag( const char *pszName, int iSize, void *pbuf );
	// BP End
	
	// Screen information
	SCREENINFO	m_scrinfo;

	int	m_iWeaponBits;
	int	m_fPlayerDead;
	int m_iIntermission;

	// sprite indexes
	int m_HUD_number_0;

	void AddHudElem(CHudBase *p);

	float GetSensitivity();

};

class TeamFortressViewport;

extern CHud gHUD;
extern TeamFortressViewport *gViewPort;

extern int g_iPlayerClass;
extern int g_iTeamNumber;
extern int g_iUser1;
extern int g_iUser2;
extern int g_iUser3;
extern int g_iMeleeOnly;
extern int g_iLaserTag;
extern int g_iStationaryReload;
extern bool g_bCrouched;
extern float g_flHealth;
extern float g_flStamina;

