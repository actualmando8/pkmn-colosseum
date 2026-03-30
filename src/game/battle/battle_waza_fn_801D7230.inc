    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r3, -32698
    lis r4, -32728
    stw r0, 0x24(r1)
    addi r3, r3, 29688
    stw r31, 0x1c(r1)
    addi r31, r4, -27848
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    li r29, 0x0
    lwz r30, 0xc(r3)
    cmpwi r30, 0x0
    beq @L_801D7430
    lfs f1, -21424(r2)
    fmr f2, f1
    bl fn_800FE6A0
    rlwinm. r0, r30, 0, 31, 31
    beq @L_801D7298
    addi r6, r31, 0x150
    li r3, 0xa
    li r4, 0x10
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_800FAEF8
    li r29, 0x1
@L_801D7298:
    rlwinm. r0, r30, 0, 30, 30
    beq @L_801D72C0
    mulli r4, r29, 0xd
    addi r6, r31, 0x150
    li r3, 0xa
    li r5, -0x1
    addi r4, r4, 0x10
    crxor 6, 6, 6
    bl fn_800FAEF8
    addi r29, r29, 0x1
@L_801D72C0:
    rlwinm. r0, r30, 0, 29, 29
    beq @L_801D72E8
    mulli r4, r29, 0xd
    addi r6, r31, 0x16c
    li r3, 0xa
    li r5, -0x1
    addi r4, r4, 0x10
    crxor 6, 6, 6
    bl fn_800FAEF8
    addi r29, r29, 0x1
@L_801D72E8:
    rlwinm. r0, r30, 0, 28, 28
    beq @L_801D7310
    mulli r4, r29, 0xd
    addi r6, r31, 0x184
    li r3, 0xa
    li r5, -0x1
    addi r4, r4, 0x10
    crxor 6, 6, 6
    bl fn_800FAEF8
    addi r29, r29, 0x1
@L_801D7310:
    rlwinm. r0, r30, 0, 27, 27
    beq @L_801D7338
    mulli r4, r29, 0xd
    addi r6, r31, 0x19c
    li r3, 0xa
    li r5, -0x1
    addi r4, r4, 0x10
    crxor 6, 6, 6
    bl fn_800FAEF8
    addi r29, r29, 0x1
@L_801D7338:
    rlwinm. r0, r30, 0, 26, 26
    beq @L_801D7360
    mulli r4, r29, 0xd
    addi r6, r31, 0x1b0
    li r3, 0xa
    li r5, -0x1
    addi r4, r4, 0x10
    crxor 6, 6, 6
    bl fn_800FAEF8
    addi r29, r29, 0x1
@L_801D7360:
    rlwinm. r0, r30, 0, 25, 25
    beq @L_801D7388
    mulli r4, r29, 0xd
    addi r6, r31, 0x1c8
    li r3, 0xa
    li r5, -0x1
    addi r4, r4, 0x10
    crxor 6, 6, 6
    bl fn_800FAEF8
    addi r29, r29, 0x1
@L_801D7388:
    rlwinm. r0, r30, 0, 24, 24
    beq @L_801D73B0
    mulli r4, r29, 0xd
    addi r6, r31, 0x1dc
    li r3, 0xa
    li r5, -0x1
    addi r4, r4, 0x10
    crxor 6, 6, 6
    bl fn_800FAEF8
    addi r29, r29, 0x1
@L_801D73B0:
    rlwinm. r0, r30, 0, 23, 23
    beq @L_801D73D8
    mulli r4, r29, 0xd
    addi r6, r31, 0x1f4
    li r3, 0xa
    li r5, -0x1
    addi r4, r4, 0x10
    crxor 6, 6, 6
    bl fn_800FAEF8
    addi r29, r29, 0x1
@L_801D73D8:
    rlwinm. r0, r30, 0, 22, 22
    beq @L_801D7400
    mulli r4, r29, 0xd
    addi r6, r31, 0x208
    li r3, 0xa
    li r5, -0x1
    addi r4, r4, 0x10
    crxor 6, 6, 6
    bl fn_800FAEF8
    addi r29, r29, 0x1
@L_801D7400:
    rlwinm. r0, r30, 0, 21, 21
    beq @L_801D7424
    mulli r4, r29, 0xd
    addi r6, r31, 0x228
    li r3, 0xa
    li r5, -0x1
    addi r4, r4, 0x10
    crxor 6, 6, 6
    bl fn_800FAEF8
@L_801D7424:
    lfs f1, -21396(r2)
    fmr f2, f1
    bl fn_800FE6A0
@L_801D7430:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
