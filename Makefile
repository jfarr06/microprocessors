TGT ?= release

.PHONY: upload build

images:
	@python3 $(CURDIR)/assets/bmp2bin.py $(CURDIR)/assets $(CURDIR)/include/images_data.h

clean:
	platformio -f -c vim run -v --target clean 

build:
	platformio -f -c vim run -v -e $(TGT) -a "-DHI"

upload:
	platformio -f -c vim run -v --target upload -e $(TGT)

init:
	platformio -f -c vim run -v --target compiledb -e $(TGT)
