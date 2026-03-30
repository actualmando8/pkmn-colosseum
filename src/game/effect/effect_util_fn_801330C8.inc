    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x1
    stw r0, 0x14(r1)
    bl fn_800D88DC
    li r3, 0x6
    bl fn_800D888C
    lfs f1, lbl_8047D0F0(r2)
    lfs f3, lbl_8047D0F4(r2)
    fmr f2, f1
    lfs f4, lbl_8047D0F8(r2)
    bl fn_800D9B58
    li r3, 0x0
    li r4, 0x6
    li r5, 0x7
    bl fn_800DA4C4
    li r3, 0x1
    li r4, 0x1
    li r5, 0x0
    bl fn_800DA2BC
    li r3, 0x0
    li r4, 0x1
    li r5, 0x1
    bl fn_800DA1E8
    li r3, 0x0
    bl fn_800DA028
    li r3, 0x4
    bl fn_800D6A00
    li r3, 0x0
    bl fn_800D7820
    li r3, 0x3
    bl fn_800D67BC
    lis r3, lbl_80478AC0@ha
    lfs f1, lbl_8047D0FC(r2)
    lfs f0, lbl_80478AC0@l(r3)
    lfs f3, lbl_8047D0F0(r2)
    fneg f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0xff
    li r5, 0x80
    li r6, 0x40
    li r7, 0x0
    bl fn_800D5CB8
    lis r3, lbl_80478AC4@ha
    lfs f2, lbl_8047D100(r2)
    lfs f3, lbl_8047D0F0(r2)
    lfs f1, lbl_80478AC4@l(r3)
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x40
    li r5, 0xff
    li r6, 0x0
    li r7, 0x0
    bl fn_800D5CB8
    lis r3, lbl_80478AC4@ha
    lfs f1, lbl_8047D104(r2)
    lfs f3, lbl_8047D0F0(r2)
    lfs f2, lbl_80478AC4@l(r3)
    bl fn_800D6680
    li r3, 0x0
    li r4, 0x0
    li r5, 0x40
    li r6, 0xff
    li r7, 0x0
    bl fn_800D5CB8
    lis r3, lbl_80478AC0@ha
    lfs f1, lbl_8047D0FC(r2)
    lfs f0, lbl_80478AC0@l(r3)
    lfs f3, lbl_8047D0F0(r2)
    fneg f2, f0
    bl fn_800D6680
    li r3, 0x0
    li r4, 0xff
    li r5, 0x80
    li r6, 0x40
    li r7, 0x0
    bl fn_800D5CB8
    bl fn_800D6728
    lwz r0, 0x14(r1)
    li r3, 0x0
    mtlr r0
    addi r1, r1, 0x10
    blr
