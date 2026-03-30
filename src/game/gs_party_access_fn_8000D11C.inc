    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r3, lbl_804673F8@ha
    addi r3, r3, lbl_804673F8@l
    lwz r0, 0x874(r3)
    cmpwi r0, 0x0
    beq @L_8000D140
    bl fn_800FF660
    @L_8000D140:
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
