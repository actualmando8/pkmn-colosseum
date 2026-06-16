    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    bne @L_80194530
    lfs f1, -23848(r2)
    b @L_8019459C
@L_80194530:
    lbz r0, 0x50(r31)
    cmpwi r0, 0x2
    beq @L_80194588
    bge @L_8019454C
    cmpwi r0, 0x1
    bge @L_80194558
    b @L_80194598
@L_8019454C:
    cmpwi r0, 0x4
    bge @L_80194598
    b @L_80194590
@L_80194558:
    lfs f1, -23840(r2)
    lfs f0, 0x40(r31)
    lfs f2, -23844(r2)
    fmuls f0, f1, f0
    fmuls f1, f2, f0
    bl fn_800CE220
    frsp f1, f1
    lfs f0, 0x38(r31)
    lfs f2, 0x44(r31)
    fmuls f0, f0, f1
    fmuls f1, f2, f0
    b @L_8019459C
@L_80194588:
    lfs f1, 0x4c(r31)
    b @L_8019459C
@L_80194590:
    lfs f1, 0x4c(r31)
    b @L_8019459C
@L_80194598:
    lfs f1, -23848(r2)
@L_8019459C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
