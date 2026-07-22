#include "game/people/people.h"

BOOL peopleTestFlags(PeopleEntry* entry, u32 mask)
{
    return (entry->flags & mask) != 0;
}
