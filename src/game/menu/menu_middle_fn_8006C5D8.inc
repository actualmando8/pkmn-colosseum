    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r31, r4
    lis r3, lbl_80268674@ha
    lha r5, 0x54(r31)
    addi r9, r3, lbl_80268674@l
    lha r6, 0x56(r31)
    lwz r8, 0x0(r9)
    li r3, 0x0
    lwz r7, 0x4(r9)
    li r4, 0x0
    lwz r0, 0x8(r9)
    stw r8, 0x8(r1)
    stw r7, 0xc(r1)
    stw r0, 0x10(r1)
    bl fn_800FE38C
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    lbz r5, 0xf(r1)
    lis r3, 0x8081
    lbz r4, 0x67(r31)
    subi r6, r3, 0x7f7f
    lbz r0, 0x13(r1)
    li r3, 0x6
    mullw r5, r5, r4
    mullw r0, r0, r4
    mulhw r7, r6, r5
    mulhw r4, r6, r0
    add r5, r7, r5
    srawi r5, r5, 7
    add r0, r4, r0
    srwi r6, r5, 31
    srawi r0, r0, 7
    srwi r4, r0, 31
    add r5, r5, r6
    add r0, r0, r4
    clrlwi r4, r5, 24
    clrlwi r0, r0, 24
    stb r4, 0xf(r1)
    stb r0, 0x13(r1)
    bl fn_800D6A00
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    li r3, 0x4
    bl fn_800D67BC
    li r3, 0x0
    li r4, 0x0
    bl fn_800D61E4
    lwz r4, 0xc(r1)
    li r3, 0x0
    bl fn_800D5BA0
    lha r3, 0x54(r31)
    li r4, 0x0
    bl fn_800D61E4
    lwz r4, 0xc(r1)
    li r3, 0x0
    bl fn_800D5BA0
    lha r3, 0x54(r31)
    lha r4, 0x56(r31)
    bl fn_800D61E4
    lwz r4, 0x10(r1)
    li r3, 0x0
    bl fn_800D5BA0
    lha r4, 0x56(r31)
    li r3, 0x0
    bl fn_800D61E4
    lwz r4, 0x10(r1)
    li r3, 0x0
    bl fn_800D5BA0
    bl fn_800D6728
    lfs f1, lbl_8047C060(r2)
    bl fn_800D5648
    li r3, 0x1
    bl fn_800D6A00
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    lbz r0, 0x67(r31)
    li r4, 0xff
    lis r3, 0x8081
    stb r4, 0x8(r1)
    mulli r0, r0, 0x38
    li r30, 0x0
    subi r3, r3, 0x7f7f
    stb r4, 0x9(r1)
    mulhw r3, r3, r0
    stb r4, 0xa(r1)
    add r0, r3, r0
    srawi r0, r0, 7
    srwi r3, r0, 31
    add r0, r0, r3
    clrlwi r0, r0, 24
    stb r0, 0xb(r1)
    b @L_8006C7A8
    @L_8006C768:
    li r3, 0x2
    bl fn_800D67BC
    mr r4, r30
    li r3, 0x0
    bl fn_800D61E4
    lwz r4, 0x8(r1)
    li r3, 0x0
    bl fn_800D5BA0
    lha r3, 0x54(r31)
    mr r4, r30
    bl fn_800D61E4
    lwz r4, 0x8(r1)
    li r3, 0x0
    bl fn_800D5BA0
    bl fn_800D6728
    addi r30, r30, 0x4
    @L_8006C7A8:
    lha r0, 0x56(r31)
    extsh r3, r30
    cmpw r3, r0
    blt @L_8006C768
    bl fn_800FE35C
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
