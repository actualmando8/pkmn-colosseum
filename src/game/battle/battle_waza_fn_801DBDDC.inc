    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    stw r30, 0x8(r1)
    beq @L_801DBF98
    beq @L_801DBF50
    lwz r30, 0x24(r31)
    cmplwi r30, 0x0
    beq @L_801DBEFC
    b @L_801DBE10
@L_801DBE0C:
    mr r30, r0
@L_801DBE10:
    lwz r0, 0xa8(r30)
    cmplwi r0, 0x0
    bne @L_801DBE0C
    b @L_801DBEF4
@L_801DBE20:
    lwz r0, 0x4(r30)
    cmpwi r0, 0x3
    beq @L_801DBE8C
    bge @L_801DBE44
    cmpwi r0, 0x2
    bge @L_801DBE58
    cmpwi r0, 0x0
    bge @L_801DBEF0
    b @L_801DBEE0
@L_801DBE44:
    cmpwi r0, 0x7
    bge @L_801DBEE0
    cmpwi r0, 0x5
    bge @L_801DBEF0
    b @L_801DBED4
@L_801DBE58:
    lwz r3, 0x78(r30)
    cmplwi r3, 0x0
    beq @L_801DBEF0
    lwz r4, 0x7c(r30)
    cmplwi r4, 0x0
    beq @L_801DBE74
    bl fn_800F9210
@L_801DBE74:
    lwz r4, 0x80(r30)
    cmplwi r4, 0x0
    beq @L_801DBEF0
    lwz r3, 0x78(r30)
    bl fn_800F9210
    b @L_801DBEF0
@L_801DBE8C:
    lwz r0, 0x18(r30)
    cmpwi r0, 0x0
    beq @L_801DBEA8
    li r0, 0x0
    stw r0, 0x8c(r30)
    stw r0, 0x88(r30)
    b @L_801DBEC4
@L_801DBEA8:
    lwz r3, 0x88(r30)
    cmplwi r3, 0x0
    beq @L_801DBEC4
    bl fn_801193BC
    lwz r3, 0x78(r30)
    lwz r4, 0x7c(r30)
    bl fn_800F9210
@L_801DBEC4:
    li r0, 0x0
    stw r0, 0x8c(r30)
    stw r0, 0x88(r30)
    b @L_801DBEF0
@L_801DBED4:
    lwz r3, 0x78(r30)
    bl fn_80131268
    b @L_801DBEF0
@L_801DBEE0:
    lis r3, -32728
    addi r3, r3, -26276
    crxor 6, 6, 6
    bl fn_800DD970
@L_801DBEF0:
    lwz r30, 0xac(r30)
@L_801DBEF4:
    cmplwi r30, 0x0
    bne @L_801DBE20
@L_801DBEFC:
    lhz r30, 0x28(r31)
    cmplwi r30, 0x0
    beq @L_801DBF18
    mr r3, r30
    bl fn_800E24B0
    mr r3, r30
    bl fn_800E209C
@L_801DBF18:
    lwz r3, 0x18(r31)
    cmplwi r3, 0x0
    beq @L_801DBF48
    lwz r4, 0x20(r31)
    cmplwi r4, 0x0
    beq @L_801DBF34
    bl fn_800F9210
@L_801DBF34:
    lwz r4, 0x1c(r31)
    cmplwi r4, 0x0
    beq @L_801DBF48
    lwz r3, 0x18(r31)
    bl fn_800F9210
@L_801DBF48:
    mr r3, r31
    bl fn_801DB288
@L_801DBF50:
    lwz r4, 0x34(r31)
    lwz r3, 0x3c(r31)
    cmplwi r4, 0x0
    lwz r5, 0x38(r31)
    beq @L_801DBF68
    stw r5, 0x38(r4)
@L_801DBF68:
    cmplwi r5, 0x0
    beq @L_801DBF78
    stw r4, 0x34(r5)
    b @L_801DBF7C
@L_801DBF78:
    stw r4, 0x68(r3)
@L_801DBF7C:
    lhz r30, 0x2a(r31)
    cmplwi r30, 0x0
    beq @L_801DBF98
    mr r3, r30
    bl fn_800E24B0
    mr r3, r30
    bl fn_800E209C
@L_801DBF98:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
