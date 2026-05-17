Firmware for ZS-EUB2 TS0012

Add binding for OnOff and LevelCtl

cmake -B.build -DBOARD=TS0012
cmake --build .build


python3 ./TlsrPgm.py -w -t 1 -a 20 -s -p/dev/ttyUSB0 -m we 0x0 ../telink_zigbee/.build/bootloader/bootloader_TS0012.bin
python3 ./TlsrPgm.py -w -t 1 -a 20 -s -p/dev/ttyUSB0 -m we 0x8000 switch_chunk00
python3 ./TlsrPgm.py -w -t 1 -a 20 -s -p/dev/ttyUSB0 -m we 0x18000 switch_chunk01
python3 ./TlsrPgm.py -w -t 1 -a 20 -s -p/dev/ttyUSB0 -m we 0x28000 switch_chunk02
python3 ./TlsrPgm.py -s -p/dev/ttyUSB0 i


https://developer.tuya.com/en/docs/iot/ztu-module-datasheet?id=Ka45nl4ywgabp

python3 ../TlsrComSwireWriter/TLSR825xComFlasher.py -p /dev/ttyUSB0 -t10 -c 24 wf 0x8000 ./.build/switch/switch_TS0013.bin