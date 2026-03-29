    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    li r3, 0x6
    bl fn_800D6A00
    li r3, 0x4
    bl fn_800D67BC
    li r3, 0x0
    li r4, 0x0
    bl fn_800D61E4
    lis r4, 0x3b
    li r3, 0x0
    addi r4, r4, 0x6dff
    bl fn_800D5BA0
    lha r3, 0x54(r31)
    li r4, 0x0
    bl fn_800D61E4
    lis r4, 0x489e
    li r3, 0x0
    subi r4, r4, 0x1301
    bl fn_800D5BA0
    lha r3, 0x54(r31)
    lha r4, 0x56(r31)
    bl fn_800D61E4
    lis r4, 0x489e
    li r3, 0x0
    subi r4, r4, 0x1301
    bl fn_800D5BA0
    lha r4, 0x56(r31)
    li r3, 0x0
    bl fn_800D61E4
    lis r4, 0x3b
    li r3, 0x0
    addi r4, r4, 0x6dff
    bl fn_800D5BA0
    bl fn_800D6728
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
