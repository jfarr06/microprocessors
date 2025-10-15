TGT = project

.PHONY: upload build

clean:
	platformio -f -c vim run -v --target clean 

build:
	platformio -f -c vim run -v -e $(TGT)

upload:
	platformio -f -c vim run -v --target upload -e $(TGT)

init:
	platformio -f -c vim run -v --target compiledb -e $(TGT)
