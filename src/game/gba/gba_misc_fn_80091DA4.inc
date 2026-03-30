    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    lis r4, 0xce6
    addi r4, r4, 0x1602
    bl fn_800F9318
    stw r3, -24976(r13)
    lis r4, 0xce6
    mr r3, r29
    addi r4, r4, 0x1002
    bl fn_800F9318
    stw r3, -24972(r13)
    li r3, 0x280
    li r4, 0x1e0
    bl fn_800E8FA0
    lis r3, 0xce6
    addi r3, r3, 0x1000
    bl fn_801CB7C4
    lis r3, 0xce6
    li r4, 0x1
    addi r3, r3, 0x1004
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    lis r4, 0x111b
    mr r3, r29
    addi r4, r4, 0x1400
    bl fn_800F9318
    lis r4, 0xce6
    mr r31, r3
    mr r3, r29
    addi r4, r4, 0x1000
    bl fn_800F9318
    mr r4, r31
    bl fn_800E3C08
    lis r4, 0xce6
    mr r3, r29
    addi r4, r4, 0x1000
    bl fn_800F9318
    li r4, 0x4
    bl fn_800E3C00
    lis r3, 0xce6
    li r4, 0x3
    addi r3, r3, 0x1000
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    li r30, 0x32
    bl fn_800D37CC
    cmpwi r3, 0x32
    bne @L_80091E98
    lfs f1, -29888(r2)
    bl fn_800C46B0
    mr r30, r3
    cmplwi r30, 0x1
    bge @L_80091E98
    li r30, 0x1
@L_80091E98:
    li r31, 0x0
    b @L_80091EAC
@L_80091EA0:
    bl fn_800F0308
    bl fn_800D3088
    add r31, r31, r3
@L_80091EAC:
    cmplw r31, r30
    blt @L_80091EA0
    lis r4, 0xcef
    mr r3, r29
    addi r4, r4, 0x1800
    li r5, 0x0
    li r6, 0x0
    bl fn_80176E0C
    li r31, 0x1
    bl fn_800D37CC
    cmpwi r3, 0x32
    bne @L_80091EF4
    lfs f1, -29904(r2)
    bl fn_800C46B0
    mr r31, r3
    cmplwi r31, 0x1
    bge @L_80091EF4
    li r31, 0x1
@L_80091EF4:
    li r30, 0x0
    b @L_80091F08
@L_80091EFC:
    bl fn_800F0308
    bl fn_800D3088
    add r30, r30, r3
@L_80091F08:
    cmplw r30, r31
    blt @L_80091EFC
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
    mtlr r0
    addi r1, r1, 0x20
    blr
