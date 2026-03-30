    nofralloc
    stwu r1, -0xd0(r1)
    mflr r0
    lis r5, lbl_80274EE0@ha
    stw r0, 0xd4(r1)
    stmw r21, 0xa4(r1)
    mr r21, r3
    addi r30, r5, lbl_80274EE0@l
    mr r31, r4
    bl fn_8019F01C
    li r7, 0x0
    mr r29, r3
    b @L_801AAF0C
@L_801AAED8:
    lis r4, lbl_80465678@ha
    lis r3, lbl_80465678@ha
    rlwinm r5, r7, 3, 0, 28
    rlwinm r0, r7, 3, 0, 28
    addi r4, r4, lbl_80465678@l
    li r6, 0x0
    add r4, r4, r5
    addi r3, r3, lbl_80465678@l
    add r3, r3, r0
    stw r6, 0x0(r4)
    li r0, 0x2
    addi r7, r7, 0x1
    stw r0, 0x4(r3)
@L_801AAF0C:
    cmpwi r7, 0x2
    blt @L_801AAED8
    lwz r0, 0x14(r29)
    li r22, 0x0
    rlwinm. r0, r0, 0, 24, 24
    beq @L_801AAF28
    ori r22, r22, 0x1
@L_801AAF28:
    li r3, 0x0
    li r4, 0x1
    bl fn_801BE490
    cmplwi r3, 0x0
    beq @L_801AAF40
    ori r22, r22, 0x3
@L_801AAF40:
    li r3, 0x0
    li r4, 0x5
    bl fn_801BE490
    cmplwi r3, 0x0
    beq @L_801AAF58
    ori r22, r22, 0x3
@L_801AAF58:
    li r3, 0x0
    li r4, 0x2
    bl fn_801BE490
    cmplwi r3, 0x0
    beq @L_801AAF70
    ori r22, r22, 0x5
@L_801AAF70:
    mr r3, r29
    mr r25, r22
    addi r4, r1, 0x68
    bl fn_80197A64
    lwz r28, 0x14(r21)
    mr r26, r3
    li r27, 0x0
    b @L_801AB24C
@L_801AAF90:
    lwz r24, 0x4(r28)
    mr r3, r27
    bl fn_801BF098
    cmplwi r24, 0x0
    mr r23, r3
    li r22, 0x0
    bne @L_801AAFBC
    addi r5, r30, 0x38
    addi r3, r2, -23016
    li r4, 0x71e
    bl fn_80196E10
