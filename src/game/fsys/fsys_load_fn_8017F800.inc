    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r4, lbl_80454038@ha
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    li r28, 0x0
    stw r3, 0x8(r1)
    lwz r31, lbl_80454038@l(r4)
    b @L_8017F900
    @L_8017F830:
    lwz r0, 0x1c(r31)
    cmpwi r0, 0x0
    beq @L_8017F8F8
    lwz r3, 0x10(r31)
    lwz r0, 0x8(r1)
    cmplw r3, r0
    bne @L_8017F8F8
    lwz r29, 0x8(r31)
    lwz r30, 0x4(r31)
    cmplwi r29, 0x0
    beq @L_8017F864
    lwz r0, 0x4(r31)
    stw r0, 0x4(r29)
    @L_8017F864:
    cmplwi r30, 0x0
    beq @L_8017F874
    lwz r0, 0x8(r31)
    stw r0, 0x8(r30)
    @L_8017F874:
    lis r3, lbl_80454038@ha
    li r5, 0x0
    addi r7, r3, lbl_80454038@l
    li r4, 0x0
    lwz r6, 0xc(r7)
    li r3, 0x0
    li r0, 0x0
    subi r6, r6, 0x1
    stw r6, 0xc(r7)
    stw r5, 0xc(r31)
    stw r4, 0x10(r31)
    stw r3, 0x14(r31)
    stw r0, 0x18(r31)
    lwz r0, 0x0(r31)
    cmplwi r0, 0x0
    beq @L_8017F8C4
    lwz r3, 0x0(r31)
    bl fn_8017FB08
    li r0, 0x0
    stw r0, 0x0(r31)
    @L_8017F8C4:
    li r0, 0x0
    lis r3, lbl_80454038@ha
    stw r0, 0x1c(r31)
    addi r3, r3, lbl_80454038@l
    lwz r0, 0x4(r3)
    cmplw r0, r31
    bne @L_8017F8F8
    li r0, 0x0
    lis r3, lbl_80454038@ha
    stw r0, 0x8(r31)
    addi r3, r3, lbl_80454038@l
    lwz r0, 0x4(r31)
    stw r0, 0x4(r3)
    @L_8017F8F8:
    lwz r31, 0x8(r31)
    addi r28, r28, 0x1
    @L_8017F900:
    cmplwi r31, 0x0
    bne @L_8017F830
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
