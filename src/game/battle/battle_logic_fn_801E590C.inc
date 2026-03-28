    nofralloc
    lwz r4, -21104(r13)
    lwz r3, 0x69c(r4)
    addi r0, r3, 0x2
    stw r0, 0x69c(r4)
    lwz r4, -21104(r13)
    lwz r3, 0x69c(r4)
    addi r0, r3, 0x1
    stw r0, 0x69c(r4)
    lbz r0, 0x0(r3)
    cmplwi r0, 0x3
    beq @L_801E5940
    li r3, 0xc
    blr
@L_801E5940:
    li r9, 0x0
    li r8, 0x0
    b @L_801E59D0
@L_801E594C:
    lwz r6, -21104(r13)
    addi r0, r8, 0x681
    addi r4, r8, 0x682
    lwz r5, 0x69c(r6)
    li r3, 0x1
    addi r5, r5, 0x1
    stw r5, 0x69c(r6)
    lwz r7, -21104(r13)
    lwz r6, 0x69c(r7)
    addi r5, r6, 0x1
    stw r5, 0x69c(r7)
    lbz r7, 0x0(r6)
    lwz r5, -21104(r13)
    srawi r6, r7, 4
    stbx r6, r5, r0
    clrlwi r7, r7, 28
    slw r0, r3, r6
    lwz r5, -21104(r13)
    stbx r7, r5, r4
    lwz r4, -21104(r13)
    lbz r4, 0x6a8(r4)
    and. r0, r4, r0
    bne @L_801E59B0
    li r3, 0xf
    blr
@L_801E59B0:
    addi r0, r7, 0x1
    slw r0, r3, r0
    and. r0, r4, r0
    bne @L_801E59C8
    li r3, 0xf
    blr
@L_801E59C8:
    addi r8, r8, 0x6
    addi r9, r9, 0x1
@L_801E59D0:
    clrlwi r0, r9, 24
    cmplwi r0, 0x3
    blt @L_801E594C
    lwz r5, -21104(r13)
    li r0, 0x0
    li r3, 0x0
    lwz r4, 0x69c(r5)
    addi r4, r4, 0x3
    stw r4, 0x69c(r5)
    lwz r5, -21104(r13)
    lhz r4, 0x692(r5)
    addi r4, r4, 0xf
    srawi r4, r4, 4
    addze r4, r4
    sth r4, 0x696(r5)
    lwz r4, -21104(r13)
    sth r0, 0x684(r4)
    lwz r4, -21104(r13)
    sth r0, 0x68a(r4)
    lwz r4, -21104(r13)
    sth r0, 0x690(r4)
    blr
