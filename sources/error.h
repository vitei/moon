#ifndef ERROR_H
#define ERROR_H

extern void emitError(const char *error);
extern void emitError(unsigned int lineNum, const char *error);

#endif
