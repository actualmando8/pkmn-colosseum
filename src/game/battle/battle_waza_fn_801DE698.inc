    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    stw r28, 0x20(r1)
    lwz r5, 0x0(r4)
    rlwinm r0, r5, 0, 30, 31
    cmplwi r0, 0x1
    bne @L_801DEC44
    rlwinm r0, r5, 0, 29, 29
    cmplwi r0, 0x4
    beq @L_801DEC44
    lwz r31, 0x8(r4)
    cmplwi r31, 0x0
    beq @L_801DEC44
    lwz r0, 0x24(r31)
    cmplw r3, r0
    bne @L_801DEC44
    lbz r0, 0x16(r31)
    cmplwi r0, 0x0
    bne @L_801DEC44
    lhz r3, 0x32(r31)
    lhz r0, 0x34(r31)
    mulli r3, r3, 0xd4
    lwz r4, 0x2c(r31)
    cmplwi r0, 0x0
    add r5, r4, r3
    bne @L_801DE840
    lwz r3, 0x90(r5)
    cmpwi r3, 0x0
    beq @L_801DE840
    subi r0, r3, 0x1
    cmplwi r31, 0x0
    stw r0, 0x90(r5)
    beq @L_801DEC44
    lbz r0, 0x16(r31)
    cmplwi r0, 0x0
    bne @L_801DEC44
    lbz r3, 0x18(r31)
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x8
    beq @L_801DEC44
    rlwinm r0, r3, 0, 29, 29
    lwz r30, 0x24(r31)
    cmpwi r0, 0x4
    beq @L_801DE76C
    li r0, 0x0
    mr r3, r30
    stb r0, 0x19(r31)
    li r4, 0x1
    bl fn_800EC188
@L_801DE76C:
    lbz r0, 0x18(r31)
    lwz r4, 0x2c(r31)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x2
    mr r3, r4
    bne @L_801DE7A4
    lhz r0, 0x14(r31)
    cmplwi r0, 0x10
    ble @L_801DE7A4
    addi r3, r4, 0xd40
    lwz r0, 0xdd4(r4)
    cmpwi r0, 0x1
    bne @L_801DE7A4
    mr r3, r4
@L_801DE7A4:
    lwz r0, 0x4(r3)
    addi r3, r3, 0x8c
    mtctr r0
    cmpwi r0, 0x0
    ble @L_801DE7D4
@L_801DE7B8:
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    bne @L_801DE7CC
    lwz r28, 0x4(r3)
    b @L_801DE7D8
@L_801DE7CC:
    addi r3, r3, 0x8
    bdnz @L_801DE7B8
@L_801DE7D4:
    li r28, 0x0
@L_801DE7D8:
    mr r3, r30
    addi r4, r1, 0x18
    addi r5, r1, 0x1c
    bl fn_800EC578
    mr r3, r30
    li r4, 0x0
    bl fn_800ECB74
    lfs f1, -21208(r2)
    mr r3, r30
    bl fn_800EC9DC
    lwz r0, 0x18(r1)
    cmpw r28, r0
    bne @L_801DE81C
    mr r3, r30
    bl fn_800EC954
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801DE834
@L_801DE81C:
    mr r3, r30
    mr r4, r28
    bl fn_800ECCA8
    lfs f1, -21204(r2)
    mr r3, r30
    bl fn_800ECA78
@L_801DE834:
    mr r3, r30
    bl fn_800EC990
    b @L_801DEC44
@L_801DE840:
    lhz r3, 0x34(r31)
    addi r0, r3, 0x1
    sth r0, 0x34(r31)
    lhz r3, 0x34(r31)
    lwz r0, 0x4(r5)
    cmpw r3, r0
    blt @L_801DE998
    lhz r0, 0x32(r31)
    cmpwi r0, 0xb
    bge @L_801DE870
    cmpwi r0, 0x9
    bge @L_801DEC44
