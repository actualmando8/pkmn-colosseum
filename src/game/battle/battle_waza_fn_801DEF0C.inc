    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r5
    stw r29, 0x14(r1)
    mr r29, r4
    stw r28, 0x10(r1)
    mr. r28, r3
    beq @L_801DF050
    lbz r0, 0x16(r28)
    cmplwi r0, 0x0
    bne @L_801DF050
    lbz r3, 0x18(r28)
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x8
    beq @L_801DF050
    rlwinm r0, r3, 0, 29, 29
    lwz r31, 0x24(r28)
    cmpwi r0, 0x4
    beq @L_801DEF78
    li r0, 0x0
    mr r3, r31
    stb r0, 0x19(r28)
    li r4, 0x1
    bl fn_800EC188
@L_801DEF78:
    lbz r0, 0x18(r28)
    lwz r4, 0x2c(r28)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x2
    mr r3, r4
    bne @L_801DEFB0
    lhz r0, 0x14(r28)
    cmplwi r0, 0x10
    ble @L_801DEFB0
    addi r3, r4, 0xd40
    lwz r0, 0xdd4(r4)
    cmpwi r0, 0x1
    bne @L_801DEFB0
    mr r3, r4
@L_801DEFB0:
    lwz r0, 0x4(r3)
    addi r3, r3, 0x8c
    mtctr r0
    cmpwi r0, 0x0
    ble @L_801DEFE0
@L_801DEFC4:
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    bne @L_801DEFD8
    lwz r28, 0x4(r3)
    b @L_801DEFE4
@L_801DEFD8:
    addi r3, r3, 0x8
    bdnz @L_801DEFC4
@L_801DEFE0:
    li r28, 0x0
@L_801DEFE4:
    mr r3, r31
    addi r4, r1, 0xc
    addi r5, r1, 0x8
    bl fn_800EC578
    mr r3, r31
    mr r4, r29
    bl fn_800ECB74
    lfs f1, -21208(r2)
    mr r3, r31
    bl fn_800EC9DC
    lwz r0, 0xc(r1)
    cmpw r28, r0
    bne @L_801DF030
    rlwinm. r0, r30, 0, 24, 31
    bne @L_801DF030
    mr r3, r31
    bl fn_800EC954
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801DF048
@L_801DF030:
    mr r3, r31
    mr r4, r28
    bl fn_800ECCA8
    lfs f1, -21204(r2)
    mr r3, r31
    bl fn_800ECA78
@L_801DF048:
    mr r3, r31
    bl fn_800EC990
@L_801DF050:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
