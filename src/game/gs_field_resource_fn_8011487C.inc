    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    rlwinm r0, r29, 0, 1, 15
    lis r4, -32729
    addi r31, r4, 8704
    ori r4, r0, 0x400
    bl fn_800F9318
    mr r5, r30
    mr r30, r3
    addi r4, r30, 0x60
    bl fn_80191F64
    mr r3, r30
    addi r4, r31, 0xac
    bl fn_80191ECC
    mr r30, r3
    cmplwi r30, 0x0
    bne @L_801148F0
    addi r3, r31, 0x1cc
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_80114924
@L_801148F0:
    lwz r3, 0x4(r30)
    bl fn_800D27FC
    cmplwi r3, 0x0
    bne @L_80114910
    addi r3, r31, 0x200
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_80114924
@L_80114910:
    lis r5, -32751
    mr r4, r28
    addi r6, r5, 20700
    mr r5, r29
    bl fn_800F9378
@L_80114924:
    mr r3, r30
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
