    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r31, r7
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    li r3, 0x7
    bl fn_800D6A00
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    add r29, r29, r27
    add r30, r30, r28
    li r3, 0x2
    bl fn_800D67BC
    extsh r3, r27
    extsh r4, r28
    bl fn_800D61E4
    lbz r4, 0x0(r31)
    li r3, 0x0
    lbz r5, 0x1(r31)
    lbz r6, 0x2(r31)
    lbz r7, 0x3(r31)
    bl fn_800D5CB8
    extsh r3, r29
    extsh r4, r30
    bl fn_800D61E4
    lbz r4, 0x0(r31)
    li r3, 0x0
    lbz r5, 0x1(r31)
    lbz r6, 0x2(r31)
    lbz r7, 0x3(r31)
    bl fn_800D5CB8
    bl fn_800D6728
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
