    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lfs f0, lbl_8047CA30(r2)
    stw r0, 0x14(r1)
    fcmpo cr0, f1, f0
    blt @L_800D56B0
    lfs f0, lbl_8047CA34(r2)
    fcmpo cr0, f1, f0
    ble @L_800D5670
    b @L_800D56B0
    @L_800D5670:
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x1
    bne @L_800D5694
    li r3, 0x24
    li r4, 0xb
    creqv 6, 6, 6
    bl fn_800D4F98
    b @L_800D56B0
    @L_800D5694:
    lfs f0, lbl_8047CA38(r2)
    li r4, 0x0
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r3, 0xc(r1)
    bl fn_800B9404
    @L_800D56B0:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
