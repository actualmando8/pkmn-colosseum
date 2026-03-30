    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr. r26, r3
    mr r27, r5
    bne @L_801F821C
    li r3, 0x0
    b @L_801F8410
    @L_801F821C:
    clrlwi r25, r4, 16
    li r29, 0x0
    li r28, 0x0
    li r30, 0x0
    b @L_801F83D8
    @L_801F8230:
    mr r3, r26
    mr r6, r30
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    mr r31, r3
    bl fn_80206A04
    clrlwi. r0, r3, 24
    bne @L_801F8258
    li r31, 0x0
    @L_801F8258:
    cmplwi r31, 0x0
    beq @L_801F83D4
    mr r3, r31
    bl fn_80206608
    clrlwi. r0, r3, 24
    bne @L_801F8278
    li r0, 0x1
    b @L_801F83B4
    @L_801F8278:
    cmplwi r26, 0x0
    bne @L_801F8288
    li r23, 0x0
    b @L_801F8300
    @L_801F8288:
    cmplwi r31, 0x0
    bne @L_801F8298
    li r23, 0x0
    b @L_801F8300
    @L_801F8298:
    li r24, 0x0
    b @L_801F82F0
    @L_801F82A0:
    mr r3, r26
    mr r6, r24
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr r23, r3
    bl fn_80206780
    clrlwi. r0, r3, 24
    bne @L_801F82C8
    li r23, 0x0
    @L_801F82C8:
    cmplwi r23, 0x0
    beq @L_801F82EC
    mr r3, r31
    mr r4, r23
    bl fn_80204928
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F82EC
    b @L_801F8300
    @L_801F82EC:
    addi r24, r24, 0x1
    @L_801F82F0:
    clrlwi r0, r24, 16
    cmplwi r0, 0x2
    blt @L_801F82A0
    li r23, 0x0
    @L_801F8300:
    cmplwi r23, 0x0
    beq @L_801F8310
    li r0, 0x2
    b @L_801F83B4
    @L_801F8310:
    cmplwi r26, 0x0
    bne @L_801F8320
    li r0, 0x0
    b @L_801F839C
    @L_801F8320:
    cmplwi r31, 0x0
    bne @L_801F8330
    li r0, 0x0
    b @L_801F839C
    @L_801F8330:
    li r24, 0x0
    b @L_801F838C
    @L_801F8338:
    mr r3, r26
    mr r6, r24
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr r23, r3
    bl fn_80206780
    clrlwi. r0, r3, 24
    bne @L_801F8360
    li r23, 0x0
    @L_801F8360:
    cmplwi r23, 0x0
    beq @L_801F8388
    mr r3, r23
    mr r4, r31
    bl fn_80204854
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F8388
    li r0, 0x1
    b @L_801F839C
    @L_801F8388:
    addi r24, r24, 0x1
    @L_801F838C:
    clrlwi r0, r24, 16
    cmplwi r0, 0x2
    blt @L_801F8338
    li r0, 0x0
    @L_801F839C:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_801F83B0
    li r0, 0x3
    b @L_801F83B4
    @L_801F83B0:
    li r0, 0x0
    @L_801F83B4:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    beq @L_801F83D4
    cmplwi r0, 0x2
    bne @L_801F83D0
    addi r28, r28, 0x1
    b @L_801F83D4
    @L_801F83D0:
    addi r29, r29, 0x1
    @L_801F83D4:
    addi r30, r30, 0x1
    @L_801F83D8:
    clrlwi r0, r30, 16
    cmplw r0, r25
    blt @L_801F8230
    clrlwi r3, r27, 16
    @L_801F83E8:
    clrlwi r0, r28, 16
    cmplw r0, r3
    bge @L_801F840C
    clrlwi r0, r29, 16
    cmplwi r0, 0x1
    blt @L_801F840C
    subi r29, r29, 0x1
    addi r28, r28, 0x1
    b @L_801F83E8
    @L_801F840C:
    mr r3, r29
    @L_801F8410:
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
