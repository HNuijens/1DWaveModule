# Project Name
TARGET = 1DWaveModule

# Sources
CPP_SOURCES = \
src/1DWaveModule.cpp\
src/DynamicStiffString.cpp\
#src/StiffString.cpp\
src/DynamicString.cpp\


# Library Locations
LIBDAISY_DIR = ../../libDaisy/
DAISYSP_DIR = ../../DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
