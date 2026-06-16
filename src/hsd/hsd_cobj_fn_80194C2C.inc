    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    beq @L_80194C48
    cmplwi r4, 0x0
    bne @L_80194C50
@L_80194C48:
    li r3, -0x1
    b @L_80194CB4
@L_80194C50:
    lfs f0, 0x0(r3)
    lis r5, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r5)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_80194CAC
    lfs f0, 0x4(r3)
    lis r5, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r5)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_80194CAC
    lfs f0, 0x8(r3)
    lis r5, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r5)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_80194CAC
    li r3, -0x1
    b @L_80194CB4
@L_80194CAC:
    bl fn_800A3ADC
    li r3, 0x0
@L_80194CB4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
