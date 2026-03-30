    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    li r6, 0x0
    li r7, 0x0
    stw r0, 0x74(r1)
    addi r5, r1, 0x1c
    stmw r27, 0x5c(r1)
    mr r29, r3
    mr r27, r4
    bl fn_802367CC
    mr r31, r3
    mr r4, r29
    addi r5, r1, 0x30
    li r3, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801F1C18
    mr r28, r3
    mr r3, r29
    mr r4, r27
    li r5, 0x27
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80254754
    li r3, 0x0
    b @L_802547FC
@L_80254754:
    addi r27, r1, 0x30
    clrlwi r28, r28, 16
    li r30, 0x0
    b @L_802547EC
@L_80254764:
    clrlslwi r0, r30, 16, 2
    lwzx r4, r27, r0
    cmplwi r4, 0x0
    beq @L_802547E8
    mr r3, r29
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    bl fn_802367CC
    addi r5, r1, 0x1c
    clrlwi r4, r31, 16
    addi r8, r1, 0x8
    clrlwi r0, r3, 16
    li r10, 0x0
    b @L_802547DC
@L_802547A0:
    clrlslwi r7, r10, 16, 1
    li r9, 0x0
    b @L_802547CC
@L_802547AC:
    clrlslwi r3, r9, 16, 1
    lhzx r6, r8, r7
    lhzx r3, r5, r3
    cmplw r6, r3
    bne @L_802547C8
    li r3, 0x1
    b @L_802547FC
@L_802547C8:
    addi r9, r9, 0x1
@L_802547CC:
    clrlwi r3, r9, 16
    cmplw r3, r4
    blt @L_802547AC
    addi r10, r10, 0x1
@L_802547DC:
    clrlwi r3, r10, 16
    cmplw r3, r0
    blt @L_802547A0
@L_802547E8:
    addi r30, r30, 0x1
@L_802547EC:
    clrlwi r0, r30, 16
    cmplw r0, r28
    blt @L_80254764
    li r3, 0x0
@L_802547FC:
    lmw r27, 0x5c(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
