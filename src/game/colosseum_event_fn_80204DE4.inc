    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r6, 0x0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r25, r3
    mr r26, r4
    mr r27, r5
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    bl fn_801F54A4
    cmplwi r25, 0x0
    rlwinm r29, r3, 0, 16, 31
    bne @L_80204E28
    li r3, 0x0
    b @L_80204F58
@L_80204E28:
    cmplwi r27, 0x0
    bne @L_80204E38
    li r31, 0x0
    b @L_80204E48
@L_80204E38:
    mr r3, r27
    mr r4, r29
    bl fn_801F0134
    mr r31, r3
@L_80204E48:
    mr r3, r25
    li r4, 0x0
    li r5, 0xfe
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    bne @L_80204E6C
    li r3, 0x0
    b @L_80204F58
@L_80204E6C:
    bl fn_801F1170
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80204E80
    li r3, 0x0
    b @L_80204F58
@L_80204E80:
    mr r3, r30
    bl fn_801F0898
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x13
    beq @L_80204E9C
    li r3, 0x0
    b @L_80204F58
@L_80204E9C:
    mr r3, r25
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    bne @L_80204EC0
    li r3, 0x0
    b @L_80204F58
@L_80204EC0:
    li r4, 0x0
    li r5, 0x27
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r28, r3, 0, 16, 31
    li r3, 0x0
    mr r4, r28
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm. r0, r26, 0, 16, 31
    rlwinm r27, r3, 0, 16, 31
    beq @L_80204F04
    cmplw r28, r0
    beq @L_80204F04
    li r3, 0x0
    b @L_80204F58
@L_80204F04:
    mr r3, r30
    li r4, 0x0
    li r5, 0x29
    li r6, 0x0
    bl fn_8011BEB4
    cmplwi r27, 0xb0
    rlwinm r3, r3, 0, 16, 31
    bne @L_80204F38
    mr r4, r25
    li r3, 0xe
    bl fn_801F025C
    mr r4, r29
    bl fn_801F0134
@L_80204F38:
    rlwinm. r4, r31, 0, 16, 31
    beq @L_80204F54
    rlwinm r0, r3, 0, 16, 31
    cmplw r0, r4
    beq @L_80204F54
    li r3, 0x0
    b @L_80204F58
@L_80204F54:
    li r3, 0x1
@L_80204F58:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
