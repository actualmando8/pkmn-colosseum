    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r3, -32698
    lis r4, -32728
    stw r0, 0x34(r1)
    li r0, 0x0
    stmw r24, 0x10(r1)
    addi r31, r3, 29688
    addi r30, r4, -27848
    addi r4, r1, 0x8
    addi r27, r31, 0x61c
    lwz r3, 0x624(r31)
    stb r0, 0xb(r1)
    rlwinm r5, r3, 2, 0, 29
    lwz r3, 0x10(r31)
    stb r0, 0xa(r1)
    lwzx r26, r27, r5
    stb r0, 0x9(r1)
    stb r0, 0x8(r1)
    bl fn_80168408
    cmplwi r26, 0x0
    beq @L_801D6B44
    lis r3, -32698
    addi r28, r3, 29688
    lbz r0, 0x8(r28)
    rlwinm r0, r0, 24, 0, 1
    srawi r0, r0, 31
    extsb. r0, r0
    beq @L_801D6AE0
    mr r3, r26
    bl fn_801D7B94
@L_801D6AE0:
    lbz r0, 0x8(r28)
    lwz r3, 0x68(r26)
    rlwinm r0, r0, 25, 0, 1
    srawi r0, r0, 31
    cmplwi r3, 0x0
    rlwinm r4, r0, 0, 24, 31
    beq @L_801D6B0C
    lbz r0, 0x14(r3)
    cmplwi r0, 0x0
    bne @L_801D6B0C
    stb r4, 0x16(r3)
@L_801D6B0C:
    mr r3, r26
    bl fn_801D6E58
    lis r3, -32698
    lhz r0, 0x32(r26)
    addi r3, r3, 29688
    stw r0, 0x14(r3)
    bl fn_801337B0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801D6B44
    li r3, 0x78
    bl fn_80102620
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801D6B44
    bl fn_801D7464
@L_801D6B44:
    bl fn_801D7230
    bl fn_801E11E0
    lis r5, -32698
    li r4, 0x0
    addi r5, r5, 29688
    mr r26, r3
    lbz r0, 0x8(r5)
    mr r5, r4
    rlwinm r0, r0, 26, 0, 1
    srawi r0, r0, 31
    extsb. r0, r0
    beq @L_801D6C64
    lwz r0, 0x624(r31)
    rlwinm r0, r0, 2, 0, 29
    lwzx r3, r27, r0
    cmplwi r3, 0x0
    beq @L_801D6C64
    lwz r6, 0x68(r3)
    cmplwi r6, 0x0
    beq @L_801D6B9C
    lhz r4, 0x2c(r6)
    lhz r5, 0x2e(r6)
@L_801D6B9C:
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801D6C64
    lfs f1, -21424(r2)
    fmr f2, f1
    bl fn_800FE6A0
    cmpwi r26, 0x3
    beq @L_801D6BDC
    bge @L_801D6BD0
    cmpwi r26, 0x1
    beq @L_801D6C1C
    bge @L_801D6C3C
    b @L_801D6C58
@L_801D6BD0:
    cmpwi r26, 0x5
    bge @L_801D6C58
    b @L_801D6BFC
@L_801D6BDC:
    lis r4, 0xff00
    li r3, 0xc8
    addi r5, r4, 0xa0
    li r4, 0x10
    addi r6, r2, -21420
    crxor 6, 6, 6
    bl fn_800FAEF8
    b @L_801D6C58
@L_801D6BFC:
    lis r3, 0xff00
    addi r6, r30, 0x138
    addi r5, r3, 0xa0
    li r3, 0xc8
    li r4, 0x10
    crxor 6, 6, 6
    bl fn_800FAEF8
    b @L_801D6C58
@L_801D6C1C:
    lis r4, 0xff00
    li r3, 0xc8
    addi r5, r4, 0xa0
    li r4, 0x10
    addi r6, r2, -21412
    crxor 6, 6, 6
    bl fn_800FAEF8
    b @L_801D6C58
