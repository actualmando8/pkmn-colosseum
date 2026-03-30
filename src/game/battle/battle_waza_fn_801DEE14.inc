    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr. r30, r3
    beq @L_801DEEF4
    lwz r31, 0x24(r30)
    li r0, 0x0
    li r4, 0x1
    stb r0, 0x19(r30)
    mr r3, r31
    bl fn_800EC188
    lwz r3, 0x2c(r30)
    lwz r0, 0x84c(r3)
    addi r3, r3, 0x8d4
    mtctr r0
    cmpwi r0, 0x0
    ble @L_801DEE7C
@L_801DEE60:
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    bne @L_801DEE74
    lwz r4, 0x4(r3)
    b @L_801DEE80
@L_801DEE74:
    addi r3, r3, 0x8
    bdnz @L_801DEE60
@L_801DEE7C:
    li r4, 0x0
@L_801DEE80:
    mr r3, r31
    bl fn_800ECCA8
    lbz r0, 0x75(r30)
    cmplwi r0, 0x0
    beq @L_801DEEA4
    mr r3, r31
    li r4, 0x0
    bl fn_800ECB74
    b @L_801DEEB0
@L_801DEEA4:
    mr r3, r31
    li r4, 0x1
    bl fn_800ECB74
@L_801DEEB0:
    lfs f1, -21208(r2)
    mr r3, r31
    bl fn_800EC9DC
    lfs f1, -21204(r2)
    mr r3, r31
    bl fn_800ECA78
    mr r3, r31
    bl fn_800EC990
    lbz r0, 0x75(r30)
    cmplwi r0, 0x0
    beq @L_801DEEF4
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    bl fn_800EC154
    li r0, 0x1
    stb r0, 0x16(r30)
@L_801DEEF4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
