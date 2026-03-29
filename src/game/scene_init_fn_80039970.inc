    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r4
    lwz r4, lbl_8047A4B8(r13)
    cmpwi r4, 0x0
    blt @L_800399F4
    lwz r3, lbl_8047A4A8(r13)
    lwz r0, lbl_8047A4BC(r13)
    subf r3, r3, r4
    mulli r3, r3, 0x1f
    cmpwi r0, 0x0
    addi r31, r3, 0x97
    beq @L_800399C4
    lfs f0, lbl_8047A4C0(r13)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    subf r31, r0, r31
    @L_800399C4:
    lha r0, 0x56(r30)
    add r0, r31, r0
    cmpwi r0, 0x97
    blt @L_800399DC
    cmpwi r31, 0x18f
    blt @L_800399E4
    @L_800399DC:
    li r4, 0x0
    b @L_800399E8
    @L_800399E4:
    li r4, 0x1
    @L_800399E8:
    mr r3, r30
    bl fn_80109220
    b @L_80039A2C
    @L_800399F4:
    lwz r3, lbl_8047A4AC(r13)
    lwz r0, lbl_8047A4BC(r13)
    mulli r3, r3, 0x1f
    cmpwi r0, 0x0
    addi r31, r3, 0x97
    bne @L_80039A20
    lfs f0, lbl_8047A4C0(r13)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    add r31, r31, r0
    @L_80039A20:
    mr r3, r30
    li r4, 0x1
    bl fn_80109220
    @L_80039A2C:
    extsh r0, r31
    li r3, 0x0
    sth r0, 0x52(r30)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
