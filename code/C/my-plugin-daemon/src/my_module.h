#ifndef MY_MODULE_H
#define MY_MODULE_H

#include <glib-object.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MY_TYPE_MODULE               (my_module_get_type ())
#define MY_MODULE(obj)               (G_TYPE_CHECK_INSTANCE_CAST ((obj), MY_TYPE_MODULE, MyModule))
#define MY_MODULE_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST ((klass), MY_TYPE_MODULE, MyModuleClass))
#define MY_IS_MODULE(obj)            (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MY_TYPE_MODULE))
#define MY_IS_MODULE_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE ((obj), MY_TYPE_MODULE))
#define MY_MODULE_GET_CLASS(obj)     (G_TYPE_INSTANCE_GET_CLASS((obj), MY_TYPE_MODULE, MyModuleClass))

typedef struct _MyModule MyModule;

GType my_module_get_type(void) G_GNUC_CONST;

MyModule *my_module_new(const gchar *path);

const char *my_module_get_path(MyModule *module);

GObject *my_module_new_object(MyModule *module);

#ifdef __cplusplus
}
#endif

#endif // MY_MODULE_H