@L_801DE870:
    cmplwi r31, 0x0
    beq @L_801DE988
    lbz r0, 0x16(r31)
    cmplwi r0, 0x0
    bne @L_801DE988
    lbz r3, 0x18(r31)
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x8
    beq @L_801DE988
    rlwinm r0, r3, 0, 29, 29
    lwz r30, 0x24(r31)
    cmpwi r0, 0x4
    beq @L_801DE8B8
    li r0, 0x0
    mr r3, r30
    stb r0, 0x19(r31)
    li r4, 0x1
    bl fn_800EC188
@L_801DE8B8:
    lbz r0, 0x18(r31)
    lwz r4, 0x2c(r31)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x2
    mr r3, r4
    bne @L_801DE8F0
    lhz r0, 0x14(r31)
    cmplwi r0, 0x10
    ble @L_801DE8F0
    addi r3, r4, 0xd40
    lwz r0, 0xdd4(r4)
    cmpwi r0, 0x1
    bne @L_801DE8F0
    mr r3, r4
@L_801DE8F0:
    lwz r0, 0x4(r3)
    addi r3, r3, 0x8c
    mtctr r0
    cmpwi r0, 0x0
    ble @L_801DE920
@L_801DE904:
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    bne @L_801DE918
    lwz r28, 0x4(r3)
    b @L_801DE924
@L_801DE918:
    addi r3, r3, 0x8
    bdnz @L_801DE904
@L_801DE920:
    li r28, 0x0
@L_801DE924:
    mr r3, r30
    addi r4, r1, 0x10
    addi r5, r1, 0x14
    bl fn_800EC578
    mr r3, r30
    li r4, 0x1
    bl fn_800ECB74
    lfs f1, -21208(r2)
    mr r3, r30
    bl fn_800EC9DC
    lwz r0, 0x10(r1)
    cmpw r28, r0
    bne @L_801DE968
    mr r3, r30
    bl fn_800EC954
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801DE980
@L_801DE968:
    mr r3, r30
    mr r4, r28
    bl fn_800ECCA8
    lfs f1, -21204(r2)
    mr r3, r30
    bl fn_800ECA78
@L_801DE980:
    mr r3, r30
    bl fn_800EC990
@L_801DE988:
    mr r3, r31
    li r4, 0x0
    bl fn_801DD100
    b @L_801DEC44
@L_801DE998:
    clrlslwi r3, r3, 16, 3
    addi r4, r3, 0x8c
    add r4, r5, r4
    lwz r0, 0x0(r4)
    cmpwi r0, 0x0
    bne @L_801DEB48
    lhz r3, 0x32(r31)
    cmplwi r3, 0xa
    bne @L_801DEA8C
    cmplwi r31, 0x0
    beq @L_801DEC44
    lwz r30, 0x24(r31)
    li r0, 0x0
    li r4, 0x1
    stb r0, 0x19(r31)
    mr r3, r30
    bl fn_800EC188
    lwz r3, 0x2c(r31)
    lwz r0, 0x84c(r3)
    addi r3, r3, 0x8d4
    mtctr r0
    cmpwi r0, 0x0
    ble @L_801DEA10
@L_801DE9F4:
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    bne @L_801DEA08
    lwz r4, 0x4(r3)
    b @L_801DEA14
@L_801DEA08:
    addi r3, r3, 0x8
    bdnz @L_801DE9F4
@L_801DEA10:
    li r4, 0x0
@L_801DEA14:
    mr r3, r30
    bl fn_800ECCA8
    lbz r0, 0x75(r31)
    cmplwi r0, 0x0
    beq @L_801DEA38
    mr r3, r30
    li r4, 0x0
    bl fn_800ECB74
    b @L_801DEA44
@L_801DEA38:
    mr r3, r30
    li r4, 0x1
    bl fn_800ECB74
