    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    mr r26, r3
    mr r27, r4
    mr r28, r5
    lbz r29, 0x16(r26)
    @L_80011A3C:
    lbz r0, 0x17(r26)
    cmplwi r0, 0x0
    beq @L_80011A70
    mr r5, r27
    mr r7, r28
    mr r9, r26
    li r3, 0xf6
    li r4, 0x0
    li r6, 0x0
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    b @L_80011A94
    @L_80011A70:
    mr r5, r27
    mr r7, r28
    mr r9, r26
    li r3, 0x4b
    li r4, 0x0
    li r6, 0x0
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    @L_80011A94:
    cmplwi r29, 0x0
    mr r30, r3
    bne @L_80011B34
    cmpwi r3, 0x3
    bne @L_80011B34
    li r0, 0x0
    li r3, 0x4b
    stw r0, 0x8(r1)
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80011AD4
    li r3, 0x4b
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    @L_80011AD4:
    li r3, 0xf6
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80011AF8
    li r3, 0xf6
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    @L_80011AF8:
    addi r5, r1, 0x8
    li r3, 0xfb
    li r4, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    mr r31, r3
    li r3, 0xfb
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    cmpwi r31, 0x1207
    bne @L_80011A3C
    @L_80011B34:
    mr r3, r30
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
