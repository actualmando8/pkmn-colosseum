    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    addi r11, r1, 0x30
    bl _savefpr_28
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    fmr f28, f1
    mr r30, r3
    fmr f29, f2
    fmr f30, f3
    fmr f31, f4
    mr r31, r4
    cmplwi r31, 0x0
    bne @L_801C5F10
    b @L_801C5F4C
@L_801C5F10:
    lbz r0, -21616(r13)
    cmplwi r0, 0x1
    bne @L_801C5F30
    li r0, 0x0
    mr r3, r31
    stb r0, -21616(r13)
    li r4, 0x8
    bl fn_801C6274
@L_801C5F30:
    fmr f1, f28
    mr r3, r30
    fmr f2, f29
    mr r4, r31
    fmr f3, f30
    fmr f4, f31
    bl fn_801C6008
@L_801C5F4C:
    addi r11, r1, 0x30
    bl _restfpr_28
    lwz r0, 0x34(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
