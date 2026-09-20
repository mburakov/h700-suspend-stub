/* SPDX-License-Identifier: GPL-2.0+ */
/* Freestanding environment for the U-Boot DRAM driver compiled into the stub. */

#ifndef STUB_COMPAT_H
#define STUB_COMPAT_H

#ifndef __ASSEMBLY__
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef unsigned long ulong;

#define BIT(nr)			(1UL << (nr))
#define ARRAY_SIZE(a)		(sizeof(a) / sizeof((a)[0]))
#define DIV_ROUND_UP(n, d)	(((n) + (d) - 1) / (d))

#define max(x, y) ({ __typeof__(x) _mx = (x); __typeof__(y) _my = (y); _mx > _my ? _mx : _my; })

#define check_member(s, m, o) \
	_Static_assert(offsetof(struct s, m) == (o), "check_member " #m)

#define debug(...)		do { } while (0)

#define CFG_SYS_SDRAM_BASE	0x40000000UL

void *memcpy(void *dst, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
void udelay(unsigned long us);
void mctl_await_completion(u32 *reg, u32 mask, u32 val);
__attribute__((noreturn)) void stub_panic(void);
void stage(u32 code);
u64 stub_deadline(unsigned long timeout_us);
bool stub_expired(u64 deadline, unsigned long reg);
#define panic(...)		stub_panic()

/*
 * U-Boot 2026.04 and later (c47b636737) probe DRAM from mctl_calc_size() to
 * detect non-power-of-2 sized chips. The stub calls it while Linux's memory
 * is live, so it must never write there: every probe passes and the size
 * stays the power of two, which the stub only uses to place its save area
 * and for reporting.
 */
static inline bool mctl_check_memory(unsigned long addr)
{
	(void)addr;
	return true;
}
#define printf(...)		do { } while (0)

/* Set by the stub: the DRAM driver runs on the resume path. */
extern bool sunxi_dram_resume;
#endif /* __ASSEMBLY__ */

#endif /* STUB_COMPAT_H */
