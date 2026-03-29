    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r31, r3
    lbz r0, 0x1(r31)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_8003A07C
    bge @L_80039FA4
    cmpwi r0, 0x0
    beq @L_80039FB0
    b @L_8003A0F4
    @L_80039FA4:
    cmpwi r0, 0x4
    bge @L_8003A0F4
    b @L_8003A0D0
    @L_80039FB0:
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8003A0F4
    li r3, 0x25
    li r4, 0xb4
    bl fn_801080CC
    lwz r4, lbl_8047A4A8(r13)
    li r3, 0x0
    lwz r0, lbl_8047A4AC(r13)
    add r27, r4, r0
    bl fn_801347D0
    clrlwi r26, r3, 16
    li r30, -0x1
    li r29, 0x0
    b @L_8003A02C
    @L_80039FF0:
    extsh r4, r29
    li r3, 0x0
    bl fn_80134768
    mr r28, r3
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8003A028
    addi r30, r30, 0x1
    cmpw r30, r27
    blt @L_8003A028
    mr r3, r28
    bl fn_80143C68
    b @L_8003A038
    @L_8003A028:
    addi r29, r29, 0x1
    @L_8003A02C:
    cmpw r29, r26
    blt @L_80039FF0
    li r3, 0x0
    @L_8003A038:
    clrlwi r3, r3, 16
    cmplwi r3, 0x0
    beq @L_8003A050
    bl fn_801440A0
    bl fn_80143F84
    b @L_8003A054
    @L_8003A050:
    li r3, 0x1b68
    @L_8003A054:
    lfs f0, lbl_8047BAB0(r2)
    li r5, 0x0
    li r4, -0x1
    stw r3, lbl_8047A4B4(r13)
    li r0, 0x1
    stfs f0, lbl_8047A4C0(r13)
    stw r5, lbl_8047A4BC(r13)
    stw r4, lbl_8047A4B8(r13)
    stb r0, 0x2(r31)
    b @L_8003A0F4
    @L_8003A07C:
    lfs f2, lbl_8047A4C0(r13)
    lfs f1, lbl_8047BAB0(r2)
    fcmpo cr0, f2, f1
    ble @L_8003A0A4
    lfs f0, lbl_8047BABC(r2)
    fsubs f0, f2, f0
    fcmpo cr0, f0, f1
    stfs f0, lbl_8047A4C0(r13)
    bge @L_8003A0A4
    stfs f1, lbl_8047A4C0(r13)
    @L_8003A0A4:
    lfs f2, lbl_8047A4C0(r13)
    lfs f1, lbl_8047BAB0(r2)
    fcmpo cr0, f2, f1
    bge @L_8003A0F4
    lfs f0, lbl_8047BABC(r2)
    fadds f0, f2, f0
    fcmpo cr0, f0, f1
    stfs f0, lbl_8047A4C0(r13)
    ble @L_8003A0F4
    stfs f1, lbl_8047A4C0(r13)
    b @L_8003A0F4
    @L_8003A0D0:
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8003A0F4
    li r3, 0x25
    li r4, 0xb8
    bl fn_801080CC
    li r0, 0x1
    stb r0, 0x2(r31)
    @L_8003A0F4:
    li r3, 0x0
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
