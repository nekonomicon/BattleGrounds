#
# Half-Life Full SDK 2.3 bg_i386.so Makefile for x86 Linux
#
# June 2003 by Ben Banfield (ben@bgmod.com) based on Leon Hartwigs Makefile
#

DLLNAME=bg

ARCH=i386

#make sure this is the correct compiler for your system
CC=g++
C=gcc

DLL_SRCDIR=.
WEAPONS_SRCDIR=$(DLL_SRCDIR)/weapons
BGRULES_SRCDIR=$(DLL_SRCDIR)/bg_rules
POINTS_SRCDIR=$(DLL_SRCDIR)/points
ENGINE_SRCDIR=../engine
COMMON_SRCDIR=../common
WPN_SHARED_SRCDIR=./wpn_shared
PM_SHARED_SRCDIR=../pm_shared
GAME_SHARED_SRCDIR=../game_shared

DLL_OBJDIR=$(DLL_SRCDIR)/obj
WPN_SHARED_OBJDIR=$(WPN_SHARED_SRCDIR)/obj
PM_SHARED_OBJDIR=$(PM_SHARED_SRCDIR)/obj
GAME_SHARED_OBJDIR=$(GAME_SHARED_SRCDIR)/obj
WEAPONS_OBJDIR=$(WEAPONS_SRCDIR)/obj
BGRULES_OBJDIR=$(BGRULES_SRCDIR)/obj
POINT_OBJDIR=$(POINTS_SRCDIR)/obj

BASE_CFLAGS=	-Dstricmp=strcasecmp -D_strnicmp=strncasecmp -Dstrnicmp=strncasecmp \
		-DCLIENT_WEAPONS

#safe optimization
CFLAGS=$(BASE_CFLAGS) -w -march=i586 -g

#full optimization
#CFLAGS=$(BASE_CFLAGS) -w -O1 -m486 -ffast-math -funroll-loops \
	-fomit-frame-pointer -fexpensive-optimizations \
	-malign-loops=2 -malign-jumps=2 -malign-functions=2

#use these when debugging 
#CFLAGS=$(BASE_CFLAGS) -g

INCLUDEDIRS=-I. -I$(ENGINE_SRCDIR) -I$(COMMON_SRCDIR) -I$(PM_SHARED_SRCDIR) -I$(GAME_SHARED_SRCDIR) -I$(WEAPONS_SRCDIR) -I$(BGRULES_SRCDIR) -I$(POINTS_SRCDIR)

LDFLAGS=-lm

SHLIBEXT=so
SHLIBCFLAGS=-fPIC
SHLIBLDFLAGS=-shared

DO_CC=$(CC) $(CFLAGS) $(SHLIBCFLAGS) $(INCLUDEDIRS) -o $@ -c $<
DO_C=$(C) $(CFLAGS) $(SHLIBCFLAGS) $(INCLUDEDIRS) -o $@ -c $<


#############################################################################
# SETUP AND BUILD
# GAME
#############################################################################

$(DLL_OBJDIR)/%.o: $(DLL_SRCDIR)/%.cpp
	$(DO_CC)

$(WEAPONS_OBJDIR)/%.o: $(WEAPONS_SRCDIR)/%.cpp
	$(DO_CC)

$(BGRULES_OBJDIR)/%.o: $(BGRULES_SRCDIR)/%.cpp
	$(DO_CC)
	
$(POINT_OBJDIR)/%.o: $(POINTS_SRCDIR)/%.cpp
	$(DO_CC)
	
$(WPN_SHARED_OBJDIR)/%.o: $(WPN_SHARED_SRCDIR)/%.cpp
	$(DO_CC)

$(GAME_SHARED_OBJDIR)/%.o: $(GAME_SHARED_SRCDIR)/%.cpp
	$(DO_CC)

$(PM_SHARED_OBJDIR)/%.o: $(PM_SHARED_SRCDIR)/%.c
	$(DO_C)

