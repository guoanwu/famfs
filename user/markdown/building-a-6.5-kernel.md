# Building and installing a famfs-compatible Linux 6.5 kernel

Clone the kernel. The kernel below has patches that enable testing with /dev/dax devices,
but are not used with /dev/pmem devices. (Running with /dev/pmem devices is currently
recommended.)

    git clone -b famfs-6.5 https://github.com/jagalactic/linux.git famfs-linux

Build the kernel

    cd famfs-linux
    cp famfs.config .config   # Put appropriate config file in place
    make oldconfig            # Accept defaults, if any
    make [-j]

At this point it is likely that you will need to install prerequisites and re-run
the kernel build.

Once you have successfully built the kernel, install it.

    sudo make modules_install headers_install install

Reboot the system and make sure you're running the new kernel

    uname -r

Output:

    6.5.12+