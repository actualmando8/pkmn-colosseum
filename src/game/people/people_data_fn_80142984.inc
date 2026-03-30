    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0x1
    li r6, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    li r3, 0x0
    mr r4, r31
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_801429BC
    li r3, 0x0
    b @L_801429D4
    @L_801429BC:
    lwz r3, lbl_80478BD8(r13)
    clrlwi r0, r31, 16
    xor r0, r3, r0
    cntlzw r0, r0
    slw r0, r3, r0
    srwi r3, r0, 31
    @L_801429D4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
