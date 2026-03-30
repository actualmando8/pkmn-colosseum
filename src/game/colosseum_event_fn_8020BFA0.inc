    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    li r3, 0x0
    bl fn_801F4718
    li r3, 0x0
    li r4, 0x0
    bl fn_801F3B24
    bl fn_80211830
    li r0, 0x0
    lis r3, -32735
    stb r0, 0x8(r1)
    addi r4, r3, -16120
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x0
    bl fn_801F37B0
    mr r3, r31
    bl fn_8020D920
    lis r4, 0x8037
    lis r5, 0x8038
    addi r7, r4, 23752
    li r6, 0x0
    addi r8, r5, -30048
    li r4, 0x0
    li r5, 0x6
    bl fn_80211948
    lis r4, -32735
    li r3, 0x0
    addi r4, r4, -16156
    li r5, 0x0
    li r6, 0x1
    bl fn_801F37B0
    li r3, 0x1
    bl fn_8022E314
    bl fn_8022E1C4
    lis r4, -32735
    li r3, 0x0
    addi r4, r4, -16192
    li r5, 0x0
    li r6, 0x1
    bl fn_801F37B0
    li r0, 0x1
    lis r3, -32735
    stb r0, 0x8(r1)
    addi r4, r3, -16120
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x0
    bl fn_801F37B0
    li r3, 0x0
    bl fn_801F3178
    li r3, 0x0
    bl fn_801F3074
    li r3, 0x0
    bl fn_801F2F3C
    bl fn_800E0C54
    rlwinm r7, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x5b
    li r6, 0x0
    bl fn_801F4C14
    bl fn_801DA7AC
    lwz r0, 0x24(r1)
    li r3, 0x1
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr