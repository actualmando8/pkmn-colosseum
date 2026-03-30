    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r0, lbl_8047ACC0(r13)
    lwz r31, lbl_8047ACB0(r13)
    mtctr r0
    cmplwi r0, 0x0
    beq @L_800FF0E8
@L_800FF0C4:
    lwz r0, 0x8(r31)
    cmpwi r0, 0x3
    bne @L_800FF0E0
    lwz r0, 0x18(r31)
    cmplw r0, r3
    bne @L_800FF0E0
    b @L_800FF0EC
@L_800FF0E0:
    addi r31, r31, 0x24
    bdnz @L_800FF0C4
@L_800FF0E8:
    li r31, 0x0
@L_800FF0EC:
    cmplwi r31, 0x0
    beq @L_800FF164
    lwz r0, 0xc(r31)
    cmpwi r0, 0x1
    bne @L_800FF114
    lwz r0, 0x1c(r31)
    cmplwi r0, 0x0
    beq @L_800FF114
    lhz r3, 0x20(r31)
    bl fn_800F7274
@L_800FF114:
    li r0, 0x0
    stw r0, 0x8(r31)
    lwz r3, 0x0(r31)
    cmplwi r3, 0x0
    beq @L_800FF130
    lwz r0, 0x4(r31)
    stw r0, 0x4(r3)
@L_800FF130:
    lwz r3, 0x4(r31)
    cmplwi r3, 0x0
    beq @L_800FF144
    lwz r0, 0x0(r31)
    stw r0, 0x0(r3)
@L_800FF144:
    lwz r0, lbl_8047ACCC(r13)
    cmplw r0, r31
    bne @L_800FF158
    lwz r0, 0x4(r31)
    stw r0, lbl_8047ACCC(r13)
@L_800FF158:
    li r0, 0x0
    stw r0, 0x0(r31)
    stw r0, 0x4(r31)
@L_800FF164:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

