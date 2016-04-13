PORT=/dev/ttyUSB0
mkespfsimage <files.txt >flash.espfs
esptool -cp $PORT -cd none -cb 115200 -bm qio -bf 80 -bz 4M -ca 0x100000 -cf flash.espfs
