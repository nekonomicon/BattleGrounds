# Microsoft Developer Studio Project File - Name="hl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=hl - Win32 Release
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "bg_dll.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "bg_dll.mak" CFG="hl - Win32 Release"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "hl - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "hl - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "hl - Win32 Profile" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GoldSrc/dlls", ELEBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "hl - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "d:/temp/builddll"
# PROP Intermediate_Dir "d:/temp/builddll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G6 /MD /W4 /WX /Gi /GR /GX /Zi /O2 /Ob0 /I "..\game_shared" /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\dlls\bg_rules" /I "..\dlls\weapons" /I "..\dlls\points" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "QUIVER" /D "VOXEL" /D "QUAKE2" /D "VALVE_DLL" /D "CLIENT_WEAPONS" /FAcs /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /base:"0xc700000" /subsystem:windows /dll /incremental:yes /map /debug /machine:I386 /def:".\bg.def" /out:"C:\Steam\SteamApps\blackpanther@bgmod.com\half-life\bg-dev\dlls\bg.dll" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "hl - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\hl___Win"
# PROP BASE Intermediate_Dir ".\hl___Win"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "d:/temp/builddll"
# PROP Intermediate_Dir "d:/temp/builddll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G6 /MTd /W4 /Gm /Gi /GX /ZI /Od /I "..\game_shared" /I "..\\" /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\dlls\bg_rules" /I "..\dlls\weapons" /I "..\dlls\points" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "QUIVER" /D "VOXEL" /D "QUAKE2" /D "VALVE_DLL" /D "CLIENT_WEAPONS" /FAcs /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "..\engine" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 user32.lib advapi32.lib /nologo /base:"0xc700000" /subsystem:windows /dll /map /debug /machine:I386 /def:".\bg.def" /out:"C:\Steam\SteamApps\blackpanther@bgmod.com\half-life\bg-dev\dlls\bg.dll" /implib:".\Debug\hl.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "hl - Win32 Profile"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\hl___Win"
# PROP BASE Intermediate_Dir ".\hl___Win"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "d:/temp/builddll"
# PROP Intermediate_Dir "d:/temp/builddll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MT /W3 /GX /Zi /O2 /I "..\engine" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "QUIVER" /D "VOXEL" /D "QUAKE2" /D "VALVE_DLL" /YX /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MT /W3 /Gm /Gi /Zi /O2 /Ob0 /I "..\game_shared" /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\dlls\bg_rules" /I "..\dlls\weapons" /I "..\dlls\points" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "QUIVER" /D "VOXEL" /D "QUAKE2" /D "VALVE_DLL" /D "CLIENT_WEAPONS" /FAcs /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\hl.def"
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /base:"0xc700000" /subsystem:windows /dll /profile /map /debug /machine:I386 /def:".\bg.def" /out:"C:\Steam\SteamApps\blackpanther@bgmod.com\half-life\bg-dev\dlls\bg.dll" /MAPINFO:LINES /MAPINFO:EXPORTS

!ENDIF 

# Begin Target

# Name "hl - Win32 Release"
# Name "hl - Win32 Debug"
# Name "hl - Win32 Profile"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Group "bg_weapons"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\weapons\base_weapon.cpp
# End Source File
# Begin Source File

SOURCE=.\weapons\bayonet.cpp
# End Source File
# Begin Source File

SOURCE=.\weapons\brown_bess.cpp
# End Source File
# Begin Source File

SOURCE=.\weapons\charleville.cpp
# End Source File
# Begin Source File

SOURCE=.\weapons\ggrenade.cpp
# End Source File
# Begin Source File

SOURCE=.\weapons\knife.cpp
# End Source File
# Begin Source File

SOURCE=.\weapons\melee.cpp
# End Source File
# Begin Source File

SOURCE=.\weapons\pennsylvania.cpp
# End Source File
# Begin Source File

SOURCE=.\weapons\pistol_a.cpp
# End Source File
# Begin Source File

SOURCE=.\weapons\pistol_b.cpp
# End Source File
# Begin Source File

SOURCE=.\weapons\revolutionnaire.cpp
# End Source File
# Begin Source File

SOURCE=.\weapons\sabre.cpp
# End Source File
# Begin Source File

SOURCE=.\weapons\weapon_data.h
# End Source File
# Begin Source File