@L_801DEA44:
    lfs f1, -21208(r2)
    mr r3, r30
    bl fn_800EC9DC
    lfs f1, -21204(r2)
    mr r3, r30
    bl fn_800ECA78
    mr r3, r30
    bl fn_800EC990
    lbz r0, 0x75(r31)
    cmplwi r0, 0x0
    beq @L_801DEC44
    mr r3, r30
    li r4, 0x0
    li r5, 0x0
    bl fn_800EC154
    li r0, 0x1
    stb r0, 0x16(r31)
    b @L_801DEC44
@L_801DEA8C:
    cmplwi r31, 0x0
    lwz r29, 0x4(r4)
    li r28, 0x0
    beq @L_801DEC44
    lbz r0, 0x75(r31)
    cmplwi r0, 0x0
    bne @L_801DEAD8
    cmpwi r3, 0x6
    bge @L_801DEAC8
    cmpwi r3, 0x1
    beq @L_801DEAD8
    bge @L_801DEAD4
    cmpwi r3, 0x0
    bge @L_801DEAD4
    b @L_801DEAD8
@L_801DEAC8:
    cmpwi r3, 0xa
    beq @L_801DEAD4
    b @L_801DEAD8
@L_801DEAD4:
    li r28, 0x1
@L_801DEAD8:
    lbz r3, 0x18(r31)
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x8
    beq @L_801DEC44
    rlwinm r0, r3, 0, 29, 29
    lwz r30, 0x24(r31)
    cmpwi r0, 0x4
    beq @L_801DEB0C
    li r0, 0x0
    mr r3, r30
    stb r0, 0x19(r31)
    li r4, 0x1
    bl fn_800EC188
@L_801DEB0C:
    mr r3, r30
    mr r4, r29
    bl fn_800ECCA8
    mr r3, r30
    mr r4, r28
    bl fn_800ECB74
    lfs f1, -21208(r2)
    mr r3, r30
    bl fn_800EC9DC
    lfs f1, -21204(r2)
    mr r3, r30
    bl fn_800ECA78
    mr r3, r30
    bl fn_800EC990
    b @L_801DEC44
@L_801DEB48:
    cmplwi r31, 0x0
    beq @L_801DEC44
    lbz r0, 0x16(r31)
    cmplwi r0, 0x0
    bne @L_801DEC44
    lbz r3, 0x18(r31)
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x8
    beq @L_801DEC44
    rlwinm r0, r3, 0, 29, 29
    lwz r28, 0x24(r31)
    cmpwi r0, 0x4
    beq @L_801DEB90
    li r0, 0x0
    mr r3, r28
    stb r0, 0x19(r31)
    li r4, 0x1
    bl fn_800EC188
@L_801DEB90:
    lbz r0, 0x18(r31)
    lwz r4, 0x2c(r31)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x2
    mr r3, r4
    bne @L_801DEBC8
    lhz r0, 0x14(r31)
    cmplwi r0, 0x10
    ble @L_801DEBC8
    addi r3, r4, 0xd40
    lwz r0, 0xdd4(r4)
    cmpwi r0, 0x1
    bne @L_801DEBC8
    mr r3, r4
@L_801DEBC8:
    lwz r0, 0x4(r3)
    addi r3, r3, 0x8c
    mtctr r0
    cmpwi r0, 0x0
    ble @L_801DEBF8
@L_801DEBDC:
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    bne @L_801DEBF0
    lwz r29, 0x4(r3)
    b @L_801DEBFC
@L_801DEBF0:
    addi r3, r3, 0x8
    bdnz @L_801DEBDC
@L_801DEBF8:
    li r29, 0x0
@L_801DEBFC:
    mr r3, r28
    addi r4, r1, 0x8
    addi r5, r1, 0xc
    bl fn_800EC578
    mr r3, r28
    li r4, 0x0
    bl fn_800ECB74
    lfs f1, -21208(r2)
    mr r3, r28
    bl fn_800EC9DC
    mr r3, r28
    mr r4, r29
    bl fn_800ECCA8
    lfs f1, -21204(r2)
    mr r3, r28
    bl fn_800ECA78
    mr r3, r28
    bl fn_800EC990
@L_801DEC44:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
