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


# Usage

Pour chaque bouton, appuyez 5 fois rapidement pour entrer en mode appairage. Ensuite, utilisez une application compatible Zigbee (comme Zigbee2MQTT ou une application de domotique) pour rechercher et connecter l'appareil. Une fois connecté, vous pourrez contrôler les fonctions d'OnOff et de LevelCtl à distance via l'application.

# Configuration des évènements par endpoint

| Evenemment | nbclicks | Commande            | Remote Address | Extra | Description                 |
|------------|----------|---------------------|----------------|-------|-----------------------------|
| Click      | 1        | OnOff Toggle/On/Off | 0xABCD         | RelayMode      | Appui court sur le bouton 1 |
| Hold       | 0        | LevelCtl Up         | 0xABCD         | step  | Appui long sur le bouton 1  |
| Hold       | 0        | LevelCtl Down       | 0xABCD         | step  | Appui long sur le bouton 1  |
