    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x14
    stw r0, 0x24(r1)
    li r6, 0x0
    stmw r27, 0xc(r1)
    mr r27, r3
    li r3, 0x0
    bl fn_801F54A4
    rlwinm r28, r3, 0, 16, 31
    mr r3, r27
    bl fn_8020D908
    mr r0, r3
    li r3, 0x0
    mr r31, r0
    li r4, 0x0
    mr r7, r31
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r31
    li r4, 0x0
    li r5, 0xe5
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    mr r30, r3
    li r5, 0x1e
    li r6, 0x0
    bl fn_80142CF4
    rlwinm r29, r3, 0, 16, 31
    li r3, 0x0
    mr r4, r29
    li r5, 0x2
    li r6, 0x0
    bl fn_80142CF4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020B9D4
    mr r3, r30
    li r4, 0x0
    li r5, 0x1f
    li r6, 0x0
    bl fn_80142CF4
    rlwinm r3, r3, 0, 16, 31
    mr r4, r28
    bl fn_801F00D0
    b @L_8020B9D8
@L_8020B9D4:
    mr r3, r31
@L_8020B9D8:
    mr r7, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x42
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r27
    mr r4, r29
    bl fn_80211E18
    lmw r27, 0xc(r1)
    li r3, 0x1
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
