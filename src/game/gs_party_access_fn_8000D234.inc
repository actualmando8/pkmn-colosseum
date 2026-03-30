    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x13
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8000D25C
    li r3, 0x13
    bl fn_80102510
    @L_8000D25C:
    lis r3, lbl_804673F8@ha
    addi r3, r3, lbl_804673F8@l
    lwz r0, 0x874(r3)
    cmpwi r0, 0x0
    bne @L_8000D27C
    li r3, 0x3e7
    bl fn_800FF730
    bl fn_800F0308
    @L_8000D27C:
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
