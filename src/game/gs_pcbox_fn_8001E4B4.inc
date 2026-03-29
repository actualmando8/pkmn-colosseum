    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r26, r3
    mr r27, r4
    mr r28, r5
    mr r29, r6
    mr r30, r7
    mr r31, r8
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    li r3, 0x3
    bl fn_800D6A00
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    li r3, 0x3
    bl fn_800D67BC
    extsh r3, r26
    extsh r4, r27
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    li r7, 0xff
    bl fn_800D5CB8
    extsh r3, r28
    extsh r4, r29
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    li r7, 0xff
    bl fn_800D5CB8
    extsh r3, r30
    extsh r4, r31
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    li r7, 0xff
    bl fn_800D5CB8
    bl fn_800D6728
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
