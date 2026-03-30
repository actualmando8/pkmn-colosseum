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
    beq @L_8020936C
    cmplwi r31, 0x0
    bne @L_8020916C
    li r3, 0x0
    b @L_802091A0
@L_8020916C:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80209190
    li r3, 0x0
    b @L_802091A0
@L_80209190:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_802091A0:
    li r4, 0x0
    li r5, 0x73
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 24, 31
    li r3, 0x0
    li r5, 0x10
    li r6, 0x0
    bl fn_80142CF4
    rlwinm. r0, r30, 0, 24, 31
    mr r30, r3
    bne @L_802091E8
    mr r3, r29
    rlwinm r4, r30, 0, 16, 31
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    b @L_8020936C
@L_802091E8:
    cmplwi r0, 0x1
    bne @L_80209204
    mr r3, r29
    rlwinm r4, r30, 0, 16, 31
    li r5, 0x4
    bl fn_801DA9E8
    b @L_8020936C
@L_80209204:
    cmplwi r0, 0x2
    bne @L_80209264
@L_8020920C:
    mr r3, r29
    rlwinm r4, r30, 0, 16, 31
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020922C
    bl fn_800F0308
    b @L_8020920C
@L_8020922C:
    mr r3, r29
    rlwinm r4, r30, 0, 16, 31
    li r5, 0x4
    bl fn_801DA8C4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r4, r3, 0, 16, 31
    mr r3, r31
    li r5, 0x1
    bl fn_8026532C
    b @L_8020936C
@L_80209264:
    cmplwi r0, 0x3
    bne @L_8020936C
    cmplwi r31, 0x0
    bne @L_8020927C
    li r3, 0x0
    b @L_802092B0
@L_8020927C:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_802092A0
    li r3, 0x0
    b @L_802092B0
@L_802092A0:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_802092B0:
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
    beq @L_80209304
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
@L_80209304:
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_8020936C
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_80209344
    li r4, 0x0
    bl fn_801DA4E8
@L_80209344:
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
@L_8020936C:
    lmw r29, 0x24(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
