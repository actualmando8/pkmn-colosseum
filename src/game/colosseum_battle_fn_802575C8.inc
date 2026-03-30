    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r28, r6
    mr r26, r4
    mr r25, r3
    mr r27, r5
    mr r4, r28
    bl fn_80235AA0
    mr r29, r3
    mr r3, r25
    mr r4, r28
    bl fn_80235A3C
    mr r30, r3
    mr r3, r25
    mr r4, r28
    bl fn_80235910
    mr r31, r3
    mr r3, r25
    mr r4, r26
    li r5, 0x7
    bl fn_80237DBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80257690
    mr r3, r25
    mr r4, r28
    li r5, 0x14
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80257654
    li r3, 0x0
    b @L_8025773C
@L_80257654:
    mr r3, r25
    mr r4, r28
    li r5, 0x18
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80257688
    mr r3, r25
    mr r4, r28
    bl fn_80237288
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80257738
@L_80257688:
    li r3, 0x0
    b @L_8025773C
@L_80257690:
    clrlwi. r0, r31, 24
    bne @L_802576B8
    clrlwi r0, r29, 24
    cmplwi r0, 0xc
    blt @L_802576B8
    clrlwi r0, r30, 24
    cmplwi r0, 0xc
    blt @L_802576B8
    li r3, 0x0
    b @L_8025773C
@L_802576B8:
    mr r3, r25
    mr r4, r26
    mr r5, r28
    mr r6, r27
    li r7, 0x90
    li r8, 0x3
    li r9, 0x41
    bl fn_8025C808
    clrlwi. r0, r3, 24
    bne @L_80257738
    mr r3, r25
    mr r4, r26
    mr r5, r28
    mr r6, r27
    li r7, 0x10
    li r8, 0x1
    li r9, 0x41
    bl fn_8025C808
    clrlwi. r0, r3, 24
    bne @L_80257738
    mr r3, r25
    mr r4, r26
    mr r5, r28
    mr r6, r27
    li r7, 0x10
    li r8, 0x2
    li r9, 0x41
    bl fn_8025C808
    clrlwi. r0, r3, 24
    bne @L_80257738
    li r3, 0x0
    b @L_8025773C
@L_80257738:
    li r3, 0x1
@L_8025773C:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
