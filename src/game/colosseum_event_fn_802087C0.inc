    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r22, 0x18(r1)
    mr r30, r4
    mr r22, r5
    mr r28, r6
    mr r29, r3
    mr r23, r7
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r31, r3
    beq @L_80208C04
    mr r4, r22
    li r3, 0x0
    li r5, 0x17
    li r6, 0x0
    bl fn_80142CF4
    mr r24, r3
    mr r4, r22
    li r3, 0x0
    li r5, 0x13
    li r6, 0x0
    bl fn_80142CF4
    mr r25, r3
    mr r4, r22
    li r3, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_80142CF4
    mr r26, r3
    mr r4, r22
    li r3, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_80142CF4
    mr r27, r3
    mr r4, r22
    li r3, 0x0
    li r5, 0x15
    li r6, 0x0
    bl fn_80142CF4
    rlwinm. r0, r28, 0, 24, 31
    mr r28, r3
    bne @L_80208900
    mr r3, r31
    rlwinm r4, r24, 0, 16, 31
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    mr r3, r31
    rlwinm r4, r25, 0, 16, 31
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    mr r3, r31
    rlwinm r4, r26, 0, 16, 31
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    mr r3, r31
    rlwinm r4, r27, 0, 16, 31
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    mr r3, r31
    rlwinm r4, r28, 0, 16, 31
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    cmplwi r23, 0x0
    beq @L_80208C04
    mr r3, r31
    bl fn_801DA354
    stb r3, 0x0(r23)
    mr r3, r31
    bl fn_801DA2C4
    b @L_80208C04
@L_80208900:
    cmplwi r0, 0x1
    bne @L_802089F4
    mr r3, r31
    rlwinm r4, r24, 0, 16, 31
    li r5, 0x4
    bl fn_801DA9E8
@L_80208918:
    mr r3, r31
    rlwinm r4, r24, 0, 16, 31
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80208938
    bl fn_800F0308
    b @L_80208918
@L_80208938:
    mr r3, r31
    rlwinm r4, r25, 0, 16, 31
    li r5, 0x4
    bl fn_801DA9E8
@L_80208948:
    mr r3, r31
    rlwinm r4, r25, 0, 16, 31
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80208968
    bl fn_800F0308
    b @L_80208948
@L_80208968:
    li r24, 0x0
    rlwinm r29, r30, 0, 24, 31
@L_80208970:
    mr r3, r31
    rlwinm r4, r26, 0, 16, 31
    li r5, 0x4
    bl fn_801DA9E8
@L_80208980:
    mr r3, r31
    rlwinm r4, r26, 0, 16, 31
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802089A0
    bl fn_800F0308
    b @L_80208980
@L_802089A0:
    addi r24, r24, 0x1
    rlwinm r0, r24, 0, 24, 31
    cmplwi r0, 0x3
    bge @L_802089B8
    cmplw r0, r29
    blt @L_80208970
@L_802089B8:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x4
    bge @L_80208C04
    mr r3, r31
    rlwinm r4, r27, 0, 16, 31
    li r5, 0x4
    bl fn_801DA9E8
@L_802089D4:
    mr r3, r31
    rlwinm r4, r27, 0, 16, 31
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80208C04
    bl fn_800F0308
    b @L_802089D4
@L_802089F4:
    cmplwi r0, 0x2
    bne @L_80208A1C
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x4
    bge @L_80208C04
    mr r3, r31
    rlwinm r4, r28, 0, 16, 31
    li r5, 0x4
    bl fn_801DA9E8
    b @L_80208C04
@L_80208A1C:
    cmplwi r0, 0x3
    bne @L_80208A64
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x4
    bge @L_80208C04
    cmplwi r23, 0x0
    beq @L_80208A44
    lbz r4, 0x0(r23)
    mr r3, r31
    bl fn_801DA224
@L_80208A44:
    mr r3, r31
    rlwinm r4, r28, 0, 16, 31
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80208C04
    bl fn_800F0308
    b @L_80208A44
@L_80208A64:
    cmplwi r0, 0x4
    bne @L_80208C04
    mr r3, r31
    rlwinm r4, r24, 0, 16, 31
    li r5, 0x4
    bl fn_801DA8C4
    mr r3, r31
    rlwinm r4, r25, 0, 16, 31
    li r5, 0x4
    bl fn_801DA8C4
    mr r3, r31
    rlwinm r4, r26, 0, 16, 31
    li r5, 0x4
    bl fn_801DA8C4
    mr r3, r31
    rlwinm r4, r27, 0, 16, 31
    li r5, 0x4
    bl fn_801DA8C4
    mr r3, r31
    rlwinm r4, r28, 0, 16, 31
    li r5, 0x4
    bl fn_801DA8C4
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_80208BF0
    cmplwi r29, 0x0
    bne @L_80208AD8
    li r3, 0x0
    b @L_80208B0C
@L_80208AD8:
    mr r3, r29
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80208AFC
    li r3, 0x0
    b @L_80208B0C
@L_80208AFC:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80208B0C:
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
    beq @L_80208B60
    li r0, 0x0
    lis r4, -32736
    stw r3, 0xc(r1)
    addi r4, r4, 32604
    addi r5, r1, 0x8
    li r3, 0x0
    stw r29, 0x8(r1)
    li r6, 0x0
    stw r0, 0x10(r1)
    bl fn_801F37B0
@L_80208B60:
    mr r3, r29
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r24, r3
    beq @L_80208BC8
    mr r3, r29
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_80208BA0
    li r4, 0x0
    bl fn_801DA4E8
@L_80208BA0:
    mr r3, r29
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r24
    bl fn_801C3C98
    mr r3, r24
    bl fn_801DB100
@L_80208BC8:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r4, r3, 0, 16, 31
    mr r3, r29
    li r5, 0x1
    bl fn_8026532C
    b @L_80208C04
@L_80208BF0:
    cmplwi r23, 0x0
    beq @L_80208C04
    lbz r4, 0x0(r23)
    mr r3, r31
    bl fn_801DA224
@L_80208C04:
    lmw r22, 0x18(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
