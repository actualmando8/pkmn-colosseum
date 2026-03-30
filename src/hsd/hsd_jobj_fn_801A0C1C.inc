    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lhz r4, 0x4(r3)
    addi r0, r4, 0x1
    sth r0, 0x4(r3)
    lhz r0, 0x4(r3)
    cmplwi r0, 0xffff
    bne @L_801A0C58
    lis r3, lbl_80274AF4@ha
    lis r5, lbl_80274B64@ha
    addi r3, r3, lbl_80274AF4@l
    li r4, 0x5d
    addi r5, r5, lbl_80274B64@l
    bl fn_80196E10
    @L_801A0C58:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
