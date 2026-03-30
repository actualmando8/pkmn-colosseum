    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r26, r3
    lis r3, -32713
    li r28, 0x0
    rlwinm. r30, r26, 0, 30, 31
    li r27, 0x0
    addi r31, r3, -12664
    bne @L_801B2F4C
    li r30, 0x1
@L_801B2F4C:
    rlwinm. r29, r26, 0, 17, 18
    bne @L_801B2F58
    rlwinm r29, r30, 13, 0, 18
@L_801B2F58:
    rlwinm. r0, r26, 0, 28, 28
    beq @L_801B2FC0
    bl fn_801A68D8
    addi r4, r31, 0x0
    stw r3, 0x20(r4)
    addi r3, r31, 0x0
    bl fn_801B3D1C
    li r28, 0x1
    bl fn_801A68D0
    li r25, 0x0
    mr r24, r3
    b @L_801B2FB8
@L_801B2F88:
    mr r3, r25
    bl fn_801A678C
    cmplwi r3, 0x0
    beq @L_801B2FB4
    lwz r0, 0x10(r3)
    lis r4, -32698
    addi r5, r4, 22288
    stw r0, 0x8(r1)
    addi r4, r1, 0x8
    lfs f1, 0x10(r5)
    bl fn_801A6098
@L_801B2FB4:
    addi r25, r25, 0x1
@L_801B2FB8:
    cmpw r25, r24
    blt @L_801B2F88
@L_801B2FC0:
    rlwinm. r0, r26, 0, 29, 29
    beq @L_801B30DC
    li r3, 0x100
    bl fn_801A67BC
    mr. r26, r3
    beq @L_801B3000
    lhz r0, 0x8(r26)
    rlwinm. r0, r0, 0, 29, 29
    beq @L_801B3000
    lis r3, -32698
    addi r5, r31, 0x30
    addi r3, r3, 22288
    addi r4, r26, 0x10
    addi r5, r5, 0xc
    bl fn_801BF16C
    b @L_801B300C
@L_801B3000:
    lwz r0, -31624(r13)
    addi r3, r31, 0x30
    stw r0, 0xc(r3)
@L_801B300C:
    rlwinm r0, r30, 31, 31, 31
    addi r3, r31, 0x30
    stw r0, 0x1c(r3)
    bl fn_801A68F0
    addi r4, r31, 0x30
    stw r3, 0x20(r4)
    addi r3, r31, 0x30
    bl fn_801B3D1C
    rlwinm. r0, r29, 0, 17, 17
    beq @L_801B3050
    addi r3, r31, 0x60
    li r0, 0x3
    stw r0, 0x4(r3)
    addi r3, r31, 0x90
    li r27, 0x1
    bl fn_801B3D1C
    b @L_801B305C
@L_801B3050:
    addi r3, r31, 0x60
    li r0, 0x2
    stw r0, 0x4(r3)
@L_801B305C:
    bl fn_801A68E0
    addi r4, r31, 0x60
    cmplwi r26, 0x0
    stw r3, 0x20(r4)
    beq @L_801B3084
    lhz r0, 0x8(r26)
    rlwinm. r0, r0, 0, 27, 27
    beq @L_801B3084
    lbz r7, 0x13(r26)
    b @L_801B3088
@L_801B3084:
    li r7, 0x0
@L_801B3088:
    addi r3, r31, 0x60
    lwz r0, 0x20(r3)
    cmpwi r0, 0x0
    beq @L_801B30BC
    addi r3, r31, 0x60
    addi r5, r31, 0x60
    li r6, 0x1
    addi r4, r31, 0x60
    li r0, 0xff
    stb r6, 0x14(r5)
    stb r0, 0x13(r4)
    stb r7, 0xf(r3)
    b @L_801B30D0
@L_801B30BC:
    addi r3, r31, 0x60
    addi r4, r31, 0x60
    li r0, 0x0
    stb r7, 0x13(r3)
    stb r0, 0x14(r4)
@L_801B30D0:
    addi r3, r31, 0x60
    bl fn_801B3D1C
    b @L_801B3104
@L_801B30DC:
    rlwinm r0, r30, 31, 31, 31
    addi r3, r31, 0xc0
    stw r0, 0x1c(r3)
    addi r3, r31, 0xc0
    bl fn_801B3D1C
    rlwinm r0, r29, 18, 31, 31
    addi r3, r31, 0xf0
    stw r0, 0x1c(r3)
    addi r3, r31, 0xf0
    bl fn_801B3D1C
@L_801B3104:
    cmpwi r28, 0x0
    beq @L_801B3128
    cmpwi r27, 0x0
    bne @L_801B311C
    li r3, 0x3
    bl fn_801B3AE8
@L_801B311C:
    li r3, 0x2
    bl fn_801B3AA8
    b @L_801B3154
@L_801B3128:
    cmpwi r27, 0x0
    beq @L_801B3144
    li r3, 0x1
    bl fn_801B3AE8
    li r3, 0x2
    bl fn_801B3AA8
    b @L_801B3154
@L_801B3144:
    li r3, 0x5
    bl fn_801B3AE8
    li r3, 0x1
    bl fn_801B3AA8
@L_801B3154:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
