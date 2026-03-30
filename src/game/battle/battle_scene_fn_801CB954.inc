    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    bl fn_80113F48
    mr r4, r29
    mr r31, r3
    bl fn_8018D998
    cmplwi r3, 0x0
    beq @L_801CB9A0
    mr r3, r31
    mr r4, r29
    rlwinm r5, r30, 0, 24, 31
    bl fn_8018C1E8
    b @L_801CB9BC
@L_801CB9A0:
    mr r3, r31
    mr r4, r29
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801CB9BC
    rlwinm r4, r30, 0, 24, 31
    bl fn_800E4014
@L_801CB9BC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
