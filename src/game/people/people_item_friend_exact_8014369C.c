#include "game/people/people_data.h"

s8 itemParamGetFriend3Up(u8* data)
{
    ItemParamData* item;

    item = (ItemParamData*)data;
    if (item == NULL) {
        return 0;
    }
    return (s8)((u8)item->friend3Up);
}

s8 itemParamGetFriend2Up(u8* data)
{
    ItemParamData* item;

    item = (ItemParamData*)data;
    if (item == NULL) {
        return 0;
    }
    return (s8)((u8)item->friend2Up);
}

s8 itemParamGetFriend1Up(u8* data)
{
    ItemParamData* item;

    item = (ItemParamData*)data;
    if (item == NULL) {
        return 0;
    }
    return (s8)((u8)item->friend1Up);
}
