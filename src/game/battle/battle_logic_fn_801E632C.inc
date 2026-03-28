    nofralloc
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    lwz r3, -21104(r13)
    lwz r0, 0x69c(r3)
    addi r4, r3, 0x6a4
    lwz r3, 0x6a4(r3)
    clrrwi r5, r0, 2
    cmplwi r3, 0x21
    clrlwi r0, r0, 30
    beq @L_801E6368
    subfic r0, r0, 0x3
    slwi r0, r0, 3
    subf r0, r0, r3
    stw r0, 0x0(r4)
    b @L_801E6374
@L_801E6368:
    slwi r3, r0, 3
    addi r0, r3, 0x1
    stw r0, 0x0(r4)
@L_801E6374:
    lwz r4, -21104(r13)
    li r3, 0x0
    li r8, 0x0
    stw r5, 0x69c(r4)
    lwz r0, 0x0(r5)
    lwz r4, -21104(r13)
    stw r0, 0x6a0(r4)
@L_801E6390:
    lwz r4, -21104(r13)
    li r0, 0x1
    slw r0, r0, r3
    lbz r4, 0x6a8(r4)
    and. r0, r4, r0
    beq @L_801E64BC
    li r0, 0x10
    mtctr r0
    addi r7, r8, 0x0
    li r12, 0x0
@L_801E63B8:
    lwz r0, -21104(r13)
    li r5, 0xff
    li r31, 0x0
    add r4, r0, r12
    addi r0, r4, 0x300
    stbx r5, r8, r0
    b @L_801E6428
@L_801E63D4:
    lwz r11, -21104(r13)
    subfic r4, r31, 0x4
    slwi r0, r31, 2
    add r5, r8, r11
    add r10, r5, r0
    lwz r0, 0x348(r10)
    srw r9, r12, r4
    cmpw r9, r0
    bgt @L_801E6424
    lwz r6, 0x340(r5)
    addi r5, r31, 0x1
    lwz r4, 0x390(r10)
    addi r0, r11, 0x300
    li r31, 0x63
    add r4, r4, r6
    lbzx r4, r9, r4
    stbx r4, r7, r0
    lwz r4, -21104(r13)
    addi r0, r4, 0x320
    stbx r5, r7, r0
@L_801E6424:
    addi r31, r31, 0x1
@L_801E6428:
    cmplwi r31, 0x5
    blt @L_801E63D4
    lwz r0, -21104(r13)
    addi r12, r12, 0x1
    li r5, 0xff
    add r4, r0, r12
    addi r0, r4, 0x300
    stbx r5, r8, r0
    li r31, 0x0
    addi r7, r7, 0x1
    b @L_801E64A8
@L_801E6454:
    lwz r11, -21104(r13)
    subfic r4, r31, 0x4
    slwi r0, r31, 2
    add r5, r8, r11
    add r10, r5, r0
    lwz r0, 0x348(r10)
    srw r9, r12, r4
    cmpw r9, r0
    bgt @L_801E64A4
    lwz r6, 0x340(r5)
    addi r5, r31, 0x1
    lwz r4, 0x390(r10)
    addi r0, r11, 0x300
    li r31, 0x63
    add r4, r4, r6
    lbzx r4, r9, r4
    stbx r4, r7, r0
    lwz r4, -21104(r13)
    addi r0, r4, 0x320
    stbx r5, r7, r0
@L_801E64A4:
    addi r31, r31, 0x1
@L_801E64A8:
    cmplwi r31, 0x5
    blt @L_801E6454
    addi r7, r7, 0x1
    addi r12, r12, 0x1
    bdnz @L_801E63B8
@L_801E64BC:
    addi r3, r3, 0x1
    cmplwi r3, 0x4
    addi r8, r8, 0xe0
    blt @L_801E6390
    lwz r9, -21104(r13)
    lbz r4, 0x682(r9)
    lbz r0, 0x688(r9)
    lbz r3, 0x68e(r9)
    slwi r5, r4, 1
    lbz r7, 0x687(r9)
    slwi r4, r0, 1
    lbz r6, 0x68d(r9)
    lbz r0, 0x681(r9)
    slwi r3, r3, 1
    slwi r7, r7, 1
    slwi r6, r6, 1
    addi r5, r5, 0x1
    addi r4, r4, 0x1
    addi r3, r3, 0x1
    slwi r0, r0, 1
    mulli r8, r0, 0xe0
    mulli r7, r7, 0xe0
    mulli r6, r6, 0xe0
    mulli r5, r5, 0xe0
    mulli r4, r4, 0xe0
    mulli r3, r3, 0xe0
    addi r8, r8, 0x300
    addi r7, r7, 0x300
    addi r6, r6, 0x300
    addi r5, r5, 0x300
    addi r4, r4, 0x300
    addi r0, r3, 0x300
    add r8, r9, r8
    add r3, r9, r7
    stw r8, -21376(r13)
    add r6, r9, r6
    add r5, r9, r5
    stw r3, -21344(r13)
    add r3, r9, r4
    add r0, r9, r0
    stw r6, -21312(r13)
    stw r5, -21280(r13)
    stw r3, -21248(r13)
    stw r0, -21216(r13)
    lwz r31, 0x14(r1)
    addi r1, r1, 0x18
    blr
