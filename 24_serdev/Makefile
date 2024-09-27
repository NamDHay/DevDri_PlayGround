obj-m += serdev_echo.o

all: module dt
	echo Build Device Tree Overlay and Kernel Module
module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
dt: serdev_overlay.dts
	dtc -@ -I dts -O dtb -o serdev_overlay.dtbo serdev_overlay.dts
	sudo dtoverlay serdev_overlay.dtbo
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	sudo rm -rf serdev_overlay.dtbo
