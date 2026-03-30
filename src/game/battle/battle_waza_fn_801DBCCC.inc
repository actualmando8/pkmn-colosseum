    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r31, r3
    lbz r0, 0x14(r3)
    cmplwi r0, 0x0
    bne @L_801DBDC8
    lwz r30, 0x3c(r31)
    lwz r0, 0x8(r31)
    lwz r3, 0x6c(r30)
    rlwinm r0, r0, 31, 31, 31
    lwz r29, 0x24(r31)
    cmplwi r3, 0x0
    lwz r27, 0x24(r30)
    mr r28, r0
    beq @L_801DBD14
    bl fn_801DBB10
@L_801DBD14:
    lbz r0, 0x75(r30)
    cmplwi r0, 0x0
    beq @L_801DBD34
    lhz r0, 0x2e(r31)
    cmplwi r0, 0x2
    bne @L_801DBD34
    mr r3, r30
    bl fn_801DB864
@L_801DBD34:
    mr r3, r30
    mr r4, r31
    bl fn_801DD100
    lwz r0, 0x8(r31)
    rlwinm. r0, r0, 0, 4, 4
    beq @L_801DBD58
    mr r3, r27
    li r4, 0x0
    bl fn_800E3C08
@L_801DBD58:
    mr r3, r30
    mr r4, r28
    bl fn_801D9C1C
    stw r31, 0x6c(r30)
    li r0, 0x1
    stb r0, 0x14(r31)
    lwz r0, 0x8(r31)
    rlwinm. r0, r0, 0, 5, 5
    beq @L_801DBD84
    mr r3, r30
    bl fn_801C2BE0
@L_801DBD84:
    lbz r0, 0x16(r31)
    cmplwi r0, 0x0
    beq @L_801DBD9C
    mr r3, r30
    mr r4, r31
    bl fn_801D30BC
@L_801DBD9C:
    li r0, 0x0
    b @L_801DBDAC
@L_801DBDA4:
    stw r0, 0x6c(r29)
    lwz r29, 0xa8(r29)
@L_801DBDAC:
    cmplwi r29, 0x0
    bne @L_801DBDA4
    li r0, 0x0
    mr r3, r31
    stw r0, 0x0(r31)
    stb r0, 0x15(r31)
    bl fn_801DB988
@L_801DBDC8:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
