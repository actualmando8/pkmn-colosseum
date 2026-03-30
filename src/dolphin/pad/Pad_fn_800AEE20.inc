    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    addi r31, r4, 0x0
    stw r30, 0x10(r1)
    mr. r30, r3
    beq @L_800AEE80
    lwz r3, 0x18(r30)
    bl fn_800AE7CC
    @L_800AEE48:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEE48
    lwz r3, 0x1c(r30)
    bl fn_800AE7CC
    @L_800AEE5C:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEE5C
    lwz r3, 0x20(r30)
    bl fn_800AE7CC
    @L_800AEE70:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEE70
    b @L_800AEEBC
    @L_800AEE80:
    li r3, 0x0
    bl fn_800AE7CC
    @L_800AEE88:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEE88
    li r3, 0x0
    bl fn_800AE7CC
    @L_800AEE9C:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEE9C
    li r3, 0x0
    bl fn_800AE7CC
    @L_800AEEB0:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEEB0
    @L_800AEEBC:
    lwz r3, 0xc(r31)
    bl fn_800AE7CC
    @L_800AEEC4:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEEC4
    lwz r3, 0x10(r31)
    bl fn_800AE7CC
    @L_800AEED8:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEED8
    lwz r3, 0x14(r31)
    bl fn_800AE7CC
    @L_800AEEEC:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEEEC
    lwz r0, 0x0(r31)
    cmplwi r0, 0x0
    bne @L_800AEF58
    lhz r3, 0x24(r31)
    bl fn_800AE7CC
    @L_800AEF0C:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEF0C
    li r3, 0x0
    bl fn_800AE7CC
    @L_800AEF20:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEF20
    li r3, 0x0
    bl fn_800AE7CC
    @L_800AEF34:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEF34
    li r3, 0x0
    bl fn_800AE7CC
    @L_800AEF48:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEF48
    b @L_800AEFA8
    @L_800AEF58:
    lhz r3, 0x26(r31)
    bl fn_800AE7CC
    @L_800AEF60:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEF60
    lwz r3, 0x18(r31)
    bl fn_800AE7CC
    @L_800AEF74:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEF74
    lwz r3, 0x1c(r31)
    bl fn_800AE7CC
    @L_800AEF88:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEF88
    lwz r3, 0x20(r31)
    bl fn_800AE7CC
    @L_800AEF9C:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AEF9C
    @L_800AEFA8:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
