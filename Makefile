# SEE https://github.com/arduino/Arduino/blob/master/build/shared/manpage.adoc

DIR_SRC := ./airquality

.PHONY: build
build:
	arduino --verify --board esp8266:esp8266:generic --verbose ./airquality/airquality.ino

.PHONY: install
install:
	arduino --upload --board esp8266:esp8266:generic --port /dev/ttyUSB0 --verbose ./airquality/airquality.ino

.PHONY: monitor
monitor:
	minicom -b 9600 -D /dev/ttyUSB0

.PHONY: format
format:
	find -type f \( -name '*.cpp' -o -name '*.hpp' -o -name '*.ino' \) -print0 | xargs -0 --max-args=1 --max-procs=8 clang-format -i

.PHONY: tags
tags:
	ctags -R \
		--sort=yes \
		--totals=yes \
		--languages=C++ \
		--extra=+f \
		${DIR_SRC}

.PHONY: clean
clean:
	rm -f ./tags
