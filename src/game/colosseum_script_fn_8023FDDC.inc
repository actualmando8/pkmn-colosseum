    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    li r7, 0x1
    stw r0, 0x74(r1)
    stmw r21, 0x44(r1)
    mr r23, r3
    mr r24, r4
    mr r25, r5
    mr r26, r6
    mr r4, r23
    addi r5, r1, 0x1c
    li r28, 0x0
    li r3, 0x0
    li r6, 0x1
    bl fn_801F1C18
    mr r31, r3
    addi r30, r1, 0x1c
    rlwinm r22, r3, 0, 16, 31
    li r29, 0x0
    b @L_8023FF10
@L_8023FE2C:
    clrlslwi r0, r29, 16, 2
    lwzx r21, r30, r0
    cmplw r24, r21
    beq @L_8023FF0C
    mr r3, r23
    mr r4, r21
    li r5, 0x8
    bl fn_80236BFC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023FE5C
    li r0, -0x1
    b @L_8023FEB8
@L_8023FE5C:
    mr r3, r23
    mr r4, r21
    li r5, 0x30
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    mr r3, r21
    subfic r0, r0, 0x1
    li r4, 0x8
    cntlzw r0, r0
    rlwinm r5, r0, 27, 5, 31
    addi r27, r5, 0x1
    bl fn_80202108
    add r27, r3, r27
    mr r3, r21
    li r4, 0x8
    bl fn_80202234
    extsb r3, r3
    extsb r0, r27
    cmpw r0, r3
    blt @L_8023FEB4
    li r0, 0x1
    b @L_8023FEB8
@L_8023FEB4:
    li r0, 0x0
@L_8023FEB8:
    extsb. r0, r0
    bne @L_8023FF0C
    mr r4, r23
    li r3, 0x0
    li r5, 0x1cc
    bl fn_80239984
    mr r0, r3
    mr r3, r24
    mr r28, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r23
    mr r8, r25
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1cc
    bl fn_80239EE8
    b @L_8023FF1C
@L_8023FF0C:
    addi r29, r29, 0x1
@L_8023FF10:
    rlwinm r0, r29, 0, 16, 31
    cmplw r0, r22
    blt @L_8023FE2C
@L_8023FF1C:
    addi r30, r1, 0x1c
    rlwinm r22, r31, 0, 16, 31
    li r29, 0x0
    b @L_8023FFE8
@L_8023FF2C:
    clrlslwi r0, r29, 16, 2
    lwzx r4, r30, r0
    cmplw r24, r4
    beq @L_8023FFE4
    mr r3, r23
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x1
    bl fn_802367CC
    rlwinm. r4, r3, 0, 16, 31
    mr r31, r3
    beq @L_8023FFE4
    addi r3, r1, 0x8
    li r27, 0x0
    b @L_8023FFC8
@L_8023FF68:
    clrlslwi r0, r27, 16, 1
    lhzx r0, r3, r0
    cmplwi r0, 0x9c
    bne @L_8023FFC4
    mr r3, r28
    mr r4, r23
    li r5, 0x1cd
    bl fn_80239984
    mr r0, r3
    mr r3, r24
    mr r28, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r23
    mr r8, r25
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1cd
    bl fn_80239EE8
    b @L_8023FFD4
@L_8023FFC4:
    addi r27, r27, 0x1
@L_8023FFC8:
    rlwinm r0, r27, 0, 16, 31
    cmplw r0, r4
    blt @L_8023FF68
@L_8023FFD4:
    rlwinm r3, r27, 0, 16, 31
    rlwinm r0, r31, 0, 16, 31
    cmplw r3, r0
    blt @L_8023FFF4
@L_8023FFE4:
    addi r29, r29, 0x1
@L_8023FFE8:
    rlwinm r0, r29, 0, 16, 31
    cmplw r0, r22
    blt @L_8023FF2C
@L_8023FFF4:
    mr r3, r23
    mr r4, r26
    li r5, 0x8
    bl fn_80236BFC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80240014
    li r0, -0x1
    b @L_80240070
@L_80240014:
    mr r3, r23
    mr r4, r26
    li r5, 0x30
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    mr r3, r26
    subfic r0, r0, 0x1
    li r4, 0x8
    cntlzw r0, r0
    rlwinm r5, r0, 27, 5, 31
    addi r27, r5, 0x1
    bl fn_80202108
    add r22, r3, r27
    mr r3, r26
    li r4, 0x8
    bl fn_80202234
    extsb r3, r3
    extsb r0, r22
    cmpw r0, r3
    blt @L_8024006C
    li r0, 0x1
    b @L_80240070
@L_8024006C:
    li r0, 0x0
@L_80240070:
    extsb. r0, r0
    bne @L_802400C0
    mr r3, r28
    mr r4, r23
    li r5, 0x1ce
    bl fn_80239984
    mr r0, r3
    mr r3, r24
    mr r28, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r23
    mr r8, r25
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1ce
    bl fn_80239EE8
@L_802400C0:
    mr r3, r28
    lmw r21, 0x44(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
