    nofralloc
    stwu r1, -0x2f0(r1)
    mflr r0
    stw r0, 0x2f4(r1)
    stmw r19, 0x2bc(r1)
    mr r27, r4
    mr r28, r5
    mr r29, r6
    mr r26, r3
    mr r30, r7
    mr r31, r8
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    li r4, 0x0
    li r3, 0x0
    b @L_80233E14
@L_80233E08:
    clrlslwi r0, r4, 16, 2
    addi r4, r4, 0x1
    stwx r3, r31, r0
@L_80233E14:
    rlwinm r0, r4, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_80233E08
    mr r3, r26
    li r4, 0x0
    li r5, 0x45
    li r6, 0x0
    bl fn_801FB1C0
    mr. r21, r3
    bne @L_80233E44
    li r3, 0x0
    b @L_802342B8
@L_80233E44:
    mr r3, r26
    li r4, 0x0
    li r5, 0x45
    li r6, 0x1
    bl fn_801FB1C0
    mr. r25, r3
    bne @L_80233E68
    li r3, 0x0
    b @L_802342B8
@L_80233E68:
    li r0, 0x2a
    addi r6, r1, 0x158
    subi r4, r21, 0x4
    mtctr r0
@L_80233E78:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r6)
    stwu r0, 0x8(r6)
    bdnz @L_80233E78
    lwz r3, 0x4(r4)
    li r0, 0x2a
    addi r5, r1, 0x4
    subi r4, r25, 0x4
    stw r3, 0x4(r6)
    mtctr r0
@L_80233EA4:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80233EA4
    lwz r3, 0x4(r4)
    rlwinm r0, r29, 0, 16, 31
    li r4, 0x0
    stw r3, 0x4(r5)
    b @L_80233ED0
@L_80233ECC:
    addi r4, r4, 0x1
@L_80233ED0:
    rlwinm r3, r4, 0, 16, 31
    cmplw r3, r0
    blt @L_80233ECC
    li r23, 0x0
    li r24, 0x0
    b @L_80234250
@L_80233EE8:
    mr r3, r27
    mr r5, r24
    li r4, 0x3
    bl fn_8012A5B0
    mr. r22, r3
    beq @L_8023424C
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8023424C
    mr r3, r22
    bl fn_801233F4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8023424C
    rlwinm r0, r29, 0, 16, 31
    li r4, 0x0
    b @L_80233F44
@L_80233F28:
    clrlslwi r3, r4, 16, 2
    lwzx r3, r28, r3
    cmplwi r3, 0x0
    beq @L_80233F40
    cmplw r3, r22
    beq @L_80233F50
@L_80233F40:
    addi r4, r4, 0x1
@L_80233F44:
    rlwinm r3, r4, 0, 16, 31
    cmplw r3, r0
    blt @L_80233F28
@L_80233F50:
    rlwinm r3, r4, 0, 16, 31
    rlwinm r0, r29, 0, 16, 31
    cmplw r3, r0
    blt @L_8023424C
    li r3, 0x0
    bl fn_801F4804
    mr r5, r3
    mr r3, r21
    mr r4, r22
    bl fn_80206AEC
    rlwinm. r0, r30, 0, 24, 31
    beq @L_80234240
    cmplwi r0, 0x1
    bne @L_80234034
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r26
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r19, r3, 0, 16, 31
    mr r3, r21
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0xc9
    li r6, 0x0
    bl fn_8012640C
    rlwinm r20, r3, 0, 16, 31
    mr r4, r19
    li r3, 0x0
    li r5, 0x23
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80234020
    mr r4, r20
    li r3, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    b @L_80234024
@L_80234020:
    li r0, 0x0
@L_80234024:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8023424C
    b @L_80234240
@L_80234034:
    cmplwi r0, 0x2
    bne @L_802340E8
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r26
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r20, r3, 0, 16, 31
    mr r3, r21
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0xc9
    li r6, 0x0
    bl fn_8012640C
    rlwinm r19, r3, 0, 16, 31
    mr r4, r20
    li r3, 0x0
    li r5, 0x23
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802340D4
    mr r4, r19
    li r3, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    b @L_802340D8
@L_802340D4:
    li r0, 0x0
@L_802340D8:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x3
    bne @L_8023424C
    b @L_80234240
@L_802340E8:
    cmplwi r0, 0x3
    bne @L_80234240
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r26
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r20, r3, 0, 16, 31
    mr r3, r21
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0xc9
    li r6, 0x0
    bl fn_8012640C
    rlwinm r19, r3, 0, 16, 31
    mr r4, r20
    li r3, 0x0
    li r5, 0x23
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80234188
    mr r4, r19
    li r3, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    b @L_8023418C
@L_80234188:
    li r0, 0x0
@L_8023418C:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x2
    beq @L_8023424C
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r26
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r20, r3, 0, 16, 31
    mr r3, r21
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0xc9
    li r6, 0x0
    bl fn_8012640C
    rlwinm r19, r3, 0, 16, 31
    mr r4, r20
    li r3, 0x0
    li r5, 0x23
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80234230
    mr r4, r19
    li r3, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    b @L_80234234
@L_80234230:
    li r0, 0x0
@L_80234234:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x3
    beq @L_8023424C
@L_80234240:
    clrlslwi r0, r23, 16, 2
    addi r23, r23, 0x1
    stwx r22, r31, r0
@L_8023424C:
    addi r24, r24, 0x1
@L_80234250:
    rlwinm r0, r24, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_80233EE8
    li r0, 0x2a
    subi r6, r21, 0x4
    addi r4, r1, 0x158
    mtctr r0
@L_8023426C:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r6)
    stwu r0, 0x8(r6)
    bdnz @L_8023426C
    lwz r3, 0x4(r4)
    li r0, 0x2a
    subi r5, r25, 0x4
    addi r4, r1, 0x4
    stw r3, 0x4(r6)
    mtctr r0
@L_80234298:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80234298
    lwz r0, 0x4(r4)
    mr r3, r23
    stw r0, 0x4(r5)
@L_802342B8:
    lmw r19, 0x2bc(r1)
    lwz r0, 0x2f4(r1)
    mtlr r0
    addi r1, r1, 0x2f0
    blr
