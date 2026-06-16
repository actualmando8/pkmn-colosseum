    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    addi r11, r1, 0x40
    bl _savefpr_26
    fmr f26, f1
    cmpwi r3, 0x0
    fmr f27, f2
    fmr f28, f3
    fmr f29, f4
    fmr f30, f5
    fmr f31, f6
    beq @L_80196CAC
    bl fn_800AA2F0
    fmr f1, f26
    fmr f2, f27
    fmr f3, f28
    fmr f4, f29
    fmr f5, f30
    fmr f6, f31
    bl fn_800BD640
    b @L_80196CC8
@L_80196CAC:
    fmr f1, f26
    fmr f2, f27
    fmr f3, f28
    fmr f4, f29
    fmr f5, f30
    fmr f6, f31
    bl fn_800BD744
@L_80196CC8:
    addi r11, r1, 0x40
    bl _restfpr_26
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
