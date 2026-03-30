    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x2f
    stw r0, 0x14(r1)
    li r6, 0x0
    stw r31, 0xc(r1)
    mr r31, r3
    li r3, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80216094
    mr r3, r31
    bl fn_80204A10
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80216094
    li r3, 0x0
    b @L_802160D8
@L_80216094:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802160D4
    mr r3, r31
    li r4, 0x3d
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802160D4
    li r3, 0x0
    b @L_802160D8
@L_802160D4:
    li r3, 0x1
@L_802160D8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
