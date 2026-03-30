    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lhz r4, 0x6(r3)
    addi r0, r4, 0x1
    sth r0, 0x6(r3)
    lhz r0, 0x6(r3)
    cmplwi r0, 0x0
    bne @L_801A0CD8
    lis r3, lbl_80274AF4@ha
    lis r5, lbl_80274B00@ha
    addi r3, r3, lbl_80274AF4@l
    li r4, 0x9e
    addi r5, r5, lbl_80274B00@l
    bl fn_80196E10
    @L_801A0CD8:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
