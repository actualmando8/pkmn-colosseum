    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_800E9288
    mr r3, r31
    bl fn_800E85E8
    mr r3, r31
    bl fn_800E6DCC
    lbz r0, 0x80(r31)
    cmplwi r0, 0x0
    beq @L_800E4C38
    li r0, 0x1
    mr r3, r31
    stb r0, 0x80(r31)
    bl fn_800E6B20
    @L_800E4C38:
    lwz r3, 0x8(r31)
    bl fn_801A05EC
    lwz r3, 0xc(r31)
    cmplwi r3, 0x0
    beq @L_800E4C60
    bl fn_801A05EC
    lwz r3, 0x10(r31)
    bl fn_801A05EC
    lwz r3, 0x14(r31)
    bl fn_801A05EC
    @L_800E4C60:
    lwz r3, 0x144(r31)
    cmplwi r3, 0x0
    beq @L_800E4C74
    li r4, 0x1
    bl fn_80118874
    @L_800E4C74:
    mr r3, r31
    li r4, 0x0
    li r5, 0x170
    bl memset
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
