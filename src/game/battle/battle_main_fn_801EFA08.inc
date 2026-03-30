    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x64(r1)
    li r5, 0x12
    li r6, 0x0
    stmw r26, 0x48(r1)
    bl fn_801F54A4
    cmplwi r3, 0x0
    beq @L_801EFA40
    li r4, 0x3e8
    li r5, 0xff
    bl fn_801659FC
@L_801EFA40:
    bl fn_8020C840
    lfs f1, -20888(r2)
    li r3, 0x8
    bl fn_801C41C8
    lis r3, lbl_8046D760@ha
    lis r5, lbl_80375CC8@ha
    addi r8, r5, lbl_80375CC8@l
    li r4, 0x0
    addi r3, r3, lbl_8046D760@l
    li r5, 0x0
    li r6, 0x2
    li r7, 0x0
    bl fn_801F0B00
    bl fn_801C2D54
    bl fn_801C2D68
    bl fn_800FF56C
    bl fn_80113FB4
    cmplwi r3, 0x0
    beq @L_801EFA98
    lis r4, 0x7b
    addi r4, r4, 0x1800
    bl fn_801C2F00
@L_801EFA98:
    li r3, 0x1
    bl fn_801C40F0
    bl fn_801337A0
    mr r28, r3
    li r3, 0x1
    bl fn_801337A8
    bl fn_8000816C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801EFACC
    li r3, 0x1
    bl fn_80133810
    b @L_801EFB1C
@L_801EFACC:
    li r29, 0x0
    b @L_801EFAFC
@L_801EFAD4:
    addi r3, r1, 0x24
    li r4, 0x1
    bl fn_80103BA8
    lhz r0, 0x24(r1)
    rlwinm. r0, r0, 0, 20, 20
    bne @L_801EFB08
    bl fn_800F0308
    bl fn_800D3088
    add r0, r29, r3
    rlwinm r29, r0, 0, 24, 31
@L_801EFAFC:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_801EFAD4
@L_801EFB08:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x4
    bge @L_801EFB1C
    li r3, 0x1
    bl fn_80133810
@L_801EFB1C:
    mr r3, r28
    bl fn_801337A8
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    bne @L_801EFE5C
    bl fn_801C2D54
    bl fn_801C2D68
    bl fn_800FF56C
    bl fn_80113FB4
    cmplwi r3, 0x0
    beq @L_801EFB54
    lis r4, 0x7b
    addi r4, r4, 0x1800
    bl fn_801C2F00
@L_801EFB54:
    lis r3, lbl_8046D760@ha
    lis r5, lbl_80375CC8@ha
    addi r8, r5, lbl_80375CC8@l
    li r4, 0x0
    addi r3, r3, lbl_8046D760@l
    li r5, 0x0
    li r6, 0x4
    li r7, 0x5
    bl fn_801F0B00
    bl fn_801C2D54
    bl fn_801C2D68
    bl fn_800FF56C
    bl fn_80113FB4
    cmplwi r3, 0x0
    beq @L_801EFB9C
    lis r4, 0x7b
    addi r4, r4, 0x1800
    bl fn_801C2F00
@L_801EFB9C:
    lis r3, lbl_8046D760@ha
    lis r5, lbl_80375CC8@ha
    addi r8, r5, lbl_80375CC8@l
    li r4, 0x0
    addi r3, r3, lbl_8046D760@l
    li r5, 0x0
    li r6, 0x4
    li r7, 0x4
    bl fn_801F0B00
    bl fn_801C2D54
    bl fn_801C2D68
    bl fn_800FF56C
    bl fn_80113FB4
    cmplwi r3, 0x0
    beq @L_801EFBE4
    lis r4, 0x7b
    addi r4, r4, 0x1800
    bl fn_801C2F00
@L_801EFBE4:
    lis r3, lbl_8046D760@ha
    lis r5, lbl_80375CC8@ha
    addi r8, r5, lbl_80375CC8@l
    li r4, 0x0
    addi r3, r3, lbl_8046D760@l
    li r5, 0x0
    li r6, 0x5
    li r7, 0x0
    bl fn_801F0B00
    bl fn_801F09D0
    li r3, 0x0
    bl fn_801F1758
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801EFC28
    bl fn_80265C84
    bl fn_80265C10
@L_801EFC28:
    lis r4, lbl_8046D760@ha
    lis r3, lbl_80375D30@ha
    addi r28, r4, lbl_8046D760@l
    addi r29, r3, lbl_80375D30@l
    mr r30, r28
    mr r31, r29
    b @L_801EFE4C
@L_801EFC44:
    bl fn_801337A0
    mr r27, r3
    li r3, 0x1
    bl fn_801337A8
    bl fn_8000816C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801EFC70
    li r3, 0x1
    bl fn_80133810
    b @L_801EFCC0
@L_801EFC70:
    li r26, 0x0
    b @L_801EFCA0
@L_801EFC78:
    addi r3, r1, 0x8
    li r4, 0x1
    bl fn_80103BA8
    lhz r0, 0x8(r1)
    rlwinm. r0, r0, 0, 20, 20
    bne @L_801EFCAC
    bl fn_800F0308
    bl fn_800D3088
    add r0, r26, r3
    rlwinm r26, r0, 0, 24, 31
