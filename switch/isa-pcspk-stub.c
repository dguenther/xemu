/*
 * Minimal ISA PC speaker device registration for Switch bring-up.
 */

#include "qemu/osdep.h"
#include "qemu/module.h"
#include "hw/isa/isa.h"
#include "hw/audio/pcspk.h"
#include "hw/qdev-properties.h"
#include "hw/timer/i8254.h"
#include "qom/object.h"

typedef struct PCSpkStubState {
    ISADevice parent;
    char *audiodev;
    void *pit;  /* Pointer to PIT device (unused in stub) */
} PCSpkStubState;

#define PCSPK_STUB(obj) OBJECT_CHECK(PCSpkStubState, (obj), TYPE_PC_SPEAKER)

static char *pcspk_stub_get_audiodev(Object *obj, Error **errp)
{
    PCSpkStubState *s = PCSPK_STUB(obj);

    (void)errp;
    return s->audiodev ? g_strdup(s->audiodev) : NULL;
}

static void pcspk_stub_set_audiodev(Object *obj, const char *value, Error **errp)
{
    PCSpkStubState *s = PCSPK_STUB(obj);

    (void)errp;
    g_free(s->audiodev);
    s->audiodev = value ? g_strdup(value) : NULL;
}

static void pcspk_stub_finalize(Object *obj)
{
    PCSpkStubState *s = PCSPK_STUB(obj);

    g_free(s->audiodev);
    s->audiodev = NULL;
}

static void pcspk_stub_instance_init(Object *obj)
{
    PCSpkStubState *s = PCSPK_STUB(obj);

    /* Add the "pit" link property that the machine expects */
    object_property_add_link(obj, "pit", TYPE_PIT_COMMON,
                             (Object **)&s->pit,
                             qdev_prop_allow_set_link_before_realize,
                             0);
}

static void pcspk_stub_class_init(ObjectClass *oc, void *data)
{
    (void)data;
    object_class_property_add_str(oc, "audiodev",
                                  pcspk_stub_get_audiodev,
                                  pcspk_stub_set_audiodev);
}

static const TypeInfo pcspk_stub_info = {
    .name = TYPE_PC_SPEAKER,
    .parent = TYPE_ISA_DEVICE,
    .instance_size = sizeof(PCSpkStubState),
    .instance_init = pcspk_stub_instance_init,
    .instance_finalize = pcspk_stub_finalize,
    .class_init = pcspk_stub_class_init,
};

static void pcspk_stub_register_types(void)
{
    type_register_static(&pcspk_stub_info);
}

type_init(pcspk_stub_register_types)
