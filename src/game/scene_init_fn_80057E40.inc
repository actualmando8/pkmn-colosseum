    nofralloc
    lis r3, lbl_803A9A08@ha
    addi r3, r3, lbl_803A9A08@l
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_80057E68
    lwz r3, 0x4(r3)
    neg r0, r3
    or r0, r0, r3
    srwi r3, r0, 31
    blr
    @L_80057E68:
    li r3, 0x2
    blr
