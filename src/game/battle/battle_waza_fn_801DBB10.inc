    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr. r29, r3
    beq @L_801DBC14
    lbz r0, 0x14(r29)
    lwz r30, 0x3c(r29)
    cmplwi r0, 0x0
    beq @L_801DBC14
    lbz r0, 0x16(r29)
    cmplwi r0, 0x0
    beq @L_801DBB54
    mr r3, r30
    bl fn_801D3034
@L_801DBB54:
    lbz r0, 0x75(r30)
    cmplwi r0, 0x0
    bne @L_801DBB70
    mr r3, r30
    li r4, 0x1
    li r5, 0x0
    bl fn_801DEF0C
@L_801DBB70:
    lwz r3, 0x24(r30)
    li r4, 0x0
    bl fn_800E3CC8
    lwz r0, 0x8(r29)
    rlwinm. r0, r0, 0, 4, 4
    beq @L_801DBB94
    lwz r3, 0x24(r30)
    lwz r4, 0x28(r30)
    bl fn_800E3C08
@L_801DBB94:
    lwz r0, 0x8(r29)
    rlwinm. r0, r0, 0, 5, 5
    beq @L_801DBBA4
    bl fn_801C2B2C
@L_801DBBA4:
    lwz r31, 0x24(r29)
    b @L_801DBBBC
@L_801DBBAC:
    mr r3, r31
    li r4, 0x1
    bl fn_801D7E58
    lwz r31, 0xa8(r31)
@L_801DBBBC:
    cmplwi r31, 0x0
    bne @L_801DBBAC
    lwz r31, 0x24(r29)
    b @L_801DBBFC
@L_801DBBCC:
    lwz r0, 0x4(r31)
    cmpwi r0, 0x3
    bne @L_801DBBF8
    lwz r0, 0x18(r31)
    cmpwi r0, 0x0
    bne @L_801DBBF8
    lwz r3, 0x88(r31)
    cmplwi r3, 0x0
    beq @L_801DBBF8
    li r4, 0x1
    bl fn_80118874
@L_801DBBF8:
    lwz r31, 0xa8(r31)
@L_801DBBFC:
    cmplwi r31, 0x0
    bne @L_801DBBCC
    li r0, 0x0
    stw r0, 0x6c(r30)
    stb r0, 0x14(r29)
    stb r0, 0x15(r29)
@L_801DBC14:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
