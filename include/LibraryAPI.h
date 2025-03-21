
#ifndef PLUGIN_API_H
#define PLUGIN_API_H

#ifdef __cplusplus
extern "C" {
#endif

struct LibraryInfo {
  const char *name;
};

typedef LibraryInfo (*GetLibraryInfoFunc)();
typedef double (*EquationFunc)(double);

#ifdef __cplusplus
}
#endif

#endif // PLUGIN_API_H