SOURCE=.\weapons\weapons.cpp
# End Source File
# Begin Source File

SOURCE=.\weapons\weapons.h
# End Source File
# End Group
# Begin Group "bg_rules"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bg_rules\ammo_ents.cpp
# End Source File
# Begin Source File

SOURCE=.\bg_rules\bg_class.cpp
# End Source File
# Begin Source File

SOURCE=.\bg_rules\bg_class.h
# End Source File
# Begin Source File

SOURCE=.\bg_rules\bg_defs.h
# End Source File
# Begin Source File

SOURCE=.\bg_rules\bg_observer.cpp
# End Source File
# Begin Source File

SOURCE=.\bg_rules\bg_respawnwaves.cpp
# End Source File
# Begin Source File

SOURCE=.\bg_rules\bg_round.cpp
# End Source File
# Begin Source File

SOURCE=.\bg_rules\bg_round.h
# End Source File
# Begin Source File

SOURCE=.\bg_rules\bg_round_container.cpp
# End Source File
# Begin Source File

SOURCE=.\bg_rules\bg_roundrules.cpp
# End Source File
# Begin Source File

SOURCE=.\bg_rules\bg_rules.cpp
# End Source File
# Begin Source File

SOURCE=.\bg_rules\bg_rules.h
# End Source File
# Begin Source File

SOURCE=.\bg_rules\bg_teams.cpp
# End Source File
# Begin Source File

SOURCE=.\bg_rules\info_reset.cpp
# End Source File
# Begin Source File

SOURCE=.\points\point_win.cpp
# End Source File
# Begin Source File

SOURCE=.\points\point_win.h
# End Source File
# Begin Source File

SOURCE=.\bg_rules\progressive_spawning.cpp
# End Source File
# End Group
# Begin Group "player_movement"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bg_rules\hulls.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_debug.c
# ADD CPP /W3
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_debug.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_defs.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_info.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_materials.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_math.c
# ADD CPP /W3
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_movevars.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_shared.c
# ADD CPP /W3
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_shared.h
# End Source File
# End Group
# Begin Group "points"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\points\base_point.cpp
# End Source File
# Begin Source File

SOURCE=.\points\capture_point.cpp
# End Source File
# Begin Source File

SOURCE=.\points\capture_point.h
# End Source File
# Begin Source File

SOURCE=.\points\point.h
# End Source File
# Begin Source File

SOURCE=.\points\point_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\points\point_model.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\animating.cpp
# End Source File
# Begin Source File

SOURCE=.\animation.cpp

!IF  "$(CFG)" == "hl - Win32 Release"

# ADD CPP /W3
# SUBTRACT CPP /WX

!ELSEIF  "$(CFG)" == "hl - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /WX

!ELSEIF  "$(CFG)" == "hl - Win32 Profile"

# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\bmodels.cpp
# End Source File
# Begin Source File

SOURCE=.\buttons.cpp
# End Source File
# Begin Source File

SOURCE=.\cbase.cpp
# End Source File
# Begin Source File

SOURCE=.\client.cpp
# End Source File
# Begin Source File

SOURCE=.\combat.cpp
# End Source File
# Begin Source File

SOURCE=.\doors.cpp
# End Source File
# Begin Source File

SOURCE=.\effects.cpp
# End Source File
# Begin Source File

SOURCE=.\explode.cpp
# End Source File
# Begin Source File

SOURCE=.\func_break.cpp
# End Source File
# Begin Source File

SOURCE=.\func_grass.cpp
# End Source File
# Begin Source File

SOURCE=.\func_tank.cpp
# End Source File
# Begin Source File

SOURCE=.\game.cpp
# End Source File
# Begin Source File

SOURCE=.\gamerules.cpp
# End Source File
# Begin Source File

SOURCE=.\globals.cpp
# End Source File
# Begin Source File

SOURCE=.\h_ai.cpp
# End Source File
# Begin Source File

SOURCE=.\h_cycler.cpp
# End Source File
# Begin Source File

SOURCE=.\h_export.cpp
# End Source File
# Begin Source File

SOURCE=.\items.cpp
# End Source File
# Begin Source File

SOURCE=.\lights.cpp
# End Source File
# Begin Source File

SOURCE=.\maprules.cpp
# End Source File
# Begin Source File

SOURCE=.\mortar.cpp
# End Source File
# Begin Source File

