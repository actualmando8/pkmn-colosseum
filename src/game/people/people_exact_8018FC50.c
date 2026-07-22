#include "game/people/people.h"

extern struct GSvec* GSmodelGetRotationPtr(void* model);

struct GSvec* peopleGetPosition(PeopleEntry* entry)
{
    return GSmodelGetRotationPtr(entry->modelHandle);
}
