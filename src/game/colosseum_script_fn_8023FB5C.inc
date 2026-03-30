    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r30, r5
    mr r28, r3
    mr r29, r4
    li r31, 0x0
    li r5, 0x8
    bl fn_80236BFC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023FD2C
    mr r3, r28
    mr r4, r29
    li r5, 0x8
    bl fn_80236BFC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023FBB0
    li r0, -0x1
    b @L_8023FC0C
@L_8023FBB0:
    mr r3, r28
    mr r4, r29
    li r5, 0x30
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    mr r3, r29
    subfic r0, r0, 0x1
    li r4, 0x8
    cntlzw r0, r0
    rlwinm r5, r0, 27, 5, 31
    addi r27, r5, 0x1
    bl fn_80202108
    add r27, r3, r27
    mr r3, r29
    li r4, 0x8
    bl fn_80202234
    extsb r3, r3
    extsb r0, r27
    cmpw r0, r3
    blt @L_8023FC08
    li r0, 0x1
    b @L_8023FC0C
@L_8023FC08:
    li r0, 0x0
@L_8023FC0C:
    extsb. r0, r0
    bne @L_8023FC5C
    mr r4, r28
    li r3, 0x0
    li r5, 0x1d0
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1d0
    bl fn_80239EE8
@L_8023FC5C:
    mr r3, r28
    mr r4, r29
    li r5, 0x8
    bl fn_80236BFC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023FC7C
    li r0, -0x1
    b @L_8023FCD8
@L_8023FC7C:
    mr r3, r28
    mr r4, r29
    li r5, 0x30
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    mr r3, r29
    subfic r0, r0, 0x1
    li r4, 0x8
    cntlzw r0, r0
    rlwinm r5, r0, 27, 5, 31
    addi r27, r5, 0x1
    bl fn_80202108
    add r27, r3, r27
    mr r3, r29
    li r4, 0x8
    bl fn_80202234
    extsb r3, r3
    extsb r0, r27
    cmpw r0, r3
    blt @L_8023FCD4
    li r0, 0x1
    b @L_8023FCD8
@L_8023FCD4:
    li r0, 0x0
@L_8023FCD8:
    extsb r0, r0
    cmpwi r0, 0x1
    bne @L_8023FD2C
    mr r3, r31
    mr r4, r28
    li r5, 0x1d1
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1d1
    bl fn_80239EE8
@L_8023FD2C:
    mr r3, r31
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
