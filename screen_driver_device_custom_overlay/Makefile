DTS = BB-SPIDEV1-00A0.dtso
DTB = BB-SPIDEV1-00A0.dtbo

all:
	cpp -nostdinc -I /opt/source/dtb-6.17.x/include -I /usr/src/linux-headers-6.17.5-bone14/include -undef -x assembler-with-cpp $(DTS) > /tmp/tmp.dts
	dtc -@ -I dts -O dtb -o st7735r-lcd.dtbo /tmp/tmp.dts
	sudo cp st7735r-lcd.dtbo /boot/dtbs/6.17.5-bone14/overlays/

clean:
	rm -f $(DTB) /tmp/tmp.dts
