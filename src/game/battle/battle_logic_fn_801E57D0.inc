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
    cmplwi r0, 0x8
    beq @L_801E5804
    li r3, 0xa
    blr
@L_801E5804:
    lwz r5, -21104(r13)
    lwz r4, 0x69c(r5)
    lbz r3, 0x0(r4)
    lbz r0, 0x1(r4)
    rlwimi r0, r3, 8, 16, 23
    sth r0, 0x694(r5)
    lwz r4, -21104(r13)
    lwz r3, 0x69c(r4)
    addi r0, r3, 0x2
    stw r0, 0x69c(r4)
    lwz r5, -21104(r13)
    lwz r4, 0x69c(r5)
    lbz r3, 0x0(r4)
    lbz r0, 0x1(r4)
    rlwimi r0, r3, 8, 16, 23
    sth r0, 0x692(r5)
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
    beq @L_801E5878
    li r3, 0xc
    blr
@L_801E5878:
    li r7, 0x0
    li r6, 0x0
    b @L_801E58F8
@L_801E5884:
    lwz r4, -21104(r13)
    clrlwi. r0, r7, 24
    lwz r3, 0x69c(r4)
    addi r0, r3, 0x1
    stw r0, 0x69c(r4)
    lwz r4, -21104(r13)
    lwz r3, 0x69c(r4)
    addi r0, r3, 0x1
    stw r0, 0x69c(r4)
    lbz r3, 0x0(r3)
    bne @L_801E58B8
    cmplwi r3, 0x22
    bne @L_801E58C8
@L_801E58B8:
    clrlwi. r0, r7, 24
    beq @L_801E58D0
    cmplwi r3, 0x11
    beq @L_801E58D0
@L_801E58C8:
    li r3, 0x13
    blr
@L_801E58D0:
    lwz r5, -21104(r13)
    addi r0, r6, 0x680
    addi r6, r6, 0x6
    lwz r4, 0x69c(r5)
    addi r7, r7, 0x1
    addi r3, r4, 0x1
    stw r3, 0x69c(r5)
    lbz r4, 0x0(r4)
    lwz r3, -21104(r13)
    stbx r4, r3, r0
@L_801E58F8:
    clrlwi r0, r7, 24
    cmplwi r0, 0x3
    blt @L_801E5884
    li r3, 0x0
    blr
