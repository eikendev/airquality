# SEE https://github.com/arduino/Arduino/blob/master/build/shared/manpage.adoc

.PHONY: build
build:
	arduino --verify --board esp8266:esp8266:generic --verbose ./airquality/airquality.ino

.PHONY: install
install:
	arduino --upload --board esp8266:esp8266:generic --port /dev/ttyUSB0 --verbose ./airquality/airquality.ino

.PHONY: monitor
monitor:
	minicom -b 9600 -D /dev/ttyUSB0
