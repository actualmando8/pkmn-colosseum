    nofralloc
    stwu r1, -0x10(r1)
    lis r6, lbl_80454038@ha
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    li r30, 0x0
    lwz r31, lbl_80454038@l(r6)
    b @L_8017F778
    @L_8017F744:
    lwz r0, 0x10(r31)
    cmplw r0, r3
    bne @L_8017F770
    lwz r0, 0x14(r31)
    cmplw r0, r4
    bne @L_8017F770
    lwz r0, 0x18(r31)
    cmplw r0, r5
    bne @L_8017F770
    lwz r3, 0xc(r31)
    b @L_8017F784
    @L_8017F770:
    lwz r31, 0x8(r31)
    addi r30, r30, 0x1
    @L_8017F778:
    cmplwi r31, 0x0
    bne @L_8017F744
    li r3, 0x0
    @L_8017F784:
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    addi r1, r1, 0x10
    blr
