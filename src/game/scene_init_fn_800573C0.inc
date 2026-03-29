    nofralloc
    lis r3, lbl_803A9768@ha
    lfs f0, lbl_8047BF00(r2)
    addi r3, r3, lbl_803A9768@l
    lfs f1, 0x288(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_800573F8
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_800573F0
    cmpwi r0, 0x3
    bne @L_800573F8
    @L_800573F0:
    li r3, 0x0
    blr
    @L_800573F8:
    li r3, 0x1
    blr
