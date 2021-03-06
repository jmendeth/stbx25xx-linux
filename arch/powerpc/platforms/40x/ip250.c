/*
 * arch/powerpc/platforms/40x/ip250.c
 *
 * Architecture- / platform-specific boot-time initialization code for
 * IBM PowerPC 4xx based AB IPBox 250. Adapted from original
 * code by Gary Thomas, Cort Dougan <cort@fsmlabs.com>, Dan Malek <dan@net4x.com>.
 * Josh Boyer <jwboyer@linux.vnet.ibm.com> and Grant Erickson <grant@lcse.umn.edu>
 *
 * Author: (C) Robert Burger <robert_burger@web.de>
 *
 * This file is licensed under the terms of the GNU General Public License version 2.  
 * This file is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

#include <linux/init.h>
#include <linux/of_platform.h>
#include <linux/rtc.h>

#include <asm/machdep.h>
#include <asm/prom.h>
#include <asm/udbg.h>
#include <asm/time.h>
#include <asm/uic.h>
#include <asm/ppc4xx.h>
#include <asm/ip250.h>
#include <linux/dm9000.h>

static __initdata struct of_device_id ip250_of_bus[] = 
{
  { .compatible = "ibm,plb3", },
  { .compatible = "ibm,opb", },
  { .compatible = "ibm,ebc", },
  {},
};

static struct resource dm9000_resources[] = 
{
  [0] = 
  {
    .start  = DM9000_MEM_ADDR,
    .end    = DM9000_MEM_ADDR + 3,
    .flags  = IORESOURCE_MEM,
  },
  [1] = 
  {
    .start  = DM9000_MEM_DATA,
    .end    = DM9000_MEM_DATA + 3,
    .flags  = IORESOURCE_MEM,
  },
  [2] = 
  {
    .start  = DM9000_IRQ,
    .end    = DM9000_IRQ,
    .flags  = IORESOURCE_IRQ | IRQF_TRIGGER_LOW,
  },
};

/* TODO: Use DMA in outblk and inblk */
/* Manually byte-swap data on 16-bit reads */
static void ip250_dm9000_outblk(void __iomem *reg, void *buf, int count)
{
  volatile u16 __iomem *port = reg;
  const u16 *tbuf = buf;

  count = (count+1) >> 1;

  if (unlikely(count <= 0))
    return;

  asm volatile("sync");

  do 
  {
    *port = __cpu_to_le16(*tbuf++);
  } 
  while (--count != 0);

  asm volatile("sync");
}

/* Manually byte-swap data on 16-bit writes */
static void ip250_dm9000_inblk(void __iomem *reg, void *buf, int count)
{
  const volatile u16 __iomem *port = reg;
  u16 *tbuf = buf;
  u16 tmp;

  count = (count+1) >> 1;

  if (unlikely(count <= 0))
    return;
  asm volatile("sync");

  do 
  {
    tmp = *port;
    eieio();
    *tbuf++ = __le16_to_cpu(tmp);
  }
  while (--count != 0);

  asm volatile("twi 0,%0,0; isync" : : "r" (tmp));
}

static struct dm9000_plat_data ip250_dm9000_pdata = 
{
  .flags    = DM9000_PLATF_16BITONLY,
  .inblk    = ip250_dm9000_inblk,
  .outblk   = ip250_dm9000_outblk,
};

static struct platform_device dm9000_device = 
{
  .name   = "dm9000",
  .num_resources  = ARRAY_SIZE(dm9000_resources),
  .resource = dm9000_resources,
  .dev    = 
  { 
    .platform_data  = &ip250_dm9000_pdata,
  },
};

static struct platform_device *ip250_devs[] __initdata = 
{
  &dm9000_device,
};

static int __init ip250_device_probe(void)
{
  struct device_node *np;
  int irq;

  of_platform_bus_probe(NULL, ip250_of_bus, NULL);

  np = of_find_compatible_node(NULL, NULL, "davicom,dm9000");
  if (np == NULL) 
  {
    printk(KERN_ERR __FILE__ ": Unable to find DM9000\n");
    goto exit;
  }

  irq = irq_of_parse_and_map(np, 0);
  of_node_put(np);
  if (irq  == NO_IRQ) 
  {
    printk(KERN_ERR __FILE__ ": Unable to get DM9000 irq\n");
    goto exit;
  }

  dm9000_resources[2].start = irq;
  dm9000_resources[2].end   = irq;

  platform_add_devices(ip250_devs, ARRAY_SIZE(ip250_devs));

exit:
  return 0;
}

machine_device_initcall(ip250, ip250_device_probe);

static int __init ip250_probe(void)
{
  unsigned long root = of_get_flat_dt_root();

  if (!of_flat_dt_is_compatible(root, "ab,ip250"))
    return 0;

  return 1;
}

define_machine(ip250) {
  .name           = "AB IPBox 250",
  .probe          = ip250_probe,
  .progress       = udbg_progress,
  .init_IRQ       = uic_init_tree,
  .get_irq        = uic_get_irq,
  .restart        = ppc4xx_reset_system,
  .calibrate_decr = generic_calibrate_decr,
};
