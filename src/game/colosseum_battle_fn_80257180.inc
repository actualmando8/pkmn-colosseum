    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r31, r3
    mr r27, r6
    mr r25, r4
    mr r26, r5
    li r3, 0x2
    mr r4, r27
    bl fn_801F025C
    mr r0, r3
    mr r3, r31
    mr r28, r0
    mr r4, r27
    bl fn_80235AA0
    mr r29, r3
    mr r3, r31
    mr r4, r27
    li r5, 0x14
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802571E8
    li r3, 0x0
    b @L_802573A8
@L_802571E8:
    mr r3, r31
    mr r4, r25
    mr r5, r26
    mr r6, r27
    li r7, 0x0
    bl fn_8025C264
    mr r30, r3
    mr r3, r31
    mr r4, r27
    li r5, 0x9
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025725C
    clrlwi r0, r29, 24
    cmplwi r0, 0xc
    blt @L_8025725C
    mr r3, r31
    mr r4, r25
    mr r5, r27
    mr r6, r26
    li r7, 0x20
    li r8, 0x1
    li r9, 0x1
    bl fn_8025C808
    clrlwi. r0, r3, 24
    bne @L_8025725C
    li r3, 0x0
    b @L_802573A8
@L_8025725C:
    mr r3, r31
    mr r4, r27
    li r5, 0x14
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80257280
    li r3, 0x0
    b @L_802573A8
@L_80257280:
    mr r3, r31
    mr r4, r27
    li r5, 0x11
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8025735C
    mr r3, r31
    mr r4, r27
    li r5, 0x14
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802572C0
    li r0, 0x0
    b @L_80257360
@L_802572C0:
    mr r3, r31
    mr r4, r27
    li r5, 0x7
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8025735C
    mr r3, r31
    mr r4, r27
    li r5, 0xf
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8025735C
    mr r3, r31
    mr r4, r27
    li r5, 0x48
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8025735C
    mr r3, r31
    mr r4, r27
    li r5, 0x29
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8025735C
    mr r3, r31
    mr r4, r27
    li r5, 0x28
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8025735C
    mr r3, r31
    mr r4, r27
    li r5, 0xc
    bl fn_80237F74
@L_8025735C:
    li r0, 0x1
@L_80257360:
    clrlwi. r0, r0, 24
    bne @L_80257370
    li r3, 0x0
    b @L_802573A8
@L_80257370:
    mr r3, r28
    li r4, 0x4b
    bl fn_801F6E98
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80257390
    li r3, 0x0
    b @L_802573A8
@L_80257390:
    cmpwi r30, 0x0
    bne @L_802573A0
    li r3, 0x0
    b @L_802573A8
@L_802573A0:
    cmpwi r30, -0x1
    li r3, 0x1
@L_802573A8:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
