    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stmw r23, 0x4c(r1)
    mr r31, r5
    mr r30, r3
    mr r28, r4
    mr r29, r6
    mr r3, r31
    li r24, 0x0
    li r25, 0x0
    bl fn_80206780
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80235C24
    li r3, 0x0
    b @L_80236254
@L_80235C24:
    li r5, 0x17
    li r6, 0x0
    li r0, 0x2
    lis r3, -32733
    stw r5, 0x30(r1)
    addi r4, r3, 25192
    mr r3, r28
    addi r5, r1, 0x30
    stw r6, 0x34(r1)
    li r6, 0x0
    stw r0, 0x38(r1)
    stw r31, 0x3c(r1)
    stw r30, 0x40(r1)
    bl fn_801F37B0
    li r6, 0x47
    li r5, 0x0
    li r0, 0x2
    lis r3, -32733
    stw r6, 0x1c(r1)
    addi r4, r3, 25192
    lwz r26, 0x34(r1)
    mr r3, r28
    stw r5, 0x20(r1)
    addi r5, r1, 0x1c
    li r6, 0x0
    stw r0, 0x24(r1)
    stw r31, 0x28(r1)
    stw r30, 0x2c(r1)
    bl fn_801F37B0
    li r0, 0x0
    li r4, 0x2a
    lis r3, -32733
    stw r4, 0x8(r1)
    addi r4, r3, 25192
    lwz r27, 0x20(r1)
    stw r0, 0xc(r1)
    mr r3, r28
    addi r5, r1, 0x8
    li r6, 0x0
    stw r0, 0x10(r1)
    stw r31, 0x14(r1)
    stw r30, 0x18(r1)
    bl fn_801F37B0
    lwz r28, 0xc(r1)
    mr r3, r30
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r23, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2a
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80235D94
    mr r3, r30
    mr r4, r31
    mr r5, r23
    bl fn_801F8424
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80235D84
    mr r3, r31
    li r4, 0x0
    bl fn_80207B8C
    b @L_80235D98
@L_80235D84:
    mr r3, r31
    li r4, 0x0
    bl fn_80207B8C
    b @L_80235D98
@L_80235D94:
    li r3, 0x9
@L_80235D98:
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2
    beq @L_80235E4C
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r23, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2a
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80235E3C
    mr r3, r30
    mr r4, r31
    mr r5, r23
    bl fn_801F8424
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80235E2C
    mr r3, r31
    li r4, 0x1
    bl fn_80207B8C
    b @L_80235E40
@L_80235E2C:
    mr r3, r31
    li r4, 0x1
    bl fn_80207B8C
    b @L_80235E40
@L_80235E3C:
    li r3, 0x9
@L_80235E40:
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2
    bne @L_80235E54
@L_80235E4C:
    li r0, 0x1
    b @L_80235E58
@L_80235E54:
    li r0, 0x0
@L_80235E58:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80235F10
    mr r3, r30
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r30
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2b
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80235EF4
    mr r3, r31
    bl fn_80207BF4
    b @L_80235EF8
@L_80235EF4:
    li r3, 0x0
@L_80235EF8:
    rlwinm r0, r3, 0, 16, 31
    subfic r0, r0, 0x1a
    cntlzw r0, r0
    rlwinm r0, r0, 27, 24, 31
    cmplwi r0, 0x1
    bne @L_80235F14
@L_80235F10:
    li r24, 0x1
@L_80235F14:
    mr r3, r30
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r23, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2a
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80235FD4
    mr r3, r30
    mr r4, r31
    mr r5, r23
    bl fn_801F8424
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80235FC4
    mr r3, r31
    li r4, 0x0
    bl fn_80207B8C
    b @L_80235FD8
@L_80235FC4:
    mr r3, r31
    li r4, 0x0
    bl fn_80207B8C
    b @L_80235FD8
@L_80235FD4:
    li r3, 0x9
@L_80235FD8:
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x8
    beq @L_8023608C
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r23, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2a
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023607C
    mr r3, r30
    mr r4, r31
    mr r5, r23
    bl fn_801F8424
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023606C
    mr r3, r31
    li r4, 0x1
    bl fn_80207B8C
    b @L_80236080
@L_8023606C:
    mr r3, r31
    li r4, 0x1
    bl fn_80207B8C
    b @L_80236080
@L_8023607C:
    li r3, 0x9
@L_80236080:
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x8
    bne @L_80236094
@L_8023608C:
    li r0, 0x1
    b @L_80236098
@L_80236094:
    li r0, 0x0
@L_80236098:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802360A8
    li r25, 0x1
@L_802360A8:
    mr r3, r30
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x24
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80236100
    mr r3, r31
    li r4, 0x16
    bl fn_802026E4
    b @L_80236104
@L_80236100:
    li r3, 0x0
@L_80236104:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802361E0
    mr r3, r30
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x24
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80236168
    mr r3, r31
    li r4, 0xe
    bl fn_802026E4
    b @L_8023616C
@L_80236168:
    li r3, 0x0
@L_8023616C:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802361E0
    mr r3, r30
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x24
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802361D0
    mr r3, r31
    li r4, 0x25
    bl fn_802026E4
    b @L_802361D4
@L_802361D0:
    li r3, 0x0
@L_802361D4:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802361E8
@L_802361E0:
    li r3, 0x1
    b @L_80236254
@L_802361E8:
    cmplwi r26, 0x0
    beq @L_80236204
    cmplwi r29, 0x0
    beq @L_802361FC
    stw r26, 0x0(r29)
@L_802361FC:
    li r3, 0x2
    b @L_80236254
@L_80236204:
    cmplwi r27, 0x0
    beq @L_80236228
    rlwinm. r0, r24, 0, 24, 31
    bne @L_80236228
    cmplwi r29, 0x0
    beq @L_80236220
    stw r27, 0x0(r29)
@L_80236220:
    li r3, 0x2
    b @L_80236254
@L_80236228:
    cmplwi r28, 0x0
    beq @L_80236250
    rlwinm r0, r25, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80236250
    cmplwi r29, 0x0
    beq @L_80236248
    stw r28, 0x0(r29)
@L_80236248:
    li r3, 0x2
    b @L_80236254
@L_80236250:
    li r3, 0x0
@L_80236254:
    lmw r23, 0x4c(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
