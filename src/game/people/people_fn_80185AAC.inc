    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r3
    bl fn_8018FCBC
    mr r5, r3
    addi r3, r1, 0x8
    addi r4, r30, 0x5c
    bl fn_800E0168
    addi r3, r1, 0x8
    bl fn_800E008C
    fmr f31, f1
    lwz r3, 0x28(r30)
    lwz r4, 0x2c(r30)
    lfs f1, 0x58(r30)
    bl fn_80188214
    clrlwi. r0, r3, 24
    bne @L_80185B0C
    li r3, 0x2
    b @L_80185B70
    @L_80185B0C:
    mr r3, r30
    bl fn_8018FCBC
    mr r5, r3
    addi r3, r1, 0x8
    addi r4, r30, 0x5c
    bl fn_800E0168
    addi r3, r1, 0x8
    bl fn_800E008C
    fcmpo cr0, f1, f31
    ble @L_80185B6C
    mr r3, r30
    addi r4, r30, 0x5c
    bl fn_8018FC74
    mr r3, r30
    bl fn_8018FC00
    mr r31, r3
    mr r3, r30
    bl fn_8018FCBC
    mr r4, r3
    mr r3, r30
    mr r5, r31
    bl fn_8018E9B4
    li r3, 0x1
    b @L_80185B70
    @L_80185B6C:
    li r3, 0x0
    @L_80185B70:
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
