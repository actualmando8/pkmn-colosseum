    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r25, r3
    mr r26, r4
    mr r27, r5
    mr r28, r6
    mr r29, r7
    mr r30, r8
    li r31, 0x0
    bl fn_801046B8
    li r0, 0x0
    mr r4, r3
    stw r0, 0x8(r1)
    mr r9, r25
    clrlwi r10, r27, 24
    li r3, 0x2
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x3
    crxor 6, 6, 6
    bl fn_801026A4
    mr r4, r28
    mr r5, r29
    li r3, 0x2
    bl fn_80102868
    li r3, 0x2
    li r4, 0x1
    bl fn_801045A8
    li r3, 0x2
    bl fn_801043A4
    li r3, 0x2
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_8001E2EC
    cmplwi r26, 0x0
    beq @L_8001E2C8
    lwz r0, 0x80(r3)
    stw r0, 0x0(r26)
    @L_8001E2C8:
    lbz r0, 0x99(r3)
    cmplwi r0, 0x0
    bne @L_8001E2D8
    li r31, 0x1
    @L_8001E2D8:
    clrlwi r0, r30, 24
    cmplwi r0, 0x0
    beq @L_8001E2EC
    li r3, 0x2
    bl fn_80102510
    @L_8001E2EC:
    mr r3, r31
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