SOURCE=.\mpstubb.cpp
# End Source File
# Begin Source File

SOURCE=.\multiplay_gamerules.cpp
# End Source File
# Begin Source File

SOURCE=.\bg_rules\origin_tracker.cpp
# End Source File
# Begin Source File

SOURCE=.\particle_emitter.cpp
# End Source File
# Begin Source File

SOURCE=.\pathcorner.cpp
# End Source File
# Begin Source File

SOURCE=.\plane.cpp
# End Source File
# Begin Source File

SOURCE=.\plats.cpp
# End Source File
# Begin Source File

SOURCE=.\player.cpp
# End Source File
# Begin Source File

SOURCE=.\singleplay_gamerules.cpp
# End Source File
# Begin Source File

SOURCE=.\skill.cpp
# End Source File
# Begin Source File

SOURCE=.\sound.cpp
# End Source File
# Begin Source File

SOURCE=.\soundent.cpp
# End Source File
# Begin Source File

SOURCE=.\spectator.cpp
# End Source File
# Begin Source File

SOURCE=.\subs.cpp
# End Source File
# Begin Source File

SOURCE=.\triggers.cpp
# End Source File
# Begin Source File

SOURCE=.\util.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\voice_gamemgr.cpp
# End Source File
# Begin Source File

SOURCE=.\world.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\activity.h
# End Source File
# Begin Source File

SOURCE=.\activitymap.h
# End Source File
# Begin Source File

SOURCE=.\animation.h
# End Source File
# Begin Source File

SOURCE=.\basemonster.h
# End Source File
# Begin Source File

SOURCE=.\cbase.h
# End Source File
# Begin Source File

SOURCE=.\cdll_dll.h
# End Source File
# Begin Source File

SOURCE=.\client.h
# End Source File
# Begin Source File

SOURCE=.\commandersmenu.h
# End Source File
# Begin Source File

SOURCE=.\decals.h
# End Source File
# Begin Source File

SOURCE=.\defaultai.h
# End Source File
# Begin Source File

SOURCE=.\doors.h
# End Source File
# Begin Source File

SOURCE=.\effects.h
# End Source File
# Begin Source File

SOURCE=..\engine\eiface.h
# End Source File
# Begin Source File

SOURCE=.\enginecallback.h
# End Source File
# Begin Source File

SOURCE=.\explode.h
# End Source File
# Begin Source File

SOURCE=.\extdll.h
# End Source File
# Begin Source File

SOURCE=.\flyingmonster.h
# End Source File
# Begin Source File

SOURCE=.\func_break.h
# End Source File
# Begin Source File

SOURCE=.\func_grass.h
# End Source File
# Begin Source File

SOURCE=.\game.h
# End Source File
# Begin Source File

SOURCE=.\gamerules.h
# End Source File
# Begin Source File

SOURCE=.\items.h
# End Source File
# Begin Source File

SOURCE=.\maprules.h
# End Source File
# Begin Source File

SOURCE=.\monsterevent.h
# End Source File
# Begin Source File

SOURCE=.\monsters.h
# End Source File
# Begin Source File

SOURCE=.\nodes.h
# End Source File
# Begin Source File

SOURCE=.\particle_emitter.h
# End Source File
# Begin Source File

SOURCE=.\plane.h
# End Source File
# Begin Source File

SOURCE=.\player.h
# End Source File
# Begin Source File

SOURCE=.\saverestore.h
# End Source File
# Begin Source File

SOURCE=.\schedule.h
# End Source File
# Begin Source File

SOURCE=.\scripted.h
# End Source File
# Begin Source File

SOURCE=.\scriptevent.h
# End Source File
# Begin Source File

SOURCE=.\skill.h
# End Source File
# Begin Source File

SOURCE=.\soundent.h
# End Source File
# Begin Source File

SOURCE=.\spectator.h
# End Source File
# Begin Source File

SOURCE=.\squadmonster.h
# End Source File
# Begin Source File

SOURCE=.\talkmonster.h
# End Source File
# Begin Source File

SOURCE=.\trains.h
# End Source File
# Begin Source File

SOURCE=.\userdata.h
# End Source File
# Begin Source File

SOURCE=.\util.h
# End Source File
# Begin Source File

SOURCE=.\vector.h
# End Source File
# Begin Source File

SOURCE=..\common\weaponinfo.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
