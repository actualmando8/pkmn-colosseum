    nofralloc
    li r6, 0x0
    li r7, 0x1
@L_801E5FCC:
    lwz r3, -21212(r13)
    subi r0, r7, 0x1
    clrlwi r5, r7, 24
    lbzx r8, r3, r0
    cmpwi r8, 0x0
    addi r3, r8, 0x0
    beq @L_801E6098
    srwi. r0, r3, 3
    mtctr r0
    beq @L_801E6080
@L_801E5FF4:
    lwz r4, -21208(r13)
    mr r0, r6
    addi r6, r6, 0x1
    stbx r5, r4, r0
    addi r0, r6, 0x0
    addi r6, r6, 0x1
    lwz r4, -21208(r13)
    stbx r5, r4, r0
    addi r0, r6, 0x0
    addi r6, r6, 0x1
    lwz r4, -21208(r13)
    stbx r5, r4, r0
    addi r0, r6, 0x0
    addi r6, r6, 0x1
    lwz r4, -21208(r13)
    stbx r5, r4, r0
    addi r0, r6, 0x0
    addi r6, r6, 0x1
    lwz r4, -21208(r13)
    stbx r5, r4, r0
    addi r0, r6, 0x0
    addi r6, r6, 0x1
    lwz r4, -21208(r13)
    stbx r5, r4, r0
    addi r0, r6, 0x0
    addi r6, r6, 0x1
    lwz r4, -21208(r13)
    stbx r5, r4, r0
    addi r0, r6, 0x0
    addi r6, r6, 0x1
    lwz r4, -21208(r13)
    stbx r5, r4, r0
    bdnz @L_801E5FF4
    andi. r3, r3, 0x7
    beq @L_801E6098
@L_801E6080:
    mtctr r3
@L_801E6084:
    lwz r4, -21208(r13)
    mr r0, r6
    addi r6, r6, 0x1
    stbx r5, r4, r0
    bdnz @L_801E6084
@L_801E6098:
    addi r7, r7, 0x1
    cmpwi r7, 0x10
    ble @L_801E5FCC
    lwz r3, -21208(r13)
    li r0, 0x0
    stbx r0, r3, r6
    blr
