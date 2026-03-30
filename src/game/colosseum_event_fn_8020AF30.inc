    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r4, 0x1
    stw r0, 0x24(r1)
    stmw r30, 0x18(r1)
    mr r30, r3
    li r3, 0x0
    bl fn_8020DA14
    li r3, 0x0
    li r4, 0x0
    bl fn_801F453C
    li r0, 0x0
    lis r4, -32735
    stw r0, 0x8(r1)
    rlwinm r31, r3, 0, 24, 31
    addi r4, r4, -20492
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x0
    bl fn_801F37B0
    cmplwi r31, 0x0
    beq @L_8020AFDC
    lwz r7, 0x8(r1)
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r31
    bl fn_80136368
    mr r0, r3
    li r3, 0x0
    mr r7, r0
    li r4, 0x0
    li r5, 0x50
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
    bl fn_8020D8F0
    mr r4, r3
    mr r3, r30
    li r5, 0x0
    bl fn_80211B94
@L_8020AFDC:
    lmw r30, 0x18(r1)
    li r3, 0x1
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
