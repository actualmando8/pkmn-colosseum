    nofralloc
    lis r5, lbl_80466DE8@ha
    cmplwi r3, 0x0
    addi r6, r5, lbl_80466DE8@l
    mr r8, r6
    beqlr
    cmplwi r4, 0x0
    bne @L_801C3E5C
    blr
    @L_801C3E5C:
    lhz r0, 0x42(r6)
    cmplwi r0, 0x8
    bgelr
    li r9, 0x0
    b @L_801C3F00
    @L_801C3E70:
    lwz r0, 0x0(r8)
    cmplw r0, r3
    bne @L_801C3EF8
    lhz r0, 0xc(r8)
    cmplwi r0, 0x2
    bgelr
    li r10, 0x0
    b @L_801C3EEC
    @L_801C3E90:
    clrlslwi r5, r10, 16, 2
    clrlwi r7, r10, 16
    addi r0, r5, 0x4
    lwzx r0, r8, r0
    cmplwi r0, 0x0
    bne @L_801C3EE8
    slwi r0, r7, 2
    li r3, 0x1
    add r5, r8, r0
    stw r4, 0x4(r5)
    lhz r5, 0xc(r8)
    addi r0, r5, 0x1
    sth r0, 0xc(r8)
    lhz r5, 0x42(r6)
    addi r0, r5, 0x1
    sth r0, 0x42(r6)
    lbz r0, 0xe(r8)
    cmplwi r0, 0x0
    beq @L_801C3EE0
    li r3, -0x1
    @L_801C3EE0:
    stb r3, 0x76(r4)
    blr
    @L_801C3EE8:
    addi r10, r10, 0x1
    @L_801C3EEC:
    clrlwi r0, r10, 16
    cmplwi r0, 0x2
    blt @L_801C3E90
    @L_801C3EF8:
    addi r9, r9, 0x1
    addi r8, r8, 0x10
    @L_801C3F00:
    clrlwi r0, r9, 16
    cmplwi r0, 0x4
    blt @L_801C3E70
    blr
