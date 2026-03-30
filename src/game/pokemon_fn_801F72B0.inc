    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr. r30, r6
    mr r28, r4
    mr r29, r5
    mr r31, r7
    beq @L_801F7374
    cmplwi r31, 0x0
    beq @L_801F7374
    bl fn_801F7870
    cmplwi r3, 0x0
    bne @L_801F72F0
    li r3, 0x0
    b @L_801F72F8
    @L_801F72F0:
    bl fn_801F7858
    clrlwi r3, r3, 16
    @L_801F72F8:
    clrlwi r0, r28, 16
    stb r3, 0x0(r30)
    li r3, 0x0
    cmplwi r0, 0x1
    stb r3, 0x0(r31)
    beq @L_801F7374
    cmplwi r0, 0x2
    bne @L_801F7374
    lbz r0, 0x0(r30)
    cmplwi r0, 0x1
    bne @L_801F7350
    clrlwi. r0, r29, 16
    bne @L_801F7338
    li r0, 0x1
    stb r0, 0x0(r31)
    b @L_801F7374
    @L_801F7338:
    cmplwi r0, 0x1
    bne @L_801F7374
    li r0, -0x1
    stb r0, 0x0(r31)
    b @L_801F7374
    b @L_801F7374
    @L_801F7350:
    clrlwi. r0, r29, 16
    bne @L_801F7364
    li r0, -0x1
    stb r0, 0x0(r31)
    b @L_801F7374
    @L_801F7364:
    cmplwi r0, 0x1
    bne @L_801F7374
    li r0, 0x1
    stb r0, 0x0(r31)
    @L_801F7374:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
