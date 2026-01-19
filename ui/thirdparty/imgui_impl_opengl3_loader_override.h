// OpenGL loader abstraction: use glad on Switch, epoxy elsewhere
#ifdef CONFIG_SWITCH
#include <glad/glad.h>
#else
#include <epoxy/gl.h>
#endif
