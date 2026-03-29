    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r3, lbl_803A1D40@ha
    addi r5, r3, lbl_803A1D40@l
    lbz r3, 0x0(r5)
    lhz r4, 0x12(r5)
    lwz r5, 0xc(r5)
    bl fn_8001B1EC
    li r3, 0x63
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lis r3, lbl_803A1D40@ha
    addi r3, r3, lbl_803A1D40@l
    lbz r0, 0x2(r3)
    cmplwi r0, 0x0
    beq @L_8001BEA8
    bl fn_800FF660
    lis r3, lbl_803A1D40@ha
    addi r3, r3, lbl_803A1D40@l
    lbz r0, 0x3(r3)
    cmplwi r0, 0x1
    beq @L_8001BEA4
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    @L_8001BEA4:
    bl fn_800F0308
    @L_8001BEA8:
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
