DTS = BB-EHRPWM0-P9_21-P9_22.dtso
DTB = BB-EHRPWM0-P9_21-P9_22.dtbo

all:
	cpp -nostdinc -I /opt/source/dtb-6.17.x/include -I /usr/src/linux-headers-6.17.5-bone14/include -undef -x assembler-with-cpp $(DTS) > /tmp/tmp.dts
	dtc -@ -I dts -O dtb -o BB-EHRPWM0-P9_21-P9_22.dtbo /tmp/tmp.dts
	sudo cp BB-EHRPWM0-P9_21-P9_22.dtbo /boot/dtbs/6.17.5-bone14/overlays/

clean:
	rm -f $(DTB) /tmp/tmp.dts
