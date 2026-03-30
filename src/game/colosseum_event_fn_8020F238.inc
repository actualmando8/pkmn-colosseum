    nofralloc
    stwu r1, -0x700(r1)
    mflr r0
    stw r0, 0x704(r1)
    stmw r28, 0x6f0(r1)
    mr r28, r3
    mr r29, r4
    li r3, 0x0
    li r4, 0x1
    bl fn_801F453C
    rlwinm r30, r3, 0, 24, 31
    mr r4, r28
    li r3, 0x0
    li r5, 0x1f
    li r6, 0x0
    bl fn_8011BEB4
    mr r3, r29
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r29
    mr r31, r0
    mr r4, r30
    bl fn_80207C6C
    mr r30, r3
    mr r4, r29
    addi r3, r1, 0x8
    bl fn_801FCEC4
    mr r7, r30
    addi r3, r1, 0x8
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_801254B4
    mr r3, r29
    mr r4, r28
    li r5, 0x3
    li r6, 0x0
    bl fn_80208750
    mr r4, r28
    addi r3, r1, 0x8
    li r5, 0x3
    li r6, 0x1
    bl fn_80208750
    mr r3, r29
    mr r4, r28
    li r5, 0x3
    li r6, 0x1
    li r7, 0x0
    bl fn_802085C4
    mr r3, r31
    mr r4, r30
    bl fn_801C3D64
    bl fn_801C3430
    mr r3, r29
    mr r7, r30
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_801254B4
    mr r3, r29
    mr r4, r28
    li r5, 0x3
    li r6, 0x0
    li r7, 0x0
    bl fn_802085C4
    mr r3, r31
    bl fn_801DB100
    lmw r28, 0x6f0(r1)
    lwz r0, 0x704(r1)
    mtlr r0
    addi r1, r1, 0x700
    blr
