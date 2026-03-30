    nofralloc
    cmpwi r3, 0x0
    blt @L_801A67B4
    cmpwi r3, 0x8
    bge @L_801A67B4
    lis r4, lbl_804655E0@ha
    slwi r3, r3, 2
    addi r0, r4, lbl_804655E0@l
    add r3, r0, r3
    lwz r3, 0x0(r3)
    blr
    @L_801A67B4:
    li r3, 0x0
    blr
