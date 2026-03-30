    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r27, r3
    mr r30, r6
    mr r28, r4
    mr r29, r5
    li r3, 0x2
    mr r4, r30
    bl fn_801F025C
    mr r0, r3
    mr r3, r27
    mr r31, r0
    mr r4, r29
    mr r5, r28
    bl fn_802395C8
    mr r0, r3
    mr r3, r27
    mr r26, r0
    mr r4, r30
    li r5, 0x7
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80259248
    li r3, 0x0
    b @L_80259424
@L_80259248:
    mr r3, r27
    mr r4, r30
    li r5, 0x11
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80259324
    mr r3, r27
    mr r4, r30
    li r5, 0x14
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80259324
    mr r3, r27
    mr r4, r30
    li r5, 0x7
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802592A4
    li r0, 0x0
    b @L_80259328
@L_802592A4:
    mr r3, r27
    mr r4, r30
    li r5, 0xf
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80259324
    mr r3, r27
    mr r4, r30
    li r5, 0x48
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80259324
    mr r3, r27
    mr r4, r30
    li r5, 0x29
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80259324
    mr r3, r27
    mr r4, r30
    li r5, 0x28
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80259324
    mr r3, r27
    mr r4, r30
    li r5, 0xc
    bl fn_80237F74
@L_80259324:
    li r0, 0x1
@L_80259328:
    clrlwi. r0, r0, 24
    bne @L_80259338
    li r3, 0x0
    b @L_80259424
@L_80259338:
    mr r3, r27
    mr r4, r30
    li r5, 0x14
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025935C
    li r3, 0x0
    b @L_80259424
@L_8025935C:
    mr r3, r27
    mr r4, r29
    bl fn_80239500
    mr r6, r3
    mr r3, r27
    mr r4, r30
    mr r5, r26
    bl fn_8023793C
    clrlwi r0, r3, 16
    cmplwi r0, 0x43
    bne @L_80259390
    li r3, 0x0
    b @L_80259424
@L_80259390:
    mr r3, r27
    mr r4, r30
    li r5, 0x5
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802593B4
    li r3, 0x0
    b @L_80259424
@L_802593B4:
    mr r3, r27
    mr r4, r30
    bl fn_80237310
    clrlwi. r0, r3, 24
    bne @L_802593D0
    li r3, 0x0
    b @L_80259424
@L_802593D0:
    mr r3, r27
    mr r4, r28
    mr r5, r29
    mr r6, r30
    li r7, 0x0
    bl fn_8025C264
    mr r27, r3
    mr r3, r31
    li r4, 0x4b
    bl fn_801F6E98
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025940C
    li r3, 0x0
    b @L_80259424
@L_8025940C:
    cmpwi r27, 0x0
    bne @L_8025941C
    li r3, 0x0
    b @L_80259424
@L_8025941C:
    cmpwi r27, -0x1
    li r3, 0x1
@L_80259424:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