@L_801D6C3C:
    lis r4, 0xff00
    li r3, 0xc8
    addi r5, r4, 0xa0
    li r4, 0x10
    addi r6, r2, -21404
    crxor 6, 6, 6
    bl fn_800FAEF8
@L_801D6C58:
    lfs f1, -21396(r2)
    fmr f2, f1
    bl fn_800FE6A0
@L_801D6C64:
    bl fn_801E11E0
    cmpwi r3, 0x1
    bne @L_801D6E44
    lis r3, -32698
    addi r31, r3, 29688
    lwz r0, 0x870(r31)
    cmpwi r0, 0x0
    bgt @L_801D6E34
    addi r26, r31, 0x670
    mr r3, r26
    bl fn_80167F28
    mr. r25, r3
    bne @L_801D6CB0
    mr r4, r26
    addi r3, r30, 0x6c
    crxor 6, 6, 6
    bl fn_800DD970
    li r27, 0x0
    b @L_801D6CC0
@L_801D6CB0:
    bl fn_80167E5C
    mr r27, r3
    mr r3, r25
    bl fn_80167E64
@L_801D6CC0:
    cmplwi r27, 0x0
    beq @L_801D6E28
    addi r0, r27, 0x1f
    li r4, 0x20
    rlwinm r24, r0, 0, 0, 26
    mr r3, r24
    bl fn_800E2C04
    rlwinm. r0, r3, 0, 16, 31
    mr r28, r3
    beq @L_801D6E28
    bl fn_800E27B0
    mr r29, r3
    mr r3, r26
    bl fn_80167F28
    mr. r25, r3
    bne @L_801D6D14
    mr r4, r26
    addi r3, r30, 0x9c
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801D6D50
@L_801D6D14:
    mr r4, r29
    mr r5, r24
    li r6, 0x0
    bl fn_80167ED0
    cmpwi r3, 0x0
    bge @L_801D6D48
    mr r4, r26
    addi r3, r30, 0xc8
    crxor 6, 6, 6
    bl fn_800DD970
    mr r3, r25
    bl fn_80167E64
    b @L_801D6D50
@L_801D6D48:
    mr r3, r25
    bl fn_80167E64
@L_801D6D50:
    lwz r0, 0x0(r29)
    cmpwi r0, 0x1
    bne @L_801D6E18
    lis r3, -32698
    addi r4, r29, 0x208
    addi r3, r3, 29688
    li r5, 0x200
    addi r3, r3, 0x1c
    bl memcpy
    lis r3, -32698
    addi r4, r29, 0x408
    addi r3, r3, 29688
    li r5, 0x200
    addi r3, r3, 0x21c
    bl memcpy
    lis r3, -32698
    addi r4, r29, 0x8
    addi r3, r3, 29688
    li r5, 0x200
    addi r3, r3, 0x41c
    bl memcpy
    lis r3, -32698
    lwz r4, 0x608(r29)
    addi r3, r3, 29688
    li r0, 0x0
    stb r4, 0x87c(r3)
    stw r0, 0x4(r29)
    stw r0, 0x0(r29)
    bl fn_801D56B0
    mr r3, r26
    bl fn_80167F28
    mr. r25, r3
    beq @L_801D6E08
    mr r4, r29
    mr r5, r27
    li r6, 0x0
    bl fn_80167E54
    cmpwi r3, 0x0
    bge @L_801D6DFC
    mr r4, r26
    addi r3, r30, 0xf8
    crxor 6, 6, 6
    bl fn_800DD970
@L_801D6DFC:
    mr r3, r25
    bl fn_80167E64
    b @L_801D6E18
@L_801D6E08:
    mr r4, r26
    addi r3, r30, 0x9c
    crxor 6, 6, 6
    bl fn_800DD970
@L_801D6E18:
    mr r3, r28
    bl fn_800E24B0
    mr r3, r28
    bl fn_800E209C
@L_801D6E28:
    li r0, 0x3c
    stw r0, 0x870(r31)
    b @L_801D6E44
@L_801D6E34:
    bl fn_800D3088
    lwz r0, 0x870(r31)
    subf r0, r3, r0
    stw r0, 0x870(r31)
@L_801D6E44:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
