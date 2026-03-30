    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    bl fn_801F025C
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r31, r3
    bl fn_80205B8C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    li r4, 0x0
    li r5, 0xf0
    li r6, 0x0
    bl fn_8012640C
    rlwinm. r29, r3, 0, 16, 31
    beq @L_80218A4C
    cmplwi r29, 0x165
    beq @L_80218A4C
    cmplwi r29, 0xffff
    beq @L_80218A4C
    cmplwi r29, 0x164
    beq @L_80218A4C
    li r27, 0x0
    li r28, 0x0
    b @L_802188F4
@L_8021889C:
    rlwinm r26, r28, 0, 24, 31
    mr r3, r30
    mr r4, r26
    bl fn_80123CD4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802188F0
    mr r3, r30
    mr r6, r26
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r0, r3, 0, 16, 31
    cmplw r29, r0
    bne @L_802188F0
    mr r3, r30
    mr r6, r26
    li r4, 0x0
    li r5, 0x80
    bl fn_8012640C
    rlwinm r27, r3, 0, 24, 31
    b @L_80218900
@L_802188F0:
    addi r28, r28, 0x1
@L_802188F4:
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_8021889C
@L_80218900:
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_8021891C
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80218A58
@L_8021891C:
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x2
    bge @L_80218938
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80218A58
@L_80218938:
    bl fn_800E0C54
    rlwinm r0, r3, 0, 16, 31
    rlwinm r3, r3, 30, 0, 1
    rlwinm r4, r0, 1, 31, 31
    rlwinm r0, r27, 0, 24, 31
    subf r3, r4, r3
    rlwinm r3, r3, 2, 0, 31
    add r3, r3, r4
    addi r3, r3, 0x2
    rlwinm r26, r3, 0, 24, 31
    cmplw r0, r26
    bge @L_8021896C
    mr r26, r27
@L_8021896C:
    mr r4, r29
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    rlwinm r4, r26, 0, 24, 31
    li r3, 0x2f
    bl fn_80132A38
    subf r27, r26, r27
    mr r3, r30
    rlwinm r6, r28, 0, 24, 31
    li r4, 0x0
    rlwinm r7, r27, 0, 24, 31
    li r5, 0x80
    bl fn_801254B4
    mr r3, r31
    li r4, 0x10
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80218A2C
    mr r3, r31
    li r4, 0x31
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80218A2C
    mr r3, r31
    li r4, 0x31
    bl fn_80201890
    rlwinm r26, r28, 0, 24, 31
    li r0, 0x1
    slw r0, r0, r26
    and. r0, r3, r0
    bne @L_80218A2C
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80218A2C
    mr r3, r31
    mr r5, r26
    li r4, 0x80
    li r6, 0x0
    bl fn_801FE7EC
@L_80218A2C:
    rlwinm. r0, r27, 0, 24, 31
    bne @L_80218A3C
    mr r3, r31
    bl fn_80200B10
@L_80218A3C:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    b @L_80218A58
@L_80218A4C:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_80218A58:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
