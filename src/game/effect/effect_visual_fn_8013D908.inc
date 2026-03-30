    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr. r5, r3
    stw r0, 0x14(r1)
    beq @L_8013D960
    lwz r3, 0xc(r5)
    li r0, 0x0
    stw r3, 0x10(r5)
    stw r0, 0x14(r5)
    lwz r3, 0x0(r5)
    cmplwi r3, 0x0
    beq @L_8013D960
    lwz r6, lbl_8047AEE0(r13)
    cmplwi r6, 0x0
    beq @L_8013D960
    lwz r7, 0x10(r5)
    lwz r4, 0x4(r5)
    lwz r5, 0x8(r5)
    lfs f1, 0x0(r7)
    bl fn_800E61BC
    li r3, 0x1
    b @L_8013D974
    @L_8013D960:
    lis r3, lbl_80272F38@ha
    addi r3, r3, lbl_80272F38@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    @L_8013D974:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
