    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr. r30, r3
    beq @L_801DCE90
    lbz r0, 0x4f(r30)
    cmplwi r0, 0x0
    bne @L_801DCE90
    lbz r0, 0x4e(r30)
    cmplwi r0, 0x0
    beq @L_801DCE90
    lbz r0, 0x4c(r30)
    lwz r31, 0x24(r30)
    cmplwi r0, 0x0
    beq @L_801DCE68
    lwz r4, 0x38(r30)
    mr r3, r31
    lwz r5, 0x3c(r30)
    lwz r6, 0x40(r30)
    lwz r7, 0x44(r30)
    bl fn_800E5E34
@L_801DCE68:
    lbz r0, 0x4d(r30)
    cmplwi r0, 0x0
    beq @L_801DCE88
    li r0, 0xff
    mr r3, r31
    stb r0, 0x4b(r30)
    addi r4, r30, 0x48
    bl fn_800E5BE0
@L_801DCE88:
    li r0, 0x1
    stb r0, 0x4f(r30)
@L_801DCE90:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
