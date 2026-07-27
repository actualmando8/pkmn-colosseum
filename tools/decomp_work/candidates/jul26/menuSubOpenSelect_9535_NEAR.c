s32 menuSubOpenSelect(u8 type, u32 p2, u32 p3, s16 x, s16 y, u32 init)
{
    u32 value;
    u32 ret;
    extern void* windowGetActiveID();
    extern s32 menuOpenCustom(s32, ...);
    extern void menuSetPosition();
    extern void windowCheckCursor(s32, s32);
    extern u32 windowGetValue(s32);
    extern void menuCloseCustom(s32, s32, s32);

    value = init;
    menuOpenCustom(0xe7, windowGetActiveID(), &value, 0, 0, 3, type, p2, p3);
    menuSetPosition(0xe7, (s32)x, (s32)y);
    windowCheckCursor(0xe7, 1);
    ret = windowGetValue(0xe7);
    menuCloseCustom(0xe7, 0, 1);
    return ret;
}