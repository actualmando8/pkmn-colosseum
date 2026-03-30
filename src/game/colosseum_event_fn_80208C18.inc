    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r5, 0xee
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r27, r4
    mr r26, r3
    li r4, 0x0
    bl fn_8012640C
    mr. r31, r3
    beq @L_80208EBC
    cmplwi r26, 0x0
    bne @L_80208C58
    li r28, 0x0
    b @L_80208C90
@L_80208C58:
    mr r3, r26
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80208C7C
    li r3, 0x0
    b @L_80208C8C
@L_80208C7C:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80208C8C:
    mr r28, r3
@L_80208C90:
    mr r3, r28
    li r4, 0x0
    li r5, 0x73
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 24, 31
    li r3, 0x0
    mr r30, r4
    li r5, 0xe
    li r6, 0x0
    bl fn_80142CF4
    mr r29, r3
    mr r4, r30
    li r3, 0x0
    li r5, 0xf
    li r6, 0x0
    bl fn_80142CF4
    rlwinm. r0, r27, 0, 24, 31
    mr r30, r3
    bne @L_80208D20
    mr r3, r31
    rlwinm r4, r29, 0, 16, 31
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    mr r3, r31
    rlwinm r4, r30, 0, 16, 31
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    mr r3, r31
    li r4, 0x67
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    b @L_80208EBC
@L_80208D20:
    cmplwi r0, 0x1
    bne @L_80208D3C
    mr r3, r31
    rlwinm r4, r29, 0, 16, 31
    li r5, 0x4
    bl fn_801DA9E8
    b @L_80208EBC
@L_80208D3C:
    cmplwi r0, 0x2
    bne @L_80208D64
@L_80208D44:
    mr r3, r31
    rlwinm r4, r29, 0, 16, 31
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80208EBC
    bl fn_800F0308
    b @L_80208D44
@L_80208D64:
    cmplwi r0, 0x3
    bne @L_80208D80
    mr r3, r31
    rlwinm r4, r30, 0, 16, 31
    li r5, 0x4
    bl fn_801DA9E8
    b @L_80208EBC
@L_80208D80:
    cmplwi r0, 0x4
    bne @L_80208E84
@L_80208D88:
    li r3, 0x0
    bl fn_801DA5C4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80208DA4
    bl fn_800F0308
    b @L_80208D88
@L_80208DA4:
    cmplwi r26, 0x0
    bne @L_80208DB4
    li r3, 0x0
    b @L_80208DE8
@L_80208DB4:
    mr r3, r26
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80208DD8
    li r3, 0x0
    b @L_80208DE8
@L_80208DD8:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80208DE8:
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x61
    li r6, 0x0
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
@L_80208E20:
    mr r3, r31
    rlwinm r4, r30, 0, 16, 31
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80208E40
    bl fn_800F0308
    b @L_80208E20
@L_80208E40:
    mr r3, r28
    bl fn_80125390
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80208EBC
    mr r3, r31
    li r4, 0x67
    li r5, 0x4
    bl fn_801DA9E8
@L_80208E64:
    mr r3, r31
    li r4, 0x67
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80208EBC
    bl fn_800F0308
    b @L_80208E64
@L_80208E84:
    cmplwi r0, 0x5
    bne @L_80208EBC
    mr r3, r31
    rlwinm r4, r29, 0, 16, 31
    li r5, 0x4
    bl fn_801DA8C4
    mr r3, r31
    rlwinm r4, r30, 0, 16, 31
    li r5, 0x4
    bl fn_801DA8C4
    mr r3, r31
    li r4, 0x67
    li r5, 0x4
    bl fn_801DA8C4
@L_80208EBC:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
