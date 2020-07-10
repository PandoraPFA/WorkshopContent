# A rather simple and inflexible Makefile (may need to edit paths to source according to local configuration)

ifndef PROJECT_DIR
    $(error PROJECT_DIR is not set)
endif
ifndef PANDORA_DIR
    $(error PANDORA_DIR is not set)
endif

CFLAGS = -c -g -fPIC -O2 -Wall -Wextra -Werror -pedantic -Wno-long-long -Wno-sign-compare -Wshadow -fno-strict-aliasing -std=c++17
ifdef BUILD_32BIT_COMPATIBLE
    CFLAGS += -m32
endif


export

all:
	$(MAKE) $(CFLAGS) -C ${PROJECT_DIR}/workshopcontent/
	$(MAKE) $(CFLAGS) -C ${PROJECT_DIR}/examplecontent/

clean:
	$(MAKE) clean -C ${PROJECT_DIR}/workshopcontent/
	$(MAKE) clean -C ${PROJECT_DIR}/examplecontent/

install:
	$(MAKE) install -C ${PROJECT_DIR}/workshopcontent/
	$(MAKE) install -C ${PROJECT_DIR}/examplecontent/
