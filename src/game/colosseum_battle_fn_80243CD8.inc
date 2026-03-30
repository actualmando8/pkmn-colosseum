    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stmw r15, 0x4c(r1)
    mr r30, r4
    mr r28, r5
    mr r31, r3
    mr r16, r6
    li r29, 0x0
    li r4, 0x0
    li r5, 0x1
    bl fn_80235B04
    mr r25, r3
    mr r4, r31
    addi r5, r1, 0x20
    li r3, 0x0
    li r6, 0x1
    li r7, 0x1
    bl fn_801F1C18
    mr r15, r3
    mr r3, r31
    mr r4, r16
    addi r5, r1, 0x8
    bl fn_80237CB8
    mr r16, r3
    mr r3, r31
    mr r4, r30
    bl fn_80236520
    mr r27, r3
    mr r3, r31
    mr r4, r30
    bl fn_8023831C
    mr r26, r3
    addi r17, r1, 0x8
    clrlwi r16, r16, 16
    li r18, 0x0
    li r19, 0x0
    b @L_80243DAC
@L_80243D70:
    mr r3, r31
    mr r4, r28
    bl fn_80239500
    clrlslwi r0, r19, 16, 1
    mr r6, r3
    lhzx r5, r17, r0
    mr r3, r31
    mr r4, r30
    bl fn_8023793C
    clrlwi r0, r3, 16
    cmplwi r0, 0x41
    bne @L_80243DA8
    li r18, 0x1
    b @L_80243DB8
@L_80243DA8:
    addi r19, r19, 0x1
@L_80243DAC:
    clrlwi r0, r19, 16
    cmplw r0, r16
    blt @L_80243D70
@L_80243DB8:
    clrlwi r0, r18, 24
    cmplwi r0, 0x1
    bne @L_80243E0C
    mr r4, r31
    li r3, 0x0
    li r5, 0x16a
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x16a
    bl fn_80239EE8
@L_80243E0C:
    lfs f1, -20592(r2)
    mr r3, r31
    mr r4, r30
    li r5, -0x1
    bl fn_802373B0
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80243E74
    mr r3, r29
    mr r4, r31
    li r5, 0x16b
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x16b
    bl fn_80239EE8
@L_80243E74:
    addi r16, r1, 0x20
    clrlwi r21, r15, 16
    li r23, 0x0
    b @L_80243F7C
@L_80243E84:
    clrlslwi r0, r23, 16, 2
    lwzx r4, r16, r0
    cmplw r30, r4
    beq @L_80243F78
    mr r3, r31
    addi r5, r1, 0xc
    li r6, 0x0
    li r7, 0x1
    bl fn_802367CC
    clrlwi. r17, r3, 16
    mr r24, r3
    beq @L_80243F78
    lis r4, 0x8025
    lis r3, 0x8025
    lis r5, 0x8025
    addi r15, r1, 0xc
    addi r19, r4, -19340
    addi r20, r3, -16448
    addi r18, r5, -6868
    li r22, 0x0
    b @L_80243F5C
@L_80243ED8:
    clrlslwi r0, r22, 16, 1
    li r3, 0x0
    lhzx r4, r15, r0
    li r5, 0x1c
    li r6, 0x0
    bl fn_8011BEB4
    cmplwi r3, 0x0
    bne @L_80243EFC
    mr r3, r18
@L_80243EFC:
    cmplw r3, r19
    beq @L_80243F0C
    cmplw r3, r20
    bne @L_80243F58
@L_80243F0C:
    mr r3, r29
    mr r4, r31
    li r5, 0x16c
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x16c
    bl fn_80239EE8
    b @L_80243F68
@L_80243F58:
    addi r22, r22, 0x1
@L_80243F5C:
    clrlwi r0, r22, 16
    cmplw r0, r17
    blt @L_80243ED8
@L_80243F68:
    clrlwi r3, r22, 16
    clrlwi r0, r24, 16
    cmplw r3, r0
    blt @L_80243F88
@L_80243F78:
    addi r23, r23, 0x1
@L_80243F7C:
    clrlwi r0, r23, 16
    cmplw r0, r21
    blt @L_80243E84
@L_80243F88:
    lis r3, 0x8025
    li r15, 0x0
    addi r16, r3, -19340
    b @L_8024404C
@L_80243F98:
    mr r4, r15
    li r3, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_8011BEB4
    cmplwi r3, 0x0
    bne @L_80243FBC
    lis r3, 0x8025
    addi r3, r3, -6868
@L_80243FBC:
    cmplw r3, r16
    beq @L_80243FD4
    lis r4, 0x8025
    addi r0, r4, -16448
    cmplw r3, r0
    bne @L_80244048
@L_80243FD4:
    mr r4, r31
    mr r7, r15
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r8, 0x0
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80244048
    mr r3, r29
    mr r4, r31
    li r5, 0x16d
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x16d
    bl fn_80239EE8
    b @L_80244060
@L_80244048:
    addi r15, r15, 0x1
@L_8024404C:
    lwz r3, -31272(r13)
    clrlwi r4, r15, 16
    lwz r0, 0x0(r3)
    cmplw r4, r0
    blt @L_80243F98
@L_80244060:
    mr r3, r31
    mr r4, r30
    li r15, 0x0
    li r5, 0x6
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80244084
    li r15, 0x1
@L_80244084:
    mr r3, r31
    mr r4, r30
    li r5, 0x3
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802440A4
    li r15, 0x1
@L_802440A4:
    mr r3, r31
    mr r4, r30
    li r5, 0x4
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802440C4
    li r15, 0x1
@L_802440C4:
    mr r3, r31
    mr r4, r30
    li r5, 0x5
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802440E4
    li r15, 0x1
@L_802440E4:
    mr r3, r31
    mr r4, r30
    li r5, 0x18
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80244104
    li r15, 0x1
@L_80244104:
    mr r3, r31
    mr r4, r30
    li r5, 0x1c
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80244124
    li r15, 0x1
@L_80244124:
    clrlwi r0, r15, 24
    cmplwi r0, 0x1
    bne @L_80244178
    mr r3, r29
    mr r4, r31
    li r5, 0x16e
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x16e
    bl fn_80239EE8
@L_80244178:
    clrlwi r0, r27, 16
    cmplwi r0, 0xcb
    bne @L_802441E8
    mr r3, r30
    li r4, 0x0
    li r5, 0xfc
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    beq @L_802441E8
    mr r3, r29
    mr r4, r31
    li r5, 0x16f
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x16f
    bl fn_80239EE8
@L_802441E8:
    clrlwi r0, r26, 16
    cmplwi r0, 0x11
    beq @L_80244244
    cmplwi r0, 0xf
    beq @L_80244244
    mr r3, r29
    mr r4, r31
    li r5, 0x170
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x170
    bl fn_80239EE8
@L_80244244:
    mr r3, r31
    mr r4, r30
    bl fn_802376EC
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_802442A4
    mr r3, r29
    mr r4, r31
    li r5, 0x171
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x171
    bl fn_80239EE8
@L_802442A4:
    clrlwi r0, r25, 24
    cmplwi r0, 0x4
    beq @L_802442B8
    cmplwi r0, 0x3
    bne @L_80244300
@L_802442B8:
    mr r3, r29
    mr r4, r31
    li r5, 0x172
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x172
    bl fn_80239EE8
@L_80244300:
    mr r3, r29
    lmw r15, 0x4c(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
