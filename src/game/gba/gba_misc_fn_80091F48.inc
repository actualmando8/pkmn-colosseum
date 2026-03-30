    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r30, r3
    lis r4, 0x6dd
    addi r4, r4, 0x1604
    bl fn_800F9318
    stw r3, -24976(r13)
    lis r4, 0x6dd
    mr r3, r30
    addi r4, r4, 0x1001
    bl fn_800F9318
    stw r3, -24972(r13)
    li r3, 0x280
    li r4, 0x1e0
    bl fn_800E8FA0
    lis r3, 0x6bb
    addi r3, r3, 0x400
    bl fn_801CBA0C
    lis r4, 0xd24
    mr r31, r3
    addi r3, r4, 0x400
    bl fn_801CBA0C
    mr r0, r3
    mr r3, r30
    mr r27, r0
    mr r4, r31
    bl fn_800F9318
    li r4, 0x2
    mr r29, r3
    bl fn_800E9108
    lwz r4, -24976(r13)
    mr r3, r29
    bl fn_800E8FE8
    mr r3, r29
    li r4, 0x1
    addi r5, r13, -24972
    bl fn_800E900C
    mr r3, r30
    mr r4, r27
    bl fn_800F9318
    li r4, 0x2
    mr r29, r3
    bl fn_800E9108
    lwz r4, -24976(r13)
    mr r3, r29
    bl fn_800E8FE8
    mr r3, r29
    li r4, 0x1
    addi r5, r13, -24972
    bl fn_800E900C
    lis r4, 0xb89
    mr r3, r30
    addi r4, r4, 0x1800
    li r5, 0x0
    li r6, 0x0
    bl fn_80176E0C
    li r28, 0x1
    bl fn_800D37CC
    cmpwi r3, 0x32
    bne @L_80092058
    lfs f1, -29904(r2)
    bl fn_800C46B0
    mr r28, r3
    cmplwi r28, 0x1
    bge @L_80092058
    li r28, 0x1
@L_80092058:
    li r29, 0x0
    b @L_8009206C
@L_80092060:
    bl fn_800F0308
    bl fn_800D3088
    add r29, r29, r3
@L_8009206C:
    cmplw r29, r28
    blt @L_80092060
    lis r3, 0xb86
    addi r3, r3, 0x1000
    bl fn_801CBA0C
    lis r4, 0xb86
    mr r29, r3
    addi r3, r4, 0x1001
    bl fn_801CBA0C
    mr r0, r3
    mr r3, r30
    mr r28, r0
    mr r4, r31
    mr r5, r30
    mr r6, r29
    li r7, 0x0
    bl fn_801845E4
    mr r3, r30
    mr r4, r27
    mr r5, r30
    mr r6, r28
    li r7, 0x0
    bl fn_801845E4
    mr r3, r27
    li r4, 0x8
    li r5, 0x0
    li r6, 0x1
    bl fn_801CB834
    mr r3, r31
    li r4, 0x8
    li r5, 0x32
    li r6, 0x0
    bl fn_801CB834
    mr r3, r31
    li r4, 0x0
    bl fn_801CB708
    mr r3, r31
    li r4, 0x9
    li r5, 0x0
    li r6, 0x1
    bl fn_801CB834
    li r3, 0x1
    bl fn_80176B48
    li r3, 0x89
    bl fn_800FF58C
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
