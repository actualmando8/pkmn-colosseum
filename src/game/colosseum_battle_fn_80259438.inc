    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r30, r5
    mr r31, r6
    mr r29, r4
    mr r28, r3
    li r5, 0x11
    mr r4, r31
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80259478
    li r3, 0x0
    b @L_80259694
@L_80259478:
    mr r3, r28
    mr r4, r31
    li r5, 0x14
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025949C
    li r3, 0x0
    b @L_80259694
@L_8025949C:
    mr r3, r28
    mr r4, r31
    li r5, 0x3
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802594C0
    li r3, 0x0
    b @L_80259694
@L_802594C0:
    mr r3, r28
    mr r4, r31
    li r5, 0x4
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802594E4
    li r3, 0x0
    b @L_80259694
@L_802594E4:
    mr r3, r28
    mr r4, r31
    li r5, 0x3
    bl fn_80237DBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80259508
    li r3, 0x0
    b @L_80259694
@L_80259508:
    mr r3, r28
    mr r4, r31
    li r5, 0x8
    bl fn_80237DBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025952C
    li r3, 0x0
    b @L_80259694
@L_8025952C:
    mr r3, r28
    mr r4, r31
    bl fn_80237310
    clrlwi. r0, r3, 24
    bne @L_80259548
    li r3, 0x0
    b @L_80259694
@L_80259548:
    mr r3, r28
    mr r4, r31
    li r5, 0x11
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025956C
    li r0, 0x0
    b @L_80259628
@L_8025956C:
    mr r3, r28
    mr r4, r31
    li r5, 0x14
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80259624
    mr r3, r28
    mr r4, r31
    li r5, 0x7
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80259624
    mr r3, r28
    mr r4, r31
    li r5, 0xf
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80259624
    mr r3, r28
    mr r4, r31
    li r5, 0x48
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80259624
    mr r3, r28
    mr r4, r31
    li r5, 0x29
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80259624
    mr r3, r28
    mr r4, r31
    li r5, 0x28
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80259624
    mr r3, r28
    mr r4, r31
    li r5, 0xc
    bl fn_80237F74
@L_80259624:
    li r0, 0x1
@L_80259628:
    clrlwi. r0, r0, 24
    bne @L_80259638
    li r3, 0x0
    b @L_80259694
@L_80259638:
    mr r3, r28
    mr r4, r29
    mr r5, r30
    mr r6, r31
    li r7, 0x0
    bl fn_8025C264
    mr r28, r3
    mr r4, r31
    li r3, 0x2
    bl fn_801F025C
    li r4, 0x4b
    bl fn_801F6E98
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025967C
    li r3, 0x0
    b @L_80259694
@L_8025967C:
    cmpwi r28, 0x0
    bne @L_8025968C
    li r3, 0x0
    b @L_80259694
@L_8025968C:
    cmpwi r28, -0x1
    li r3, 0x1
@L_80259694:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
