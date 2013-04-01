# $Id: GNUmakefile,v 1.1.1.1 2013/02/23 13:30:13 rui Exp $
# --------------------------------------------------------------
# GNUmakefile for silicon scripts
# --------------------------------------------------------------

name := matrix
G4TARGET := $(name)
G4EXLIB := true

#ifndef GDML_BASE
#  GDML_BASE := /home/fan/geant4/gdml/
#endif

#ifndef XERCESCROOT
#  XERCESCROOT := /home/fan/geant4/xerces-c_2_6_0-redhat_80-gcc_32/
#endif

CPPFLAGS    += -I`root-config --incdir`
EXTRALIBS   += `root-config --glibs` 
ifndef G4INSTALL
  G4INSTALL = ../../..
endif
.PHONY: all
all: lib bin
include $(G4INSTALL)/config/binmake.gmk

#include $(G4INSTALL)/config/binmake.gmk
#ifdef G4ANALYSIS_USE
#   CPPFLAGS += `aida-config --include`
#   LDLIBS += `aida-config --lib`
#endif
visclean:
	rm -f g4*.prim g4*.eps g4*.wrl *.heprep
	rm -f *.eps *.gif *.mpg  .DAWN_* *.data *.aida *.bin Data* MPV.txt
