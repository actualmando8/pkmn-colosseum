    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    addi r11, r1, 0x30
    bl _savefpr_28
    stw r31, 0xc(r1)
    mr r31, r3
    fmr f28, f1
    fmr f29, f2
    lwz r3, 0xc(r3)
    fmr f30, f3
    li r4, 0x1
    fmr f31, f4
    bl fn_801943BC
    fmr f1, f28
    lwz r3, 0xc(r31)
    fmr f2, f29
    bl fn_801943A0
    fmr f1, f30
    lwz r3, 0xc(r31)
    bl fn_801944E8
    fmr f1, f31
    lwz r3, 0xc(r31)
    bl fn_801944C0
    li r0, 0x1
    addi r11, r1, 0x30
    stb r0, 0x2(r31)
    bl _restfpr_28
    lwz r0, 0x34(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