@L_801AAFBC:
    lfs f1, 0x8(r24)
    lfs f0, -23008(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801AB058
    lwz r21, 0x4(r24)
    cmplwi r21, 0x0
    beq @L_801AB024
    cmplwi r21, 0x0
    bne @L_801AAFF4
    addi r3, r2, -23004
    li r4, 0x25d
    addi r5, r2, -22996
    bl fn_80196E10
@L_801AAFF4:
    lwz r0, 0x14(r21)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801AB014
    lwz r0, 0x14(r21)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801AB014
    li r3, 0x1
@L_801AB014:
    cmpwi r3, 0x0
    beq @L_801AB024
    mr r3, r21
    bl fn_8019D9DC
@L_801AB024:
    cmplwi r26, 0x0
    beq @L_801AB04C
    lwz r3, 0x4(r24)
    addi r5, r1, 0x38
    lwz r6, 0x4(r24)
    lwz r4, 0x78(r3)
    addi r3, r6, 0x44
    bl fn_800A2D98
    addi r21, r1, 0x38
    b @L_801AB16C
@L_801AB04C:
    lwz r3, 0x4(r24)
    addi r21, r3, 0x44
    b @L_801AB16C
@L_801AB058:
    lfs f0, -22988(r2)
    stfs f0, 0x64(r1)
    stfs f0, 0x60(r1)
    stfs f0, 0x5c(r1)
    stfs f0, 0x58(r1)
    stfs f0, 0x54(r1)
    stfs f0, 0x50(r1)
    stfs f0, 0x4c(r1)
    stfs f0, 0x48(r1)
    stfs f0, 0x44(r1)
    stfs f0, 0x40(r1)
    stfs f0, 0x3c(r1)
    stfs f0, 0x38(r1)
    b @L_801AB160
@L_801AB090:
    lwz r0, 0x4(r24)
    cmplwi r0, 0x0
    bne @L_801AB0AC
    addi r5, r30, 0x44
    addi r3, r2, -23016
    li r4, 0x732
    bl fn_80196E10
@L_801AB0AC:
    lwz r21, 0x4(r24)
    cmplwi r21, 0x0
    beq @L_801AB100
    cmplwi r21, 0x0
    bne @L_801AB0D0
    addi r3, r2, -23004
    li r4, 0x25d
    addi r5, r2, -22996
    bl fn_80196E10
@L_801AB0D0:
    lwz r0, 0x14(r21)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801AB0F0
    lwz r0, 0x14(r21)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801AB0F0
    li r3, 0x1
@L_801AB0F0:
    cmpwi r3, 0x0
    beq @L_801AB100
    mr r3, r21
    bl fn_8019D9DC
@L_801AB100:
    addic. r0, r21, 0x44
    bne @L_801AB118
    addi r3, r2, -23016
    li r4, 0x735
    addi r5, r2, -22984
    bl fn_80196E10
@L_801AB118:
    lwz r0, 0x78(r21)
    cmplwi r0, 0x0
    bne @L_801AB134
    addi r5, r30, 0x54
    addi r3, r2, -23016
    li r4, 0x736
    bl fn_80196E10
@L_801AB134:
    lwz r4, 0x78(r21)
    addi r3, r21, 0x44
    addi r5, r1, 0x8
    bl fn_800A2D98
    lfs f1, 0x8(r24)
    addi r3, r1, 0x8
    addi r4, r1, 0x38
    addi r5, r1, 0x38
    bl fn_801A85F0
    lwz r24, 0x0(r24)
    addi r22, r22, 0x1
@L_801AB160:
    cmplwi r24, 0x0
    bne @L_801AB090
    addi r21, r1, 0x38
@L_801AB16C:
    mr r3, r22
    bl fn_801AA5AC
    cmplwi r26, 0x0
    beq @L_801AB18C
    mr r3, r21
    mr r4, r26
    addi r5, r1, 0x38
    bl fn_800A2D98
@L_801AB18C:
    mr r3, r31
    mr r4, r21
    addi r5, r1, 0x8
    bl fn_800A2D98
    mr r4, r23
    addi r3, r1, 0x8
    bl fn_800BD4B4
    lis r3, lbl_8036CC40@ha
    rlwinm. r0, r25, 0, 31, 31
    addi r4, r3, lbl_8036CC40@l
    lwz r3, 0xc(r4)
    addi r0, r3, 0x1
    stw r0, 0xc(r4)
    beq @L_801AB244
    addi r3, r1, 0x8
    addi r4, r1, 0x38
    bl fn_800A2FAC
    cmplwi r3, 0x0
    bne @L_801AB1E4
    addi r3, r1, 0x8
    addi r4, r1, 0x38
    bl fn_800A2D64
@L_801AB1E4:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_801AB210
    mr r4, r23
    addi r3, r1, 0x38
    bl fn_800BD504
    lis r3, lbl_8036CC40@ha
    addi r4, r3, lbl_8036CC40@l
    lwz r3, 0xc(r4)
    addi r0, r3, 0x1
    stw r0, 0xc(r4)
@L_801AB210:
    rlwinm. r0, r25, 0, 29, 30
    beq @L_801AB244
    mr r3, r27
    bl fn_801BBF28
    mr r4, r3
    addi r3, r1, 0x38
    li r5, 0x0
    bl fn_800BD58C
    lis r3, lbl_8036CC40@ha
    addi r4, r3, lbl_8036CC40@l
    lwz r3, 0xc(r4)
    addi r0, r3, 0x1
    stw r0, 0xc(r4)
@L_801AB244:
    lwz r28, 0x0(r28)
    addi r27, r27, 0x1
@L_801AB24C:
    cmpwi r27, 0xa
    bge @L_801AB25C
    cmplwi r28, 0x0
    bne @L_801AAF90
@L_801AB25C:
    lmw r21, 0xa4(r1)
    lwz r0, 0xd4(r1)
    mtlr r0
    addi r1, r1, 0xd0
    blr
