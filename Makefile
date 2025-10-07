LAB :=

.PHONY: upload build

clean:
	platformio -f -c vim run -v --target clean 

build:
ifeq ($(strip $(LAB)),)
	platformio -f -c vim run -v 
else
	platformio -f -c vim run -v -e lab$(LAB)
endif

upload:
ifeq ($(strip $(LAB)),)
	$(info $$LAB is required!)
else
	platformio -f -c vim run -v --target upload -e lab$(LAB)
endif

init:
	platformio -f -c vim run -v --target compiledb
