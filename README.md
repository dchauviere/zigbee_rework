Firmware for ZS-EUB2 TS0012

Add binding for OnOff and LevelCtl

cmake -B.build -DBOARD=TS0012
cmake --build .build


python3 ./TlsrPgm.py -w -t 1 -a 20 -s -p/dev/ttyUSB0 -m we 0x0 ../telink_zigbee/.build/bootloader/bootloader_TS0012.bin
python3 ./TlsrPgm.py -w -t 1 -a 20 -s -p/dev/ttyUSB0 -m we 0x8000 switch_chunk00
python3 ./TlsrPgm.py -w -t 1 -a 20 -s -p/dev/ttyUSB0 -m we 0x18000 switch_chunk01
python3 ./TlsrPgm.py -w -t 1 -a 20 -s -p/dev/ttyUSB0 -m we 0x28000 switch_chunk02
python3 ./TlsrPgm.py -s -p/dev/ttyUSB0 i