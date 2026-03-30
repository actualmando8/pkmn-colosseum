    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r26, r4
    mr r27, r3
    mr r29, r5
    mr r28, r6
    mr r3, r26
    bl fn_802016A4
    mr r30, r3
    mr r3, r28
    bl fn_802016A4
    mr r31, r3
    mr r3, r27
    mr r4, r26
    mr r5, r29
    mr r6, r28
    li r7, 0x0
    bl fn_8025C264
    mr r0, r3
    mr r3, r27
    mr r29, r0
    mr r4, r28
    li r5, 0xc
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80256F48
    li r29, 0x0
@L_80256F48:
    mr r3, r27
    mr r4, r28
    li r5, 0x11
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80257030
    mr r3, r27
    mr r4, r28
    li r5, 0x14
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80257030
    mr r3, r27
    mr r4, r28
    li r5, 0x7
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80257030
    mr r3, r27
    mr r4, r28
    li r5, 0xf
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80257030
    mr r3, r27
    mr r4, r28
    li r5, 0x48
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80257030
    mr r3, r27
    mr r4, r28
    li r5, 0x29
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80257030
    mr r3, r27
    mr r4, r28
    li r5, 0x28
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80257030
    mr r3, r27
    mr r4, r28
    li r5, 0xc
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80257030
    li r0, 0x0
    b @L_80257034
@L_80257030:
    li r0, 0x1
@L_80257034:
    clrlwi. r0, r0, 24
    bne @L_80257044
    li r3, 0x0
    b @L_802570BC
@L_80257044:
    clrlwi r3, r30, 24
    clrlwi r0, r31, 24
    cmplw r3, r0
    beq @L_802570A0
    mr r3, r27
    mr r4, r28
    bl fn_80237288
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802570A0
    mr r3, r27
    mr r4, r28
    li r5, 0xa
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802570A0
    clrlwi r0, r30, 24
    cmplwi r0, 0x2
    beq @L_802570A0
    clrlwi r0, r31, 24
    cmplwi r0, 0x2
    bne @L_802570A4
@L_802570A0:
    li r29, 0x0
@L_802570A4:
    cmpwi r29, 0x0
    bne @L_802570B4
    li r3, 0x0
    b @L_802570BC
@L_802570B4:
    cmpwi r29, -0x1
    li r3, 0x1
@L_802570BC:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
