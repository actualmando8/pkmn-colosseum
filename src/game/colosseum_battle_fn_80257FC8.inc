    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    li r7, 0x0
    stw r0, 0x44(r1)
    addi r5, r1, 0x8
    stmw r27, 0x2c(r1)
    mr r29, r6
    mr r27, r3
    mr r28, r4
    li r6, 0x0
    bl fn_802367CC
    mr r31, r3
    mr r3, r27
    mr r4, r29
    bl fn_80236458
    mr r0, r3
    mr r3, r27
    mr r30, r0
    mr r4, r29
    li r5, 0x14
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80258030
    li r3, 0x0
    b @L_802580A8
@L_80258030:
    mr r3, r27
    mr r4, r28
    li r5, 0x10
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80258060
    mr r3, r30
    bl fn_80219270
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80258068
@L_80258060:
    li r3, 0x0
    b @L_802580A8
@L_80258068:
    clrlwi r5, r30, 16
    addi r4, r1, 0x8
    clrlwi r0, r31, 16
    li r6, 0x0
    b @L_80258098
@L_8025807C:
    clrlslwi r3, r6, 16, 1
    lhzx r3, r4, r3
    cmplw r5, r3
    bne @L_80258094
    li r3, 0x0
    b @L_802580A8
@L_80258094:
    addi r6, r6, 0x1
@L_80258098:
    clrlwi r3, r6, 16
    cmplw r3, r0
    blt @L_8025807C
    li r3, 0x1
@L_802580A8:
    lmw r27, 0x2c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
