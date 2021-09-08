#ifndef LABEL_H
#define LABEL_H

#include <inttypes.h>

struct _Label {
        char* Name;
        int16_t Offset;
};
typedef struct _Label Label;

void InsertLabel(const char* const Name, const int16_t const Offset);

int16_t GetLabel(const char* const Name);

void ClearTable();

int GetFill();

Label* GetElement(int idx);

#endif
