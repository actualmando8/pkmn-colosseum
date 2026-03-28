    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lbz r0, -21472(r13)
    cmplwi r0, 0x0
    beq @L_801E17CC
    lbz r0, -21471(r13)
    cmplwi r0, 0x0
    bne @L_801E17D4
@L_801E17CC:
    li r0, 0x0
    b @L_801E17D8
@L_801E17D4:
    li r0, 0x1
@L_801E17D8:
    clrlwi. r0, r0, 24
    beq @L_801E1800
    lis r4, -32697
    lwz r3, -21444(r13)
    addi r7, r4, -28624
    lwz r4, -21448(r13)
    lwz r5, -21452(r13)
    lwz r6, 0x0(r7)
    lwz r7, 0x4(r7)
    bl fn_801E3978
@L_801E1800:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
