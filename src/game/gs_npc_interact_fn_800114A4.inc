    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stmw r23, 0x3c(r1)
    mr r24, r3
    mr r25, r4
    mr r26, r5
    mr r27, r6
    mr r28, r7
    mr r3, r26
    bl fn_8020E204
    bl fn_8020E1A4
    clrlwi r0, r28, 24
    mr r31, r3
    cmplwi r0, 0x0
    bne @L_800114FC
    mr r4, r27
    mr r5, r24
    mr r6, r25
    bl fn_8001BD80
    mr r29, r3
    b @L_800116E8
    @L_800114FC:
    clrlwi r30, r27, 24
    @L_80011500:
    li r3, 0xf8
    li r4, 0x1e
    bl fn_801080CC
    stw r30, 0x8(r1)
    mr r9, r24
    mr r10, r25
    li r3, 0xf8
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x3
    crxor 6, 6, 6
    bl fn_801026A4
    mr r0, r3
    li r3, 0xf8
    mr r29, r0
    li r4, 0x20
    bl fn_801080CC
    cmpwi r29, -0x1
    bne @L_8001156C
    li r3, 0xf8
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, -0x1
    b @L_800116EC
    @L_8001156C:
    mr r3, r25
    mr r4, r24
    mr r5, r29
    bl fn_80010C98
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80011500
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800116D8
    clrlwi r0, r31, 24
    cmplwi r0, 0x2
    blt @L_800116D8
    mr r4, r25
    mr r5, r26
    li r3, 0xf
    bl fn_801F02AC
    mr r23, r3
    bl fn_802062FC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_800115D8
    mr r3, r23
    mr r4, r26
    li r5, 0x0
    bl fn_802656AC
    b @L_800115DC
    @L_800115D8:
    li r3, 0x0
    @L_800115DC:
    stw r3, 0x14(r1)
    mr r4, r25
    mr r5, r26
    li r3, 0x10
    bl fn_801F02AC
    mr r23, r3
    bl fn_802062FC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80011618
    mr r3, r23
    mr r4, r26
    li r5, 0x0
    bl fn_802656AC
    b @L_8001161C
    @L_80011618:
    li r3, 0x0
    @L_8001161C:
    stw r3, 0x1c(r1)
    mr r4, r25
    mr r5, r26
    li r3, 0xe
    bl fn_801F02AC
    mr r23, r3
    bl fn_802062FC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80011658
    mr r3, r23
    mr r4, r26
    li r5, 0x0
    bl fn_802656AC
    b @L_8001165C
    @L_80011658:
    li r3, 0x0
    @L_8001165C:
    li r0, 0x0
    stw r3, 0x24(r1)
    addi r3, r1, 0x10
    li r4, 0x0
    stw r0, 0x2c(r1)
    li r5, 0x1
    stb r28, 0x31(r1)
    bl fn_80011288
    mr r23, r3
    li r3, 0xff
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800116A4
    li r3, 0xff
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    @L_800116A4:
    li r3, 0x104
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800116C8
    li r3, 0x104
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    @L_800116C8:
    li r3, 0x100
    bl fn_80102620
    cmpwi r23, -0x1
    beq @L_80011500
    @L_800116D8:
    li r3, 0xf8
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    @L_800116E8:
    mr r3, r29
    @L_800116EC:
    lmw r23, 0x3c(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
