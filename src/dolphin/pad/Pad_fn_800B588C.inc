    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x40(r1)
    stmw r25, 0x24(r1)
    mr. r28, r4
    addi r27, r3, 0x0
    addi r29, r5, 0x0
    addi r30, r6, 0x0
    blt @L_800B58CC
    cmpwi r28, 0x7f
    bge @L_800B58CC
    lbz r0, 0x8(r29)
    cmplwi r0, 0xff
    beq @L_800B58CC
    cmplwi r0, 0x0
    bne @L_800B58D4
    @L_800B58CC:
    li r3, -0x80
    b @L_800B5ACC
    @L_800B58D4:
    addi r3, r27, 0x0
    addi r4, r1, 0x18
    bl fn_800B023C
    cmpwi r3, 0x0
    bge @L_800B58EC
    b @L_800B5ACC
    @L_800B58EC:
    lwz r3, 0x18(r1)
    bl fn_800B1E0C
    mr r0, r3
    lwz r3, 0x18(r1)
    mr r26, r0
    slwi r0, r28, 6
    add r31, r26, r0
    addi r4, r31, 0x0
    bl fn_800B4270
    mr. r4, r3
    bge @L_800B5924
    lwz r3, 0x18(r1)
    bl fn_800B02F4
    b @L_800B5ACC
    @L_800B5924:
    addi r4, r29, 0x8
    addi r3, r29, 0x28
    subf r0, r4, r3
    cmplw r4, r3
    mtctr r0
    bge @L_800B5970
    @L_800B593C:
    lbz r0, 0x0(r4)
    cmplwi r0, 0x0
    bne @L_800B5968
    addi r0, r29, 0x28
    li r3, 0x0
    b @L_800B5958
    @L_800B5954:
    stb r3, 0x0(r4)
    @L_800B5958:
    addi r4, r4, 0x1
    cmplw r4, r0
    blt @L_800B5954
    b @L_800B5970
    @L_800B5968:
    addi r4, r4, 0x1
    bdnz @L_800B593C
    @L_800B5970:
    addi r3, r31, 0x8
    addi r4, r29, 0x8
    li r5, 0x20
    bl fn_800C80D0
    cmpwi r3, 0x0
    bne @L_800B59B8
    addi r3, r31, 0x0
    addi r4, r29, 0x0
    li r5, 0x4
    bl fn_800C80D0
    cmpwi r3, 0x0
    bne @L_800B59B8
    addi r3, r31, 0x4
    addi r4, r29, 0x4
    li r5, 0x2
    bl fn_800C80D0
    cmpwi r3, 0x0
    beq @L_800B5A68
    @L_800B59B8:
    li r25, 0x0
    @L_800B59BC:
    cmpw r25, r28
    beq @L_800B5A28
    lbz r0, 0x0(r26)
    cmplwi r0, 0xff
    beq @L_800B5A28
    addi r3, r26, 0x0
    addi r4, r29, 0x0
    li r5, 0x4
    bl fn_800C80D0
    cmpwi r3, 0x0
    bne @L_800B5A28
    addi r3, r26, 0x4
    addi r4, r29, 0x4
    li r5, 0x2
    bl fn_800C80D0
    cmpwi r3, 0x0
    bne @L_800B5A28
    addi r3, r26, 0x8
    addi r4, r29, 0x8
    li r5, 0x20
    bl fn_800C80D0
    cmpwi r3, 0x0
    bne @L_800B5A28
    lwz r3, 0x18(r1)
    li r4, -0x7
    bl fn_800B02F4
    b @L_800B5ACC
    @L_800B5A28:
    addi r25, r25, 0x1
    cmpwi r25, 0x7f
    addi r26, r26, 0x40
    blt @L_800B59BC
    addi r3, r31, 0x8
    addi r4, r29, 0x8
    li r5, 0x20
    bl memcpy
    addi r3, r31, 0x0
    addi r4, r29, 0x0
    li r5, 0x4
    bl memcpy
    addi r3, r31, 0x4
    addi r4, r29, 0x4
    li r5, 0x2
    bl memcpy
    @L_800B5A68:
    lwz r0, 0x28(r29)
    addi r3, r27, 0x0
    addi r4, r30, 0x0
    stw r0, 0x28(r31)
    lbz r0, 0x7(r29)
    stb r0, 0x7(r31)
    lwz r0, 0x2c(r29)
    stw r0, 0x2c(r31)
    lhz r0, 0x30(r29)
    sth r0, 0x30(r31)
    lhz r0, 0x32(r29)
    sth r0, 0x32(r31)
    lwz r0, 0x3c(r29)
    stw r0, 0x3c(r31)
    lbz r0, 0x34(r29)
    stb r0, 0x34(r31)
    lbz r0, 0x35(r29)
    stb r0, 0x35(r31)
    bl fn_800B1FAC
    mr. r26, r3
    bge @L_800B5AC8
    lwz r3, 0x18(r1)
    mr r4, r26
    bl fn_800B02F4
    @L_800B5AC8:
    mr r3, r26
    @L_800B5ACC:
    lmw r25, 0x24(r1)
    lwz r0, 0x44(r1)
    addi r1, r1, 0x40
    mtlr r0
    blr
