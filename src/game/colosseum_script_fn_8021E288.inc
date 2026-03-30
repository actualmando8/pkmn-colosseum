    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x24(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r29, 0x14(r1)
    bl fn_801F54A4
    lwz r3, lbl_8047B610(r13)
    li r4, 0x0
    lbz r3, 0x1(r3)
    bl fn_801F025C
    lwz r0, lbl_8047B618(r13)
    mr r31, r3
    rlwinm. r0, r0, 0, 24, 24
    bne @L_8021E5E0
    li r4, 0x8
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021E34C
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r30, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r29, r3
    beq @L_8021E34C
    li r4, 0x2e
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021E34C
    mr r3, r29
    li r4, 0x2e
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r30
    li r5, 0x1
    bl fn_80265598
@L_8021E34C:
    mr r3, r31
    li r4, 0x5
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021E3D0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_8021E3D0
    li r4, 0x2f
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021E3D0
    mr r3, r30
    li r4, 0x2f
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
@L_8021E3D0:
    mr r3, r31
    li r4, 0x7
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021E454
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_8021E454
    li r4, 0x30
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021E454
    mr r3, r30
    li r4, 0x30
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
@L_8021E454:
    mr r3, r31
    li r4, 0x6
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021E4D8
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_8021E4D8
    li r4, 0x31
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021E4D8
    mr r3, r30
    li r4, 0x31
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
@L_8021E4D8:
    mr r3, r31
    li r4, 0x3
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021E55C
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_8021E55C
    li r4, 0x32
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021E55C
    mr r3, r30
    li r4, 0x32
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
@L_8021E55C:
    mr r3, r31
    li r4, 0x4
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021E5E0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_8021E5E0
    li r4, 0x33
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021E5E0
    mr r3, r30
    li r4, 0x33
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
@L_8021E5E0:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x2
    stw r0, lbl_8047B610(r13)
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
