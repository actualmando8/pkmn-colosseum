    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r5
    stw r29, 0x14(r1)
    mr r29, r4
    beq @L_801DF144
    lbz r0, 0x75(r3)
    cmplwi r0, 0x0
    bne @L_801DF0D8
    lhz r0, 0x32(r3)
    cmpwi r0, 0x6
    bge @L_801DF0C8
    cmpwi r0, 0x1
    beq @L_801DF0D8
    bge @L_801DF0D4
    cmpwi r0, 0x0
    bge @L_801DF0D4
    b @L_801DF0D8
@L_801DF0C8:
    cmpwi r0, 0xa
    beq @L_801DF0D4
    b @L_801DF0D8
@L_801DF0D4:
    li r30, 0x1
@L_801DF0D8:
    lbz r4, 0x18(r3)
    rlwinm r0, r4, 0, 28, 28
    cmpwi r0, 0x8
    beq @L_801DF144
    rlwinm r0, r4, 0, 29, 29
    lwz r31, 0x24(r3)
    cmpwi r0, 0x4
    beq @L_801DF10C
    li r0, 0x0
    li r4, 0x1
    stb r0, 0x19(r3)
    mr r3, r31
    bl fn_800EC188
@L_801DF10C:
    mr r3, r31
    mr r4, r29
    bl fn_800ECCA8
    mr r3, r31
    mr r4, r30
    bl fn_800ECB74
    lfs f1, -21208(r2)
    mr r3, r31
    bl fn_800EC9DC
    lfs f1, -21204(r2)
    mr r3, r31
    bl fn_800ECA78
    mr r3, r31
    bl fn_800EC990
@L_801DF144:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
