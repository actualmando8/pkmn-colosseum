    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    rlwinm r31, r5, 0, 24, 31
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r4
    lwz r30, 0x24(r3)
    b @L_801DB964
@L_801DB924:
    lwz r0, 0x6c(r30)
    cmpwi r0, 0x1
    bne @L_801DB960
    lwz r0, 0x4(r30)
    cmpwi r0, 0x2
    beq @L_801DB940
    b @L_801DB960
@L_801DB940:
    lwz r3, 0xa4(r30)
    bl fn_800E3CBC
    rlwinm r0, r3, 0, 24, 31
    cmplw r31, r0
    bne @L_801DB960
    lwz r3, 0xa4(r30)
    mr r4, r29
    bl fn_800E3760
@L_801DB960:
    lwz r30, 0xa8(r30)
@L_801DB964:
    cmplwi r30, 0x0
    bne @L_801DB924
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
