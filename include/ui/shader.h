#ifndef QEMU_SHADER_H
#define QEMU_SHADER_H

#ifdef CONFIG_SWITCH
#include <glad/glad.h>
#else
#include <epoxy/gl.h>
#endif

typedef struct QemuGLShader QemuGLShader;

void qemu_gl_run_texture_blit(QemuGLShader *gls, bool flip);

QemuGLShader *qemu_gl_init_shader(void);
void qemu_gl_fini_shader(QemuGLShader *gls);

#endif /* QEMU_SHADER_H */
