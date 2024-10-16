MAKEFLAGS += --no-print-directory
SRCS=dataStruct

.PHONY: setup test $(SRCS)

setup: $(SRCS)
	@echo $(shell pwd)
test: test_exc
	./test_exc
test_exc:
	touch test_exc
$(SRCS):
	@$(MAKE) -C $@
