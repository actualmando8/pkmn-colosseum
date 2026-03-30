    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    cmpwi r3, 0x0
    blt @L_800936C4
    cmpwi r3, 0x3
    ble @L_800936CC
@L_800936C4:
    li r3, 0x0
    b @L_800937D4
@L_800936CC:
    lis r4, -32704
    rlwinm r30, r3, 2, 0, 29
    addi r31, r4, -19672
    lwzx r28, r31, r30
    cmplwi r28, 0x0
    bne @L_800936EC
    li r3, 0x1
    b @L_800937D4
@L_800936EC:
    lwz r3, 0x4338(r28)
    li r4, 0x1
    bl fn_800716E8
@L_800936F8:
    mr r3, r28
    bl fn_8009F7B4
    lwz r29, 0x433c(r28)
    mr r3, r28
    bl fn_8009F890
    addi r3, r28, 0x20
    li r4, 0x8
    bl fn_800A257C
    rlwinm r0, r29, 16, 16, 31
    cmpwi r0, 0x3
    bne @L_8009372C
    bl fn_800F0308
    b @L_800936F8
@L_8009372C:
    mr r3, r28
    bl fn_8009F7B4
    li r0, 0xd
    lis r3, 0x3
    stw r0, 0x4340(r28)
    addi r0, r3, 0xd
    mr r3, r28
    stw r0, 0x433c(r28)
    bl fn_8009F890
    addi r3, r28, 0x20
    li r4, 0x8
    bl fn_800A257C
    addi r3, r28, 0x18
    bl fn_8009FABC
    addi r3, r28, 0x20
    li r4, 0x0
    bl fn_800A1E54
    lwz r3, 0x4338(r28)
    li r4, 0x0
    li r5, 0x0
    bl fn_800716C8
    lwz r3, 0x4338(r28)
    li r4, 0x0
    bl fn_800716E8
    lwzx r3, r31, r30
    bl fn_800E202C
    mr r29, r3
    rlwinm r0, r29, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_800937B8
    lis r3, -32729
    li r4, 0x1e6
    addi r3, r3, -2648
    addi r5, r2, -29880
    bl fn_80196E10
@L_800937B8:
    mr r3, r29
    bl fn_800E24B0
    mr r3, r29
    bl fn_800E209C
    li r0, 0x0
    li r3, 0x1
    stwx r0, r31, r30
@L_800937D4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
