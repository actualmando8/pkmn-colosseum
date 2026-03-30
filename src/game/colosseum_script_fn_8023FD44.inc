    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lfs f1, -20588(r2)
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r30, r5
    mr r28, r3
    mr r29, r4
    li r31, 0x0
    li r5, 0x1
    bl fn_802373B0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023FDC4
    mr r4, r28
    li r3, 0x0
    li r5, 0x1cf
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
    li r10, 0x1cf
    bl fn_80239EE8
@L_8023FDC4:
    mr r3, r31
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
