    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stmw r25, 0x44(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    li r31, 0x0
    bl fn_80235714
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023D580
    mr r4, r28
    li r3, 0x0
    li r5, 0x210
    bl fn_80239984
    mr r31, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x210
    bl fn_80239EE8
@L_8023D580:
    mr r4, r28
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x1
    li r7, 0x1
    bl fn_801F1C18
    addi r26, r1, 0x8
    rlwinm r27, r3, 0, 16, 31
    li r25, 0x0
    b @L_8023D60C
@L_8023D5A8:
    clrlslwi r0, r25, 16, 2
    lwzx r4, r26, r0
    cmplw r29, r4
    beq @L_8023D608
    mr r3, r28
    addi r5, r1, 0x28
    li r6, 0x0
    li r7, 0x1
    bl fn_802367CC
    rlwinm. r4, r3, 0, 16, 31
    beq @L_8023D608
    addi r3, r1, 0x28
    li r5, 0x0
    b @L_8023D5FC
@L_8023D5E0:
    clrlslwi r0, r5, 16, 1
    lhzx r0, r3, r0
    cmplwi r0, 0x10a
    bne @L_8023D5F8
    li r0, 0x1
    b @L_8023D61C
@L_8023D5F8:
    addi r5, r5, 0x1
@L_8023D5FC:
    rlwinm r0, r5, 0, 16, 31
    cmplw r0, r4
    blt @L_8023D5E0
@L_8023D608:
    addi r25, r25, 0x1
@L_8023D60C:
    rlwinm r0, r25, 0, 16, 31
    cmplw r0, r27
    blt @L_8023D5A8
    li r0, 0x0
@L_8023D61C:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023D66C
    mr r3, r31
    mr r4, r28
    li r5, 0x211
    bl fn_80239984
    mr r31, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x211
    bl fn_80239EE8
@L_8023D66C:
    mr r3, r28
    mr r4, r29
    bl fn_80235714
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023D6C8
    mr r3, r31
    mr r4, r28
    li r5, 0x212
    bl fn_80239984
    mr r31, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x212
    bl fn_80239EE8
@L_8023D6C8:
    lfs f1, -20592(r2)
    mr r25, r31
    mr r3, r28
    mr r4, r29
    li r5, -0x1
    bl fn_802373B0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023D734
    mr r3, r31
    mr r4, r28
    li r5, 0x213
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r25, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x213
    bl fn_80239EE8
@L_8023D734:
    mr r3, r25
    lmw r25, 0x44(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
