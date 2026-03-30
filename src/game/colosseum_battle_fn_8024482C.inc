    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    li r6, 0x1
    li r7, 0x1
    stw r0, 0x94(r1)
    stmw r25, 0x74(r1)
    mr r25, r3
    mr r26, r4
    mr r27, r5
    addi r5, r1, 0x8
    mr r4, r25
    li r29, 0x0
    li r3, 0x0
    bl fn_801F1A6C
    addi r30, r1, 0x8
    clrlwi r31, r3, 16
    li r28, 0x0
    b @L_802448DC
@L_80244874:
    clrlslwi r0, r28, 16, 2
    mr r3, r25
    lwzx r4, r30, r0
    bl fn_80238748
    clrlwi. r0, r3, 24
    bne @L_802448D8
    mr r4, r25
    li r3, 0x0
    li r5, 0x161
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r25
    mr r8, r27
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x161
    bl fn_80239EE8
    b @L_802448E8
@L_802448D8:
    addi r28, r28, 0x1
@L_802448DC:
    clrlwi r0, r28, 16
    cmplw r0, r31
    blt @L_80244874
@L_802448E8:
    mr r3, r29
    lmw r25, 0x74(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
