    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    bne @L_80194710
    lfs f1, -23848(r2)
    b @L_80194774
@L_80194710:
    lbz r0, 0x50(r31)
    cmpwi r0, 0x2
    beq @L_80194760
    bge @L_8019472C
    cmpwi r0, 0x1
    bge @L_80194738
    b @L_80194770
@L_8019472C:
    cmpwi r0, 0x4
    bge @L_80194770
    b @L_80194768
@L_80194738:
    lfs f1, -23840(r2)
    lfs f0, 0x40(r31)
    lfs f2, -23844(r2)
    fmuls f0, f1, f0
    fmuls f1, f2, f0
    bl fn_800CE220
    frsp f1, f1
    lfs f0, 0x38(r31)
    fmuls f1, f0, f1
    b @L_80194774
@L_80194760:
    lfs f1, 0x40(r31)
    b @L_80194774
@L_80194768:
    lfs f1, 0x40(r31)
    b @L_80194774
@L_80194770:
    lfs f1, -23848(r2)
@L_80194774:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
