    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r5, 0xee
    li r6, 0x0
    stw r0, 0x34(r1)
    stmw r29, 0x24(r1)
    mr r30, r4
    mr r31, r3
    li r4, 0x0
    bl fn_8012640C
    mr. r29, r3
    beq @L_80209118
    cmplwi r31, 0x0
    bne @L_80208F10
    li r3, 0x0
    b @L_80208F44
@L_80208F10:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80208F34
    li r3, 0x0
    b @L_80208F44
@L_80208F34:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80208F44:
    li r4, 0x0
    li r5, 0x73
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 24, 31
    li r3, 0x0
    li r5, 0xd
    li r6, 0x0
    bl fn_80142CF4
    rlwinm. r0, r30, 0, 24, 31
    mr r30, r3
    bne @L_80208F8C
    mr r3, r29
    rlwinm r4, r30, 0, 16, 31
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    b @L_80209118
@L_80208F8C:
    cmplwi r0, 0x1
    bne @L_80208FA8
    mr r3, r29
    rlwinm r4, r30, 0, 16, 31
    li r5, 0x4
    bl fn_801DA9E8
    b @L_80209118
@L_80208FA8:
    cmplwi r0, 0x2
    bne @L_80208FD0
@L_80208FB0:
    mr r3, r29
    rlwinm r4, r30, 0, 16, 31
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80209118
    bl fn_800F0308
    b @L_80208FB0
@L_80208FD0:
    cmplwi r0, 0x3
    bne @L_80208FEC
    mr r3, r29
    rlwinm r4, r30, 0, 16, 31
    li r5, 0x4
    bl fn_801DA8C4
    b @L_80209118
@L_80208FEC:
    cmplwi r0, 0x4
    bne @L_80209118
    cmplwi r31, 0x0
    bne @L_80209004
    li r3, 0x0
    b @L_80209038
@L_80209004:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80209028
    li r3, 0x0
    b @L_80209038
@L_80209028:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80209038:
    li r4, 0x0
    li r5, 0x73
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 24, 31
    li r3, 0x0
    li r5, 0x10
    li r6, 0x0
    bl fn_80142CF4
    cmplwi r3, 0x0
    beq @L_8020908C
    li r0, 0x0
    lis r4, -32736
    stw r3, 0xc(r1)
    addi r4, r4, 32604
    addi r5, r1, 0x8
    li r3, 0x0
    stw r31, 0x8(r1)
    li r6, 0x0
    stw r0, 0x10(r1)
    bl fn_801F37B0
@L_8020908C:
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_802090F4
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_802090CC
    li r4, 0x0
    bl fn_801DA4E8
@L_802090CC:
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r30
    bl fn_801C3C98
    mr r3, r30
    bl fn_801DB100
@L_802090F4:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r4, r3, 0, 16, 31
    mr r3, r31
    li r5, 0x1
    bl fn_8026532C
@L_80209118:
    lmw r29, 0x24(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
