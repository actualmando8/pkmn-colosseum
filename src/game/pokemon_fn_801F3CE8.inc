    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stmw r18, 0x28(r1)
    mr. r31, r4
    mr r26, r3
    mr r30, r5
    mr r25, r6
    beq @L_801F3D14
    cmplwi r30, 0x0
    bne @L_801F3D1C
    @L_801F3D14:
    li r3, 0x1
    b @L_801F420C
    @L_801F3D1C:
    li r0, 0x0
    li r5, 0xd
    lis r4, fn_801F3678@ha
    stw r5, 0x18(r1)
    addi r4, r4, fn_801F3678@l
    addi r5, r1, 0x18
    stw r0, 0x1c(r1)
    li r6, 0x0
    stw r0, 0x20(r1)
    stw r0, 0x24(r1)
    bl fn_801F37B0
    lwz r0, 0x1c(r1)
    clrlwi. r0, r0, 16
    beq @L_801F3D5C
    li r29, 0x0
    b @L_801F3EC4
    @L_801F3D5C:
    li r0, 0x0
    li r4, 0x4d
    lis r3, fn_801F3678@ha
    stw r4, 0x8(r1)
    addi r4, r3, fn_801F3678@l
    addi r5, r1, 0x8
    stw r0, 0xc(r1)
    mr r3, r26
    li r6, 0x0
    stw r0, 0x10(r1)
    stw r0, 0x14(r1)
    bl fn_801F37B0
    lwz r0, 0xc(r1)
    clrlwi. r0, r0, 16
    beq @L_801F3DA0
    li r29, 0x0
    b @L_801F3EC4
    @L_801F3DA0:
    mr r3, r26
    li r4, 0x0
    li r5, 0xa
    li r6, 0x4e
    bl fn_801F54A4
    cmpwi r3, 0x1
    bne @L_801F3DC4
    li r29, 0x0
    b @L_801F3EC4
    @L_801F3DC4:
    mr r3, r26
    li r4, 0x0
    li r5, 0xa
    li r6, 0x4f
    bl fn_801F54A4
    cmpwi r3, 0x1
    bne @L_801F3DE8
    li r29, 0x1
    b @L_801F3EC4
    @L_801F3DE8:
    mr r3, r26
    li r4, 0x0
    li r5, 0xa
    li r6, 0x50
    bl fn_801F54A4
    cmpwi r3, 0x1
    bne @L_801F3E0C
    li r29, 0x2
    b @L_801F3EC4
    @L_801F3E0C:
    mr r3, r26
    li r4, 0x0
    li r5, 0xa
    li r6, 0x51
    bl fn_801F54A4
    cmpwi r3, 0x1
    bne @L_801F3E30
    li r29, 0x3
    b @L_801F3EC4
    @L_801F3E30:
    mr r3, r26
    li r4, 0x0
    li r5, 0xa
    li r6, 0x52
    bl fn_801F54A4
    cmpwi r3, 0x1
    bne @L_801F3E54
    li r29, 0x4
    b @L_801F3EC4
    @L_801F3E54:
    mr r3, r26
    li r4, 0x0
    li r5, 0xa
    li r6, 0x53
    bl fn_801F54A4
    cmpwi r3, 0x1
    bne @L_801F3E78
    li r29, 0x1
    b @L_801F3EC4
    @L_801F3E78:
    mr r3, r26
    li r4, 0x0
    li r5, 0xa
    li r6, 0x54
    bl fn_801F54A4
    cmpwi r3, 0x1
    bne @L_801F3E9C
    li r29, 0x2
    b @L_801F3EC4
    @L_801F3E9C:
    mr r3, r26
    li r4, 0x0
    li r5, 0xa
    li r6, 0x55
    bl fn_801F54A4
    cmpwi r3, 0x1
    bne @L_801F3EC0
    li r29, 0x3
    b @L_801F3EC4
    @L_801F3EC0:
    li r29, 0x0
    @L_801F3EC4:
    mr r3, r26
    li r4, 0x0
    li r5, 0x1d
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r27, r3, 24
    mr r3, r26
    li r4, 0x0
    li r5, 0x5b
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r28, r3, 16
    mr r3, r31
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    mr. r23, r3
    bne @L_801F3F18
    li r24, 0x0
    b @L_801F3FD0
    @L_801F3F18:
    li r22, 0x0
    b @L_801F3FB4
    @L_801F3F20:
    mr r3, r26
    mr r6, r22
    li r4, 0x0
    li r5, 0x35
    bl fn_801F54A4
    mr. r19, r3
    beq @L_801F3FB0
    li r20, 0x0
    b @L_801F3FA4
    @L_801F3F44:
    mr r3, r19
    mr r6, r20
    li r4, 0x0
    li r5, 0x7
    bl fn_801F76B8
    mr. r24, r3
    beq @L_801F3FA0
    li r21, 0x0
    b @L_801F3F94
    @L_801F3F68:
    mr r3, r24
    mr r6, r21
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    cmplwi r3, 0x0
    beq @L_801F3F90
    cmplw r23, r3
    bne @L_801F3F90
    b @L_801F3FC4
    @L_801F3F90:
    addi r21, r21, 0x1
    @L_801F3F94:
    clrlwi r0, r21, 16
    cmplwi r0, 0x6
    blt @L_801F3F68
    @L_801F3FA0:
    addi r20, r20, 0x1
    @L_801F3FA4:
    clrlwi r0, r20, 16
    cmplwi r0, 0x2
    blt @L_801F3F44
    @L_801F3FB0:
    addi r22, r22, 0x1
    @L_801F3FB4:
    clrlwi r0, r22, 16
    cmplwi r0, 0x2
    blt @L_801F3F20
    li r24, 0x0
    @L_801F3FC4:
    cmplwi r24, 0x0
    bne @L_801F3FD0
    li r24, 0x0
    @L_801F3FD0:
    cmplwi r24, 0x0
    bne @L_801F3FE0
    li r7, 0x0
    b @L_801F4008
    @L_801F3FE0:
    mr r3, r24
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r3, 0x0
    bne @L_801F4004
    li r7, 0x0
    b @L_801F4008
    @L_801F4004:
    mr r7, r3
    @L_801F4008:
    mr r3, r31
    mr r4, r27
    mr r5, r29
    mr r6, r28
    bl fn_802043D4
    mr r24, r3
    mr r3, r30
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    mr. r23, r3
    bne @L_801F4044
    li r19, 0x0
    b @L_801F40FC
    @L_801F4044:
    li r20, 0x0
    b @L_801F40E0
    @L_801F404C:
    mr r3, r26
    mr r6, r20
    li r4, 0x0
    li r5, 0x35
    bl fn_801F54A4
    mr. r18, r3
    beq @L_801F40DC
    li r22, 0x0
    b @L_801F40D0
    @L_801F4070:
    mr r3, r18
    mr r6, r22
    li r4, 0x0
    li r5, 0x7
    bl fn_801F76B8
    mr. r19, r3
    beq @L_801F40CC
    li r21, 0x0
    b @L_801F40C0
    @L_801F4094:
    mr r3, r19
    mr r6, r21
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    cmplwi r3, 0x0
    beq @L_801F40BC
    cmplw r23, r3
    bne @L_801F40BC
    b @L_801F40F0
    @L_801F40BC:
    addi r21, r21, 0x1
    @L_801F40C0:
    clrlwi r0, r21, 16
    cmplwi r0, 0x6
    blt @L_801F4094
    @L_801F40CC:
    addi r22, r22, 0x1
    @L_801F40D0:
    clrlwi r0, r22, 16
    cmplwi r0, 0x2
    blt @L_801F4070
    @L_801F40DC:
    addi r20, r20, 0x1
    @L_801F40E0:
    clrlwi r0, r20, 16
    cmplwi r0, 0x2
    blt @L_801F404C
    li r19, 0x0
    @L_801F40F0:
    cmplwi r19, 0x0
    bne @L_801F40FC
    li r19, 0x0
    @L_801F40FC:
    cmplwi r19, 0x0
    bne @L_801F410C
    li r7, 0x0
    b @L_801F4134
    @L_801F410C:
    mr r3, r19
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r3, 0x0
    bne @L_801F4130
    li r7, 0x0
    b @L_801F4134
    @L_801F4130:
    mr r7, r3
    @L_801F4134:
    mr r3, r30
    mr r4, r27
    mr r5, r29
    mr r6, r28
    bl fn_802043D4
    clrlwi. r0, r25, 24
    mr r25, r3
    bne @L_801F4160
    li r18, 0x0
    li r19, 0x0
    b @L_801F417C
    @L_801F4160:
    mr r3, r31
    bl fn_802051D4
    mr r0, r3
    mr r3, r30
    mr r18, r0
    bl fn_802051D4
    mr r19, r3
    @L_801F417C:
    mr r4, r18
    li r3, 0x0
    li r5, 0x4
    li r6, 0x0
    bl fn_8011BEB4
    extsb r18, r3
    mr r4, r19
    li r3, 0x0
    li r5, 0x4
    li r6, 0x0
    bl fn_8011BEB4
    extsb. r0, r18
    extsb r3, r3
    bne @L_801F41BC
    extsb. r0, r3
    beq @L_801F41D8
    @L_801F41BC:
    cmpw r18, r3
    ble @L_801F41CC
    li r3, 0x1
    b @L_801F420C
    @L_801F41CC:
    bge @L_801F41D8
    li r3, 0x0
    b @L_801F420C
    @L_801F41D8:
    cmplw r24, r25
    ble @L_801F41E8
    li r3, 0x1
    b @L_801F420C
    @L_801F41E8:
    bge @L_801F41F4
    li r3, 0x0
    b @L_801F420C
    @L_801F41F4:
    bl fn_800E0C54
    clrlwi. r0, r3, 31
    beq @L_801F4208
    li r3, 0x1
    b @L_801F420C
    @L_801F4208:
    li r3, 0x0
    @L_801F420C:
    lmw r18, 0x28(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
