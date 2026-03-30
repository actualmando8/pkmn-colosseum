    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lbz r3, 0x18(r3)
    rlwinm r0, r3, 0, 30, 30
    cmpwi r0, 0x2
    beq @L_801DCF70
    ori r0, r3, 0x2
    stb r0, 0x18(r31)
    lwz r3, 0x24(r31)
    bl fn_800E6DC0
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801DCF4C
    lwz r3, 0x24(r31)
    addi r4, r31, 0x5c
    bl fn_800E7290
    b @L_801DCF60
@L_801DCF4C:
    lwz r3, 0x24(r31)
    addi r4, r31, 0x5c
    li r5, 0x0
    li r6, 0x0
    bl fn_800E732C
@L_801DCF60:
    mr r3, r31
    li r4, 0x1
    li r5, 0x0
    bl fn_801DEF0C
@L_801DCF70:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
