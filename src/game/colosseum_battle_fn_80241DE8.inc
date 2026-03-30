    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r23, 0x2c(r1)
    mr r26, r3
    mr r27, r4
    mr r28, r5
    mr r23, r6
    li r29, 0x0
    bl fn_80236520
    mr r30, r3
    mr r3, r26
    mr r4, r27
    bl fn_802364BC
    mr r31, r3
    mr r3, r26
    mr r4, r23
    bl fn_80236520
    mr r23, r3
    mr r4, r26
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801F1C18
    clrlwi. r0, r23, 16
    mr r25, r3
    beq @L_80241ED4
    cmplwi r0, 0xffff
    beq @L_80241ED4
    cmplwi r0, 0x165
    beq @L_80241ED4
    cmplwi r0, 0x163
    beq @L_80241ED4
    mr r3, r26
    mr r4, r23
    li r5, 0x4
    bl fn_8023943C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80241ED4
    mr r4, r26
    li r3, 0x0
    li r5, 0x1a9
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1a9
    bl fn_80239EE8
@L_80241ED4:
    addi r24, r1, 0x8
    clrlwi r25, r25, 16
    li r23, 0x0
    b @L_80241F60
@L_80241EE4:
    clrlslwi r0, r23, 16, 2
    mr r3, r26
    lwzx r4, r24, r0
    bl fn_802377E8
    clrlwi r0, r3, 16
    cmplwi r0, 0x12e
    beq @L_80241F10
    cmplwi r0, 0xd4
    beq @L_80241F10
    cmplwi r0, 0x177
    bne @L_80241F5C
@L_80241F10:
    mr r3, r29
    mr r4, r26
    li r5, 0x1aa
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1aa
    bl fn_80239EE8
    b @L_80241F6C
@L_80241F5C:
    addi r23, r23, 0x1
@L_80241F60:
    clrlwi r0, r23, 16
    cmplw r0, r25
    blt @L_80241EE4
@L_80241F6C:
    clrlwi r0, r30, 16
    cmplwi r0, 0x121
    beq @L_80241F84
    clrlwi r0, r31, 16
    cmplwi r0, 0x121
    bne @L_80241FCC
@L_80241F84:
    mr r3, r29
    mr r4, r26
    li r5, 0x1ab
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1ab
    bl fn_80239EE8
@L_80241FCC:
    mr r3, r29
    lmw r23, 0x2c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
