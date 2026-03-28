    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    mulli r7, r5, 0x48
    lis r5, lbl_803A9F08@ha
    addi r5, r5, lbl_803A9F08@l
    mulli r0, r6, 0xc
    add r5, r5, r7
    add r5, r5, r0
    addi r5, r5, 0x30
    lbz r0, 0x0(r5)
    cmplwi r0, 0x0
    beq @L_80069A4C
    lhz r5, 0x2(r5)
    bl fn_8010B9E8
    li r3, 0x1
    b @L_80069A50
@L_80069A4C:
    li r3, 0x0
@L_80069A50:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
