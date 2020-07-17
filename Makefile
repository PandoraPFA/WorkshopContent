# A rather simple and inflexible Makefile (may need to edit paths to source according to local configuration)

ifndef PROJECT_DIR
    $(error PROJECT_DIR is not set)
endif
ifndef PANDORA_DIR
    $(error PANDORA_DIR is not set)
endif

all:
	$(MAKE) -C ${PROJECT_DIR}/workshopcontent/
	$(MAKE) -C ${PROJECT_DIR}/examplecontent/

clean:
	$(MAKE) clean -C ${PROJECT_DIR}/workshopcontent/
	$(MAKE) clean -C ${PROJECT_DIR}/examplecontent/

install:
	$(MAKE) install -C ${PROJECT_DIR}/workshopcontent/
	$(MAKE) install -C ${PROJECT_DIR}/examplecontent/