@L_801EFCA0:
    rlwinm r0, r26, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_801EFC78
@L_801EFCAC:
    rlwinm r0, r26, 0, 24, 31
    cmplwi r0, 0x4
    bge @L_801EFCC0
    li r3, 0x1
    bl fn_80133810
@L_801EFCC0:
    mr r3, r27
    bl fn_801337A8
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    bne @L_801EFE58
    li r3, 0x0
    bl fn_801F1758
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801EFD5C
    bl fn_80265B3C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801EFD5C
    bl fn_801C2D54
    bl fn_801C2D68
    bl fn_800FF56C
    bl fn_80113FB4
    cmplwi r3, 0x0
    beq @L_801EFD1C
    lis r4, 0x7b
    addi r4, r4, 0x1800
    bl fn_801C2F00
@L_801EFD1C:
    li r3, 0x0
    bl fn_80261AD0
    li r3, 0x0
    bl fn_801F1588
    li r3, 0xb4
    bl fn_80008184
    mr. r28, r3
    beq @L_801EFE58
    li r26, 0x0
    b @L_801EFD50
@L_801EFD44:
    bl fn_800F0308
    bl fn_800D3088
    add r26, r26, r3
@L_801EFD50:
    cmplw r26, r28
    blt @L_801EFD44
    b @L_801EFE58
@L_801EFD5C:
    li r3, 0x0
    bl fn_801F1700
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801EFD78
    bl fn_80265A6C
    bl fn_802659F8
@L_801EFD78:
    bl fn_801C2D54
    bl fn_801C2D5C
    bl fn_800FF56C
    bl fn_80113FB4
    cmplwi r3, 0x0
    beq @L_801EFD9C
    lis r4, 0x7b
    addi r4, r4, 0x1800
    bl fn_801C2F00
@L_801EFD9C:
    li r3, -0x1
    bl fn_80261AD0
    bl fn_801F09D0
    mr r3, r28
    mr r8, r29
    li r4, 0x0
    li r5, 0x0
    li r6, 0x7
    li r7, 0x0
    bl fn_801F0B00
    bl fn_801F09D0
    li r3, 0x0
    bl fn_80261954
    bl fn_801C2D68
    li r3, 0x0
    bl fn_801F1700
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801EFDF0
    bl fn_802659A4
    bl fn_802658C8
@L_801EFDF0:
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    bne @L_801EFE58
    mr r3, r28
    mr r8, r29
    li r4, 0x0
    li r5, 0x0
    li r6, 0xb
    li r7, 0x0
    bl fn_801F0B00
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    bne @L_801EFE58
    mr r3, r30
    mr r8, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0xd
    li r7, 0x0
    bl fn_801F0B00
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    bne @L_801EFE58
@L_801EFE4C:
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    beq @L_801EFC44
@L_801EFE58:
    bl fn_801F09D0
@L_801EFE5C:
    bl fn_801C2D54
    bl fn_801C2D68
    bl fn_800FF56C
    bl fn_80113FB4
    cmplwi r3, 0x0
    beq @L_801EFE80
    lis r4, 0x7b
    addi r4, r4, 0x1800
    bl fn_801C2F00
@L_801EFE80:
    bl fn_801EF2D4
    li r3, 0x0
    bl fn_801F1758
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801EFEA0
    bl fn_80265BBC
    bl fn_80265D54
@L_801EFEA0:
    lis r3, lbl_8046D760@ha
    lis r5, lbl_80375D80@ha
    addi r8, r5, lbl_80375D80@l
    li r4, 0x0
    addi r3, r3, lbl_8046D760@l
    li r5, 0x0
    li r6, 0xe
    li r7, 0x0
    bl fn_801F0B00
    bl fn_801EF634
    lis r5, lbl_80375D80@ha
    lis r6, lbl_8046D760@ha
    rlwinm r7, r3, 0, 16, 31
    li r4, 0x0
    addi r3, r6, lbl_8046D760@l
    addi r8, r5, lbl_80375D80@l
    li r5, 0x0
    li r6, 0xf
    bl fn_801F0B00
    bl fn_801F09D0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x12
    li r6, 0x0
    bl fn_801F54A4
    cmplwi r3, 0x0
    beq @L_801EFF14
    li r4, 0x3e8
    bl fn_801657F8
@L_801EFF14:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x12
    li r6, 0x0
    bl fn_801F54A4
    cmplwi r3, 0x0
    bne @L_801EFF44
    li r3, 0x1
    li r4, 0x3e8
    li r5, 0x3e8
    li r6, 0xff
    bl fn_8016597C
@L_801EFF44:
    lfs f1, -20888(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    lis r3, lbl_8046D760@ha
    lis r5, lbl_80375D80@ha
    addi r8, r5, lbl_80375D80@l
    li r4, 0x0
    addi r3, r3, lbl_8046D760@l
    li r5, 0x0
    li r6, 0x10
    li r7, 0x0
    bl fn_801F0B00
    lis r3, lbl_8046D760@ha
    lis r5, lbl_80375D80@ha
    addi r8, r5, lbl_80375D80@l
    li r4, 0x0
    addi r3, r3, lbl_8046D760@l
    li r5, 0x0
    li r6, 0x11
    li r7, 0x0
    bl fn_801F0B00
    bl fn_800FF660
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    lmw r26, 0x48(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr