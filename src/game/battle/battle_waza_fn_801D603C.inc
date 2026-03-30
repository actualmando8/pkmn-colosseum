    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r3, -32698
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    addi r31, r3, 29688
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    lwz r0, 0x0(r31)
    cmpwi r0, 0x1
    beq @L_801D6078
    bge @L_801D6230
    cmpwi r0, 0x0
    bge @L_801D622C
    b @L_801D6230
@L_801D6078:
    lwz r3, 0x624(r31)
    li r0, 0x0
    addi r30, r31, 0x61c
    stb r0, 0xb(r1)
    rlwinm r5, r3, 2, 0, 29
    lwz r3, 0x10(r31)
    stb r0, 0xa(r1)
    addi r4, r1, 0x8
    lwzx r29, r30, r5
    stb r0, 0x9(r1)
    stb r0, 0x8(r1)
    bl fn_80168408
    cmplwi r29, 0x0
    beq @L_801D6104
    lis r3, -32698
    addi r3, r3, 29688
    lbz r0, 0x8(r3)
    rlwinm r0, r0, 24, 0, 1
    srawi r0, r0, 31
    extsb. r0, r0
    beq @L_801D60D4
    mr r3, r29
    bl fn_801D7B94
@L_801D60D4:
    lis r3, -32698
    lhz r0, 0x32(r29)
    addi r3, r3, 29688
    stw r0, 0x14(r3)
    bl fn_801337B0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801D6104
    li r3, 0x78
    bl fn_80102620
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801D6104
    bl fn_801D7464
@L_801D6104:
    bl fn_801D7230
    bl fn_801E11E0
    lis r5, -32698
    li r4, 0x0
    addi r5, r5, 29688
    mr r29, r3
    lbz r0, 0x8(r5)
    mr r5, r4
    rlwinm r0, r0, 26, 0, 1
    srawi r0, r0, 31
    extsb. r0, r0
    beq @L_801D6230
    lwz r0, 0x624(r31)
    rlwinm r0, r0, 2, 0, 29
    lwzx r3, r30, r0
    cmplwi r3, 0x0
    beq @L_801D6230
    lwz r6, 0x68(r3)
    cmplwi r6, 0x0
    beq @L_801D615C
    lhz r4, 0x2c(r6)
    lhz r5, 0x2e(r6)
@L_801D615C:
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801D6230
    lfs f1, -21424(r2)
    fmr f2, f1
    bl fn_800FE6A0
    cmpwi r29, 0x3
    beq @L_801D619C
    bge @L_801D6190
    cmpwi r29, 0x1
    beq @L_801D61E0
    bge @L_801D6200
    b @L_801D621C
@L_801D6190:
    cmpwi r29, 0x5
    bge @L_801D621C
    b @L_801D61BC
@L_801D619C:
    lis r4, 0xff00
    li r3, 0xc8
    addi r5, r4, 0xa0
    li r4, 0x10
    addi r6, r2, -21420
    crxor 6, 6, 6
    bl fn_800FAEF8
    b @L_801D621C
@L_801D61BC:
    lis r3, -32728
    lis r4, 0xff00
    addi r6, r3, -27536
    addi r5, r4, 0xa0
    li r3, 0xc8
    li r4, 0x10
    crxor 6, 6, 6
    bl fn_800FAEF8
    b @L_801D621C
@L_801D61E0:
    lis r4, 0xff00
    li r3, 0xc8
    addi r5, r4, 0xa0
    li r4, 0x10
    addi r6, r2, -21412
    crxor 6, 6, 6
    bl fn_800FAEF8
    b @L_801D621C
@L_801D6200:
    lis r4, 0xff00
    li r3, 0xc8
    addi r5, r4, 0xa0
    li r4, 0x10
    addi r6, r2, -21404
    crxor 6, 6, 6
    bl fn_800FAEF8
@L_801D621C:
    lfs f1, -21396(r2)
    fmr f2, f1
    bl fn_800FE6A0
    b @L_801D6230
@L_801D622C:
    bl fn_801D6A64
@L_801D6230:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
