/*
 * wARM functionality test
 *
 * written by Gražvydas "notaz" Ignotas
 *
 * see warm.c for license info
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#if 1
#include "warm.h"
#else
#define warm_init(...) 0
#define warm_finish(...)
#define warm_cache_op_range(...)
#define warm_change_cb_range(...)
#endif

typedef unsigned long long u64;
static u64 start_time, end_time;

static unsigned char buff[8 * 1024 * 1024] __attribute__((aligned(32)));
static unsigned char *buff_mid = &buff[8 * 1024 * 1024 / 2];

static u64 xtime(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (u64)tv.tv_sec * 1000000 + tv.tv_usec;
}

static void test_start(void)
{
	start_time = xtime();
}

static void test_end(void)
{
	end_time = xtime();
}

static void show_result(const char *name, int bytes)
{
	double secs = (end_time - start_time) / 1000000.0;
	printf("%-16s: %4.1fs, %5.1f MB/s\n", name, secs, bytes / secs / 1048576);
}

static void do_memset(void)
{
	memset(buff, 0, sizeof(buff));
}

static void byte_fill(void)
{
	char *p = (void *)buff;
	int i;
	for (i = sizeof(buff); i > 0; i--)
		*p++ = 0;
}

static void word_fill(void)
{
	long *p = (void *)buff;
	int i;
	for (i = sizeof(buff) / sizeof(*p); i > 0; i--)
		*p++ = 0;
}

static void do_memcpy(void)
{
	memcpy(buff, buff_mid, sizeof(buff) / 2);
}

static void byte_cpy(void)
{
	char *d = (void *)buff;
	char *s = (void *)buff_mid;
	int i;
	for (i = sizeof(buff) / sizeof(*d) / 2; i > 0; i--)
		*d++ = *s++;
}

static void word_cpy(void)
{
	long *d = (void *)buff;
	long *s = (void *)buff_mid;
	int i;
	for (i = sizeof(buff) / sizeof(*d) / 2; i > 0; i--)
		*d++ = *s++;
}

static void word_inc(void)
{
	long *p = (void *)buff;
	int i;

	for (i = sizeof(buff) / sizeof(*p); i > 0; i--) {
		(*p)++;
		p++;
	}
}

#define ONE_TEST(count, func) \
	test_start(); \
	for (i = count; i > 0; i--) \
		func(); \
	test_end()

static void tests(void)
{
	int i;

	ONE_TEST(64, do_memset);
	show_result("memset", sizeof(buff) * 64);

	ONE_TEST(64, byte_fill);
	show_result("byte fill", sizeof(buff) * 64);

	ONE_TEST(64, word_fill);
	show_result("word fill", sizeof(buff) * 64);

	ONE_TEST(128, do_memcpy);
	show_result("memcpy", sizeof(buff) * 128 / 2);

	ONE_TEST(128, byte_cpy);
	show_result("byte copy", sizeof(buff) * 128 / 2);

	ONE_TEST(128, word_cpy);
	show_result("word copy", sizeof(buff) * 128 / 2);

	ONE_TEST(64, word_inc);
	show_result("word inc", sizeof(buff) * 64);

	usleep(200000);
}

#if 1
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

void coherency_test(void)
{
	volatile unsigned char *buff_mapped;
	volatile unsigned char *buff_vol;
	unsigned long buff_phys, mapped_phys, align;
	unsigned char buff_mapped_vals[6];
	unsigned char buff_vals[5];
	int random_offs;
	int memdev;

	srand(time(NULL));

	buff_phys = warm_virt2phys(buff_mid);
	align = buff_phys & 0xfff;

	memdev = open("/dev/mem", O_RDONLY | O_SYNC);
	if (memdev < 0) {
		perror("open /dev/mem");
		return;
	}

	/* the mapping is valid for 1 page only.
	 * Also it doesn't work for GP2X F100 (2.4 kernels?),
	 * only upper mem maps correctly there. */
	buff_mapped = mmap(NULL, 0x1000, PROT_READ,
			MAP_SHARED, memdev, buff_phys & ~0xfff);
	if (buff_mapped == MAP_FAILED) {
		perror("mmap buff");
		return;
	}
	buff_mapped += align;

	buff_mapped_vals[5] = buff_mapped[0];	/* touch */
	mapped_phys = warm_virt2phys((void *)buff_mapped);
	if (buff_phys != mapped_phys)
		printf("warning: mmap requested %08lx, got %08lx\n", buff_phys, mapped_phys);

	random_offs = rand() % (0x1000 - align);

	buff_vol = (volatile void *)buff_mid;
	buff_vals[0] = buff_vol[random_offs]; buff_mapped_vals[0] = buff_mapped[random_offs];

	/* incremented: */
	buff_vol[random_offs]++;
	buff_vals[1] = buff_vol[random_offs]; buff_mapped_vals[1] = buff_mapped[random_offs];

	/* cleaned: */
	warm_cache_op_range(WOP_D_CLEAN, (char *)buff_vol + random_offs, 32);
	buff_vals[2] = buff_vol[random_offs]; buff_mapped_vals[2] = buff_mapped[random_offs];

	/* incremented: */
	buff_vol[random_offs]++;
	buff_vals[3] = buff_vol[random_offs]; buff_mapped_vals[3] = buff_mapped[random_offs];

	/* invalidated: */
	warm_cache_op_range(WOP_D_INVALIDATE, (char *)buff_vol + random_offs, 32);
	buff_vals[4] = buff_vol[random_offs]; buff_mapped_vals[4] = buff_mapped[random_offs];

	printf("buff is @ %p -> %lx, mapped %p, random offset %x\n", buff_vol, buff_phys,
		buff_mapped, random_offs);
	printf("val: %02x, mmaped: %02x\n", buff_vals[0], buff_mapped_vals[0]);

	printf("incremented:\n");
	printf("val: %02x, mmaped: %02x\n", buff_vals[1], buff_mapped_vals[1]);

	printf("cleaned:\n");
	printf("val: %02x, mmaped: %02x\n", buff_vals[2], buff_mapped_vals[2]);

	printf("incremented:\n");
	printf("val: %02x, mmaped: %02x\n", buff_vals[3], buff_mapped_vals[3]);

	printf("invalidated:\n");
	printf("val: %02x, mmaped: %02x\n", buff_vals[4], buff_mapped_vals[4]);
}
#else
#define coherency_test()
#endif

int main()
{
	int ret;

	ret = warm_init();
	if (ret != 0)
	{
		printf("init failed.\n");
		return 1;
	}

	printf("buff: %p - %p\n", buff, buff + sizeof(buff) - 1);

	printf("-- default --\n");
	tests();

	printf("-- ncnb --\n");
	warm_change_cb_range(WCB_C_BIT|WCB_B_BIT, 0, buff, sizeof(buff));
	tests();

	printf("-- nc b --\n");
	warm_change_cb_range(WCB_B_BIT, 1, buff, sizeof(buff));
	tests();

	printf("--  cnb --\n");
	warm_change_cb_range(WCB_C_BIT, 1, buff, sizeof(buff));
	warm_change_cb_range(WCB_B_BIT, 0, buff, sizeof(buff));
	tests();

	warm_change_cb_range(WCB_C_BIT|WCB_B_BIT, 1, buff, sizeof(buff));
	coherency_test();

	warm_finish();

	return 0;
}

