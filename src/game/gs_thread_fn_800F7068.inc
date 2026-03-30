    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    clrlwi r31, r4, 24
    stw r30, 0x8(r1)
    clrlwi r30, r3, 16
    @L_800F7084:
    lwz r5, lbl_80478B00(r13)
    li r4, 0x0
    lhz r0, 0x0(r5)
    mtctr r0
    cmpwi r0, 0x0
    ble @L_800F70C8
    @L_800F709C:
    lwz r0, 0xc(r5)
    add r3, r0, r4
    lbz r0, 0x4(r3)
    cmplwi r0, 0x0
    beq @L_800F70C0
    lhz r0, 0x6(r3)
    cmplw r0, r30
    bne @L_800F70C0
    b @L_800F70CC
    @L_800F70C0:
    addi r4, r4, 0x16c
    bdnz @L_800F709C
    @L_800F70C8:
    li r3, 0x0
    @L_800F70CC:
    cmplwi r3, 0x0
    bne @L_800F70DC
    li r3, 0x0
    b @L_800F70F0
    @L_800F70DC:
    cmplwi r31, 0x0
    beq @L_800F70EC
    bl fn_800F0308
    b @L_800F7084
    @L_800F70EC:
    li r3, 0x1
    @L_800F70F0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
