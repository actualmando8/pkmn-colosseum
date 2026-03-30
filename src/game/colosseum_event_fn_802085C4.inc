    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stmw r25, 0x44(r1)
    mr r25, r3
    mr r26, r4
    mr r27, r5
    mr r28, r6
    mr r29, r7
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r30, r3, 0, 16, 31
    mr r3, r25
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r31, r3
    beq @L_8020869C
    mr r3, r25
    addi r4, r1, 0x8
    bl fn_801FE168
    cmpwi r29, 0x0
    blt @L_80208664
    mr r3, r25
    mr r4, r30
    li r5, 0x1
    bl fn_802656AC
    addi r9, r1, 0x8
    mr r29, r3
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
@L_80208664:
    mr r3, r31
    mr r4, r26
    mr r5, r27
    mr r6, r28
    bl fn_802094CC
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020869C
    cmpwi r29, 0x0
    blt @L_8020869C
    mr r3, r29
    li r4, 0x0
    li r5, 0x0
    bl fn_80102568
@L_8020869C:
    lmw r25, 0x44(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
