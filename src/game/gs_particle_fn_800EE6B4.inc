    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r3, 0x8(r3)
    lwz r0, 0x14(r3)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_800EE6E4
    lwz r5, 0x18(r3)
    b @L_800EE6E8
    @L_800EE6E4:
    li r5, 0x0
    @L_800EE6E8:
    cmplwi r5, 0x0
    bne @L_800EE6F8
    li r3, 0x0
    b @L_800EE744
    @L_800EE6F8:
    li r3, 0x0
    b @L_800EE718
    @L_800EE700:
    cmplw r3, r4
    addi r3, r3, 0x1
    bne @L_800EE714
    lwz r31, 0x8(r5)
    b @L_800EE724
    @L_800EE714:
    lwz r5, 0x4(r5)
    @L_800EE718:
    cmplwi r5, 0x0
    bne @L_800EE700
    li r31, 0x0
    @L_800EE724:
    cmplwi r31, 0x0
    bne @L_800EE734
    li r3, 0x0
    b @L_800EE744
    @L_800EE734:
    bl fn_800DF7A4
    cmplwi r3, 0x0
    beq @L_800EE744
    stw r31, 0x8(r3)
    @L_800EE744:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
