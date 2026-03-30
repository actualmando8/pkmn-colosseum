    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r31, r3
    lis r4, 0x6dc
    addi r4, r4, 0x1605
    bl fn_800F9318
    stw r3, -24976(r13)
    lis r4, 0x6dc
    mr r3, r31
    addi r4, r4, 0x1001
    bl fn_800F9318
    stw r3, -24972(r13)
    li r3, 0x280
    li r4, 0x1e0
    bl fn_800E8FA0
    lis r3, 0x6dc
    addi r3, r3, 0x1000
    bl fn_801CB7C4
    lis r4, 0x1124
    mr r3, r31
    addi r4, r4, 0x1400
    bl fn_800F9318
    lis r4, 0x6dc
    mr r30, r3
    mr r3, r31
    addi r4, r4, 0x1000
    bl fn_800F9318
    mr r4, r30
    bl fn_800E3C08
    lis r4, 0x6dc
    mr r3, r31
    addi r4, r4, 0x1000
    bl fn_800F9318
    li r4, 0x4
    bl fn_800E3C00
    lis r3, 0x6dc
    li r4, 0x2
    addi r3, r3, 0x1000
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    li r29, 0x64
    bl fn_800D37CC
    cmpwi r3, 0x32
    bne @L_80091A64
    lfs f1, -29892(r2)
    bl fn_800C46B0
    mr r29, r3
    cmplwi r29, 0x1
    bge @L_80091A64
    li r29, 0x1
@L_80091A64:
    li r30, 0x0
    b @L_80091A78
@L_80091A6C:
    bl fn_800F0308
    bl fn_800D3088
    add r30, r30, r3
@L_80091A78:
    cmplw r30, r29
    blt @L_80091A6C
    lis r3, 0x6bd
    addi r3, r3, 0x400
    bl fn_801CBA0C
    li r4, 0x5
    mr r28, r3
    li r5, 0x0
    li r6, 0x1
    bl fn_801CB834
    mr r3, r31
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
    lis r4, 0xc40
    mr r3, r31
    addi r4, r4, 0x1800
    li r5, 0x0
    li r6, 0x0
    bl fn_80176E0C
    li r30, 0x1
    bl fn_800D37CC
    cmpwi r3, 0x32
    bne @L_80091B14
    lfs f1, -29904(r2)
    bl fn_800C46B0
    mr r30, r3
    cmplwi r30, 0x1
    bge @L_80091B14
    li r30, 0x1
@L_80091B14:
    li r29, 0x0
    b @L_80091B28
@L_80091B1C:
    bl fn_800F0308
    bl fn_800D3088
    add r29, r29, r3
@L_80091B28:
    cmplw r29, r30
    blt @L_80091B1C
    lis r3, 0xc3b
    addi r3, r3, 0x1000
    bl fn_801CBA0C
    mr r0, r3
    mr r3, r31
    mr r6, r0
    mr r4, r28
    mr r5, r31
    li r7, 0x0
    bl fn_801845E4
    li r3, 0x1
    bl fn_80176B48
    li r3, 0x82
    bl fn_800FF58C
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
