    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr. r30, r3
    beq @L_801DBCB4
    lbz r0, 0x14(r30)
    lwz r31, 0x3c(r30)
    cmplwi r0, 0x0
    beq @L_801DBCB4
    lwz r0, 0x6c(r31)
    cmplw r0, r30
    bne @L_801DBCB4
    lwz r0, 0xc(r30)
    cmpwi r0, 0xb
    bge @L_801DBC7C
    cmpwi r0, 0x9
    bge @L_801DBC8C
@L_801DBC7C:
    mr r3, r31
    li r4, 0x1
    li r5, 0x0
    bl fn_801DEF0C
@L_801DBC8C:
    lbz r0, 0x16(r30)
    cmplwi r0, 0x0
    beq @L_801DBCA0
    mr r3, r31
    bl fn_801D3034
@L_801DBCA0:
    lwz r3, 0x24(r31)
    li r4, 0x0
    bl fn_800E3CC8
    li r0, 0x0
    stw r0, 0x6c(r31)
@L_801DBCB4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
