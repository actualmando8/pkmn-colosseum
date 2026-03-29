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
    mr r31, r5
    mr r30, r6
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    li r3, 0x7
    bl fn_800D6A00
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    li r3, 0x2
    bl fn_800D67BC
    subi r3, r28, 0xa
    subi r0, r29, 0xa
    extsh r3, r3
    extsh r4, r0
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xc0
    bl fn_800D5BA0
    addi r31, r31, 0xa
    addi r30, r30, 0xa
    add r31, r28, r31
    add r30, r29, r30
    extsh r3, r31
    extsh r4, r30
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xc0
    bl fn_800D5BA0
    bl fn_800D6728
    lfs f1, lbl_8047B7E0(r2)
    bl fn_800D5648
    li r3, 0x2
    bl fn_800D6A00
    li r3, 0x5
    bl fn_800D67BC
    subi r3, r28, 0xa
    subi r0, r29, 0xa
    extsh r3, r3
    extsh r4, r0
    bl fn_800D61E4
    li r3, 0x0
    li r4, -0x1
    bl fn_800D5BA0
    subi r0, r29, 0xa
    extsh r3, r31
    extsh r4, r0
    bl fn_800D61E4
    li r3, 0x0
    li r4, -0x1
    bl fn_800D5BA0
    extsh r3, r31
    extsh r4, r30
    bl fn_800D61E4
    li r3, 0x0
    li r4, -0x1
    bl fn_800D5BA0
    subi r0, r28, 0xa
    extsh r4, r30
    extsh r3, r0
    bl fn_800D61E4
    li r3, 0x0
    li r4, -0x1
    bl fn_800D5BA0
    subi r3, r28, 0xa
    subi r0, r29, 0xa
    extsh r3, r3
    extsh r4, r0
    bl fn_800D61E4
    li r3, 0x0
    li r4, -0x1
    bl fn_800D5BA0
    bl fn_800D6728
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
