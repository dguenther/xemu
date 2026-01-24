/*
 * QEMU Stubs for Nintendo Switch - ACPI Functions
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "qemu/osdep.h"

/* Forward declarations */
typedef struct Aml Aml;

/* AML (ACPI Machine Language) functions */
Aml *aml_resource_template(void)
{
    return NULL;
}

Aml *aml_io(uint16_t io_decode, uint16_t io_minimum, uint16_t io_maximum, uint8_t io_alignment, uint8_t io_length)
{
    (void)io_decode; (void)io_minimum; (void)io_maximum; (void)io_alignment; (void)io_length;
    return NULL;
}

void aml_append(Aml *parent, Aml *child)
{
    (void)parent; (void)child;
}

Aml *aml_irq_no_flags(uint32_t irqs)
{
    (void)irqs;
    return NULL;
}

Aml *aml_device(const char *name)
{
    (void)name;
    return NULL;
}

Aml *aml_eisaid(const char *str)
{
    (void)str;
    return NULL;
}

Aml *aml_name_decl(const char *name, Aml *val)
{
    (void)name; (void)val;
    return NULL;
}