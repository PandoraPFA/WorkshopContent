# A rather simple and inflexible Makefile (may need to edit paths to source according to local configuration)

ifndef PROJECT_DIR
    $(error PROJECT_DIR is not set)
endif
ifndef PANDORA_DIR
    $(error PANDORA_DIR is not set)
endif

CC = g++
CFLAGS = -c -g -fPIC -O2 -Wall -Wextra -Werror -pedantic -Wno-long-long -Wno-sign-compare -Wshadow -fno-strict-aliasing -std=c++17
ifdef BUILD_32BIT_COMPATIBLE
    CFLAGS += -m32
endif

LIBS = -L$(PANDORA_DIR)/lib -lPandoraSDK -lLArContent
ifdef MONITORING
    LIBS += $(shell root-config --glibs --evelibs)
    LIBS += -lPandoraMonitoring
endif
ifdef BUILD_32BIT_COMPATIBLE
    LIBS += -m32
endif

INCLUDES  = -I$(PROJECT_DIR)
INCLUDES += -I$(PANDORA_DIR)/PandoraSDK/include
INCLUDES += -I$(PANDORA_DIR)/LArContent/
ifdef MONITORING
    INCLUDES += -I $(shell root-config --incdir)
    INCLUDES += -I$(PANDORA_DIR)/PandoraMonitoring/include
endif

ifdef MONITORING
    DEFINES = -DMONITORING=1
endif

SOURCES  = $(wildcard $(PROJECT_DIR)/workshopcontent/*.cc)
SOURCES += $(wildcard $(PROJECT_DIR)/workshopcontent/Algorithms/*.cc)
OBJECTS = $(SOURCES:.cc=.o)
DEPENDS = $(OBJECTS:.o=.d)

TEST_SOURCES = $(wildcard $(PROJECT_DIR)/workshopcontent/Test/*.cc)
TEST_OBJECTS = $(TEST_SOURCES:.cc=.o)
TEST_DEPENDS = $(TEST_OBJECTS:.o=.d)

PROJECT_LIBRARY_DIR=$(PROJECT_DIR)/lib
PROJECT_BINARY_DIR=$(PROJECT_DIR)/bin

PROJECT_LIBRARY = $(PROJECT_LIBRARY_DIR)/libWorkshopContent.so
PROJECT_BINARY = $(PROJECT_BINARY_DIR)/PandoraWorkshop

all: library binary

library: $(SOURCES) $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -shared -o $(PROJECT_LIBRARY)

binary: $(TEST_SOURCES) $(TEST_OBJECTS)
	$(CC) $(TEST_OBJECTS) $(LIBS) -L$(PROJECT_LIBRARY_DIR) -lWorkshopContent -o $(PROJECT_BINARY)

-include $(DEPENDS)
-include $(TEST_DEPENDS)

%.o:%.cc
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -MP -MMD -MT $*.o -MT $*.d -MF $*.d -o $*.o $*.cc

clean:
	rm -f $(OBJECTS)
	rm -f $(DEPENDS)
	rm -f $(PROJECT_LIBRARY)
	rm -f $(TEST_OBJECTS)
	rm -f $(TEST_DEPENDS)
	rm -f $(PROJECT_BINARY)

install:
ifdef INCLUDE_TARGET
	rsync -r --include '*/' --include '*.h' --exclude '*' --prune-empty-dirs $(PROJECT_DIR)/ ${INCLUDE_TARGET}
endif
ifdef LIB_TARGET
	cp $(PROJECT_LIBRARY) ${LIB_TARGET}
endif
