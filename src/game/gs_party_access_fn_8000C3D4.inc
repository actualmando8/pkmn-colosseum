    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    cmpwi r4, 0x2
    beq @L_8000C460
    bge @L_8000C3FC
    cmpwi r4, 0x0
    beq @L_8000C408
    bge @L_8000C44C
    b @L_8000C48C
    @L_8000C3FC:
    cmpwi r4, 0x4
    bge @L_8000C48C
    b @L_8000C480
    @L_8000C408:
    lfs f1, lbl_8047B6E0(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0x44c5
    li r4, 0x1
    li r5, 0x0
    bl fn_80105FF8
    li r3, 0x1
    bl fn_801069FC
    lfs f1, lbl_8047B6E0(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    b @L_8000C48C
    @L_8000C44C:
    li r3, 0x1
    bl fn_80106080
    li r3, 0x1
    bl fn_80105FB0
    b @L_8000C48C
    @L_8000C460:
    bl fn_800F0308
    bl fn_800F0308
    bl fn_800F0308
    li r3, 0x0
    bl fn_80106080
    bl fn_800F0308
    bl fn_800F0308
    b @L_8000C48C
    @L_8000C480:
    li r3, 0x31
    li r4, 0x7da
    bl fn_80132A38
    @L_8000C48C:
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
