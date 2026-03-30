    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    li r31, 0x0
    bl fn_8023831C
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1d
    beq @L_8023F080
    cmplwi r0, 0x18
    bne @L_8023F0C8
@L_8023F080:
    mr r4, r27
    li r3, 0x0
    li r5, 0x1e2
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1e2
    bl fn_80239EE8
@L_8023F0C8:
    mr r3, r27
    mr r4, r30
    li r5, 0x3c
    bl fn_80237F74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023F12C
    mr r3, r31
    mr r4, r27
    li r5, 0x1e3
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1e3
    bl fn_80239EE8
@L_8023F12C:
    mr r3, r31
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
