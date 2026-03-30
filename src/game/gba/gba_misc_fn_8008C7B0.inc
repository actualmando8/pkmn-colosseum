    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r31, r3
    lis r4, 0xce6
    addi r4, r4, 0x1602
    bl fn_800F9318
    stw r3, -24976(r13)
    lis r4, 0xce6
    mr r3, r31
    addi r4, r4, 0x1002
    bl fn_800F9318
    stw r3, -24972(r13)
    li r3, 0x280
    li r4, 0x1e0
    bl fn_800E8FA0
    lis r3, 0xce6
    li r4, 0x0
    addi r3, r3, 0x1000
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    li r29, 0x1
    bl fn_800D37CC
    cmpwi r3, 0x32
    bne @L_8008C834
    lfs f1, -29904(r2)
    bl fn_800C46B0
    mr r29, r3
    cmplwi r29, 0x1
    bge @L_8008C834
    li r29, 0x1
@L_8008C834:
    li r30, 0x0
    b @L_8008C848
@L_8008C83C:
    bl fn_800F0308
    bl fn_800D3088
    add r30, r30, r3
@L_8008C848:
    cmplw r30, r29
    blt @L_8008C83C
    lis r3, 0xce6
    addi r3, r3, 0x1000
    bl fn_801CB7C4
    lis r4, 0xce6
    mr r3, r31
    addi r4, r4, 0x1000
    bl fn_800F9318
    mr r30, r3
    li r4, 0x1
    lwz r3, 0x144(r30)
    bl fn_80118874
    li r0, 0x0
    lis r4, 0xce6
    stw r0, 0x144(r30)
    mr r3, r31
    lfs f0, -29900(r2)
    addi r4, r4, 0x1004
    stfs f0, 0x8(r1)
    bl fn_800F9318
    li r4, 0x0
    mr r29, r3
    bl fn_800ECCA8
    mr r3, r29
    addi r4, r1, 0x8
    li r5, 0x0
    bl fn_800EC4D0
    lfs f1, 0x8(r1)
    mr r3, r29
    lfs f0, -29896(r2)
    li r4, 0x0
    fsubs f0, f1, f0
    stfs f0, 0x8(r1)
    bl fn_800ECCA8
    lfs f1, 0x8(r1)
    mr r3, r29
    bl fn_800ECA78
    mr r3, r29
    li r4, 0x0
    bl fn_800ECB74
    mr r3, r29
    bl fn_800EC990
    lis r3, 0x6af
    addi r3, r3, 0x400
    bl fn_801CBA0C
    lis r4, 0xb72
    mr r28, r3
    addi r3, r4, 0x400
    bl fn_801CBA0C
    mr r0, r3
    mr r3, r31
    mr r27, r0
    mr r4, r28
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
    mr r3, r31
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
    lis r4, 0x1151
    mr r3, r31
    addi r4, r4, 0x1400
    bl fn_800F9318
    mr r30, r3
    mr r3, r31
    mr r4, r28
    bl fn_800F9318
    mr r4, r30
    bl fn_800E3C08
    mr r3, r31
    mr r4, r28
    bl fn_800F9318
    li r4, 0x4
    bl fn_800E3C00
    lis r4, 0xd04
    mr r3, r31
    addi r4, r4, 0x1800
    li r5, 0x0
    li r6, 0x0
    bl fn_80176E0C
    li r30, 0x1
    bl fn_800D37CC
    cmpwi r3, 0x32
    bne @L_8008C9FC
    lfs f1, -29904(r2)
    bl fn_800C46B0
    mr r30, r3
    cmplwi r30, 0x1
    bge @L_8008C9FC
    li r30, 0x1
@L_8008C9FC:
    li r29, 0x0
    b @L_8008CA10
@L_8008CA04:
    bl fn_800F0308
    bl fn_800D3088
    add r29, r29, r3
@L_8008CA10:
    cmplw r29, r30
    blt @L_8008CA04
    lis r3, 0xd0d
    addi r3, r3, 0x1000
    bl fn_801CBA0C
    lis r4, 0xd0d
    mr r29, r3
    addi r3, r4, 0x1001
    bl fn_801CBA0C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    mr r4, r28
    mr r5, r31
    mr r6, r29
    li r7, 0x0
    bl fn_801845E4
    mr r3, r31
    mr r4, r27
    mr r5, r31
    mr r6, r30
    li r7, 0x0
    bl fn_801845E4
    mr r3, r28
    li r4, 0x9
    li r5, 0x0
    li r6, 0x1
    bl fn_801CB834
    mr r3, r27
    li r4, 0x6
    li r5, 0x0
    li r6, 0x1
    bl fn_801CB834
    li r3, 0x1
    bl fn_80176B48
    li r3, 0x8d0
    bl fn_80190528
    li r3, 0x1
    bl fn_800FF58C
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
