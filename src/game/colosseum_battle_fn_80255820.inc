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
    bl fn_80235910
    mr r29, r3
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
    bne @L_802558D8
    clrlwi r0, r29, 24
    cmplwi r0, 0xc
    blt @L_802558D8
    mr r3, r31
    mr r4, r25
    mr r5, r27
    mr r6, r26
    li r7, 0x10
    li r8, 0x4
    li r9, 0x1
    bl fn_8025C808
    clrlwi. r0, r3, 24
    bne @L_802558D8
    li r3, 0x0
    b @L_80255A24
@L_802558D8:
    mr r3, r31
    mr r4, r27
    li r5, 0x14
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802558FC
    li r3, 0x0
    b @L_80255A24
@L_802558FC:
    mr r3, r31
    mr r4, r27
    li r5, 0x11
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802559D8
    mr r3, r31
    mr r4, r27
    li r5, 0x14
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025593C
    li r0, 0x0
    b @L_802559DC
@L_8025593C:
    mr r3, r31
    mr r4, r27
    li r5, 0x7
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802559D8
    mr r3, r31
    mr r4, r27
    li r5, 0xf
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802559D8
    mr r3, r31
    mr r4, r27
    li r5, 0x48
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802559D8
    mr r3, r31
    mr r4, r27
    li r5, 0x29
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802559D8
    mr r3, r31
    mr r4, r27
    li r5, 0x28
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802559D8
    mr r3, r31
    mr r4, r27
    li r5, 0xc
    bl fn_80237F74
@L_802559D8:
    li r0, 0x1
@L_802559DC:
    clrlwi. r0, r0, 24
    bne @L_802559EC
    li r3, 0x0
    b @L_80255A24
@L_802559EC:
    mr r3, r28
    li r4, 0x4b
    bl fn_801F6E98
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80255A0C
    li r3, 0x0
    b @L_80255A24
@L_80255A0C:
    cmpwi r30, 0x0
    bne @L_80255A1C
    li r3, 0x0
    b @L_80255A24
@L_80255A1C:
    cmpwi r30, -0x1
    li r3, 0x1
@L_80255A24:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
