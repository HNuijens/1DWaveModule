# Project Name
TARGET = 1DWaveModule

# Sources
CPP_SOURCES = \
src/1DWaveModule.cpp\
src/StiffString.cpp

# Library Locations
LIBDAISY_DIR = ../../libDaisy/
DAISYSP_DIR = ../../DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
