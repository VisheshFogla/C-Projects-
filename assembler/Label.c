#include "Label.h"
#include <string.h>
#include <stdlib.h>


static Label* Table[100];

static int NumFilled = 0;
static int MaxFill = 50;

int GetFill(){
	return NumFilled;
}

Label* GetElement(int idx){
	return Table[idx];
}

int16_t GetLabel(const char* const Name) {

        size_t name_len = strlen(Name);

        for (int i = 0; i < NumFilled; i++){
                Label* check = Table[i];
                if (name_len == strlen(check->Name) && strncmp(Name, check->Name, name_len) == 0) {
                        return check->Offset;
                }
        }

        return -1;

}

void InsertLabel(const char* const Name, const int16_t const Offset){
	if (NumFilled < MaxFill){
		Label* next = (Label*)calloc(1, sizeof(Label));
        	next->Name = Name;
        	next->Offset = Offset;
		Table[NumFilled] = next;
		NumFilled = NumFilled + 1;
	}
}

void ClearTable(){
	for (int i = 0; i < NumFilled; i++){
		Label* next = Table[i];
		free(next->Name);
		free(next);
	}
}