OBJ = \
	$(POINT_OBJDIR)/base_point.o \
	$(POINT_OBJDIR)/capture_point.o \
	$(POINT_OBJDIR)/point_win.o \
	$(POINT_OBJDIR)/point_manager.o \
	$(POINT_OBJDIR)/point_model.o \
	$(WEAPONS_OBJDIR)/base_weapon.o \
	$(WEAPONS_OBJDIR)/bayonet.o \
	$(WEAPONS_OBJDIR)/brown_bess.o \
	$(WEAPONS_OBJDIR)/charleville.o \
	$(WEAPONS_OBJDIR)/knife.o \
	$(WEAPONS_OBJDIR)/ggrenade.o \
	$(WEAPONS_OBJDIR)/melee.o \
	$(WEAPONS_OBJDIR)/pennsylvania.o \
	$(WEAPONS_OBJDIR)/pistol_a.o \
	$(WEAPONS_OBJDIR)/pistol_b.o \
	$(WEAPONS_OBJDIR)/revolutionnaire.o \
	$(WEAPONS_OBJDIR)/sabre.o \
	$(WEAPONS_OBJDIR)/weapons.o \
	$(BGRULES_OBJDIR)/ammo_ents.o \
	$(BGRULES_OBJDIR)/info_reset.o \
	$(BGRULES_OBJDIR)/bg_class.o \
	$(BGRULES_OBJDIR)/bg_observer.o \
	$(BGRULES_OBJDIR)/bg_respawnwaves.o \
	$(BGRULES_OBJDIR)/bg_round.o \
	$(BGRULES_OBJDIR)/bg_round_container.o \
	$(BGRULES_OBJDIR)/bg_roundrules.o \
	$(BGRULES_OBJDIR)/bg_rules.o \
	$(BGRULES_OBJDIR)/bg_teams.o \
	$(BGRULES_OBJDIR)/origin_tracker.o \
	$(BGRULES_OBJDIR)/progressive_spawning.o \
	$(DLL_OBJDIR)/particle_emitter.o \
	$(DLL_OBJDIR)/func_grass.o \
	$(DLL_OBJDIR)/animating.o \
	$(DLL_OBJDIR)/animation.o \
	$(DLL_OBJDIR)/bmodels.o \
	$(DLL_OBJDIR)/buttons.o \
	$(DLL_OBJDIR)/cbase.o \
	$(DLL_OBJDIR)/client.o \
	$(DLL_OBJDIR)/combat.o \
	$(DLL_OBJDIR)/doors.o \
	$(DLL_OBJDIR)/effects.o \
	$(DLL_OBJDIR)/explode.o \
	$(DLL_OBJDIR)/func_break.o \
	$(DLL_OBJDIR)/func_tank.o \
	$(DLL_OBJDIR)/game.o \
	$(DLL_OBJDIR)/gamerules.o \
	$(DLL_OBJDIR)/globals.o \
	$(DLL_OBJDIR)/h_ai.o \
	$(DLL_OBJDIR)/h_cycler.o \
	$(DLL_OBJDIR)/h_export.o \
	$(DLL_OBJDIR)/items.o \
	$(DLL_OBJDIR)/lights.o \
	$(DLL_OBJDIR)/maprules.o \
	$(DLL_OBJDIR)/mortar.o \
	$(DLL_OBJDIR)/mpstubb.o \
	$(DLL_OBJDIR)/multiplay_gamerules.o \
	$(DLL_OBJDIR)/pathcorner.o \
	$(DLL_OBJDIR)/plane.o \
	$(DLL_OBJDIR)/plats.o \
	$(DLL_OBJDIR)/player.o \
	$(DLL_OBJDIR)/singleplay_gamerules.o \
	$(DLL_OBJDIR)/skill.o \
	$(DLL_OBJDIR)/sound.o \
	$(DLL_OBJDIR)/soundent.o \
	$(DLL_OBJDIR)/spectator.o \
	$(DLL_OBJDIR)/subs.o \
	$(DLL_OBJDIR)/triggers.o \
	$(DLL_OBJDIR)/util.o \
	$(DLL_OBJDIR)/world.o \
	$(GAME_SHARED_OBJDIR)/voice_gamemgr.o \
	$(PM_SHARED_OBJDIR)/pm_debug.o \
	$(PM_SHARED_OBJDIR)/pm_math.o \
	$(PM_SHARED_OBJDIR)/pm_shared.o

$(DLLNAME)_$(ARCH).$(SHLIBEXT) : neat $(OBJ)
	$(CC) $(CFLAGS) $(SHLIBLDFLAGS) $(LDFLAGS) -o $@ $(OBJ) -lm

neat:
	-mkdir $(DLL_OBJDIR)
	-mkdir $(WPN_SHARED_OBJDIR)
	-mkdir $(GAME_SHARED_OBJDIR)
	-mkdir $(PM_SHARED_OBJDIR)
	-mkdir $(WEAPONS_OBJDIR)
	-mkdir $(POINT_OBJDIR)
	-mkdir $(BGRULES_OBJDIR)
clean:
	-rm -f $(OBJ)
	-rm -f $(DLLNAME)_$(ARCH).$(SHLIBEXT)
spotless: clean
	-rm -r $(DLL_OBJDIR)
	-rm -r $(WPN_SHARED_OBJDIR)
	-rm -r $(GAME_SHARED_OBJDIR)
	-rm -r $(PM_SHARED_OBJDIR)
	-rm -r $(WEAPONS_OBJDIR)
	-rm -r $(POINT_OBJDIR)
	-rm -r $(BGRULES_OBJDIR)

