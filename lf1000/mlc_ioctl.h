#ifndef MLC_IOCTL_H
#define MLC_IOCTL_H

#define MLC_IOC_MAGIC   'm'

struct position_cmd {
	unsigned int top;
   	unsigned int left;
	unsigned int right;
	unsigned int bottom;
};

struct screensize_cmd {
	unsigned int width;
	unsigned int height;
};

struct overlaysize_cmd {
	unsigned int srcwidth;
	unsigned int srcheight;
	unsigned int dstwidth;
	unsigned int dstheight;
};

union mlc_cmd {
	struct position_cmd position;
	struct screensize_cmd screensize;
	struct overlaysize_cmd overlaysize;
};

/* supported ioctls */

#define MLC_IOCTENABLE		_IO(MLC_IOC_MAGIC,  0)
#define MLC_IOCTBACKGND		_IO(MLC_IOC_MAGIC,  1)
#define MLC_IOCQBACKGND		_IO(MLC_IOC_MAGIC,  2)
#define MLC_IOCTPRIORITY	_IO(MLC_IOC_MAGIC,  3)
#define MLC_IOCQPRIORITY	_IO(MLC_IOC_MAGIC,  4)
#define MLC_IOCTTOPDIRTY	_IO(MLC_IOC_MAGIC,  5)
#define MLC_IOCSSCREENSIZE	_IOW(MLC_IOC_MAGIC, 6, struct screensize_cmd *)
#define MLC_IOCGSCREENSIZE	_IOR(MLC_IOC_MAGIC, 7, struct screensize_cmd *)

#define MLC_IOCTLAYEREN		_IO(MLC_IOC_MAGIC,  8)
#define MLC_IOCTADDRESS		_IO(MLC_IOC_MAGIC,  9)
#define MLC_IOCQADDRESS		_IO(MLC_IOC_MAGIC,  25)
#define MLC_IOCTHSTRIDE		_IO(MLC_IOC_MAGIC,  10)
#define MLC_IOCQHSTRIDE		_IO(MLC_IOC_MAGIC,  26)
#define MLC_IOCTVSTRIDE		_IO(MLC_IOC_MAGIC,  11)
#define MLC_IOCQVSTRIDE		_IO(MLC_IOC_MAGIC,  27)
#define MLC_IOCTLOCKSIZE	_IO(MLC_IOC_MAGIC,  12)
#define MLC_IOCSPOSITION	_IOW(MLC_IOC_MAGIC, 13, struct position_cmd *)
#define MLC_IOCGPOSITION	_IOR(MLC_IOC_MAGIC, 14, struct position_cmd *)
#define MLC_IOCTFORMAT		_IO(MLC_IOC_MAGIC,  15)
#define MLC_IOCQFORMAT		_IO(MLC_IOC_MAGIC,  16)
#define MLC_IOCTDIRTY		_IO(MLC_IOC_MAGIC,  17)
#define MLC_IOCQDIRTY		_IO(MLC_IOC_MAGIC,  32)
#define MLC_IOCT3DENB		_IO(MLC_IOC_MAGIC,  18)
#define MLC_IOCQ3DENB		_IO(MLC_IOC_MAGIC,  33)
#define MLC_IOCTALPHA		_IO(MLC_IOC_MAGIC,  19)
#define MLC_IOCQALPHA		_IO(MLC_IOC_MAGIC,  28)
#define MLC_IOCTTPCOLOR		_IO(MLC_IOC_MAGIC,  20)
#define MLC_IOCTBLEND		_IO(MLC_IOC_MAGIC,  21)
#define MLC_IOCTTRANSP		_IO(MLC_IOC_MAGIC,  22)
#define MLC_IOCTINVERT		_IO(MLC_IOC_MAGIC,  23)
#define MLC_IOCTINVCOLOR	_IO(MLC_IOC_MAGIC,  24)
#define MLC_IOCQFBSIZE		_IO(MLC_IOC_MAGIC,  29)
#define MLC_IOCSOVERLAYSIZE	_IOW(MLC_IOC_MAGIC, 30, struct overlaysize_cmd *)
#define MLC_IOCGOVERLAYSIZE	_IOR(MLC_IOC_MAGIC, 31, struct overlaysize_cmd *)
/* LF1000-only */
#define MLC_IOCTINVISIBLE	_IO(MLC_IOC_MAGIC,  34)
#define MLC_IOCQINVISIBLE	_IO(MLC_IOC_MAGIC,  35)
#define MLC_IOCSINVISIBLEAREA	_IOW(MLC_IOC_MAGIC, 36, struct position_cmd *)
#define MLC_IOCGINVISIBLEAREA	_IOR(MLC_IOC_MAGIC, 37, struct position_cmd *)

#define MLC_IOCTADDRESSCB	_IO(MLC_IOC_MAGIC,  38)
#define MLC_IOCTADDRESSCR	_IO(MLC_IOC_MAGIC,  39)

#endif
