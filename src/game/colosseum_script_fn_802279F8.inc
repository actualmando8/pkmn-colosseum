    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r31, r4
    mr r27, r5
    mr r26, r6
    mr r30, r3
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BEB4
    mr r29, r3
    mr r4, r27
    li r3, 0x0
    li r5, 0x7
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r27, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x43
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80227A64
    rlwinm. r0, r26, 0, 24, 31
    beq @L_80227C2C
@L_80227A64:
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x3f
    beq @L_80227C2C
    cmplwi r0, 0x43
    bne @L_80227A80
    li r28, 0x0
    b @L_80227AA4
@L_80227A80:
    cmplwi r0, 0x42
    bne @L_80227A90
    li r28, 0x5
    b @L_80227AA4
@L_80227A90:
    cmplwi r0, 0x41
    bne @L_80227C2C
    li r28, 0x14
    b @L_80227AA4
    b @L_80227C2C
@L_80227AA4:
    rlwinm r0, r26, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80227AE4
    rlwinm r4, r28, 0, 24, 31
    li r0, 0xa
    mullw r3, r29, r4
    divw. r7, r3, r0
    bne @L_80227AD0
    cmplwi r4, 0x0
    beq @L_80227AD0
    li r7, 0x1
@L_80227AD0:
    mr r3, r30
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
@L_80227AE4:
    rlwinm r0, r28, 0, 24, 31
    cmpwi r0, 0x5
    beq @L_80227B50
    bge @L_80227B00
    cmpwi r0, 0x0
    beq @L_80227B0C
    b @L_80227C2C
@L_80227B00:
    cmpwi r0, 0x14
    beq @L_80227BC0
    b @L_80227C2C
@L_80227B0C:
    mr r3, r30
    mr r4, r31
    bl fn_8020981C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80227B34
    mr r3, r30
    mr r4, r31
    li r5, 0x0
    bl fn_802097C8
@L_80227B34:
    mr r3, r30
    li r4, 0x42
    bl fn_80209960
    mr r3, r30
    li r4, 0x41
    bl fn_80209960
    b @L_80227C2C
@L_80227B50:
    cmplwi r27, 0x0
    beq @L_80227C2C
    mr r3, r30
    bl fn_802096E8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80227C2C
    mr r3, r30
    li r4, 0x41
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80227B94
    mr r3, r30
    li r4, 0x41
    bl fn_80209960
    b @L_80227C2C
@L_80227B94:
    mr r3, r30
    mr r4, r31
    bl fn_8020981C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80227C2C
    mr r3, r30
    mr r4, r31
    li r5, 0x0
    bl fn_802097C8
    b @L_80227C2C
@L_80227BC0:
    cmplwi r27, 0x0
    beq @L_80227C2C
    mr r3, r30
    bl fn_802096E8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80227C2C
    mr r3, r30
    li r4, 0x42
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80227C04
    mr r3, r30
    li r4, 0x42
    bl fn_80209960
    b @L_80227C2C
@L_80227C04:
    mr r3, r30
    mr r4, r31
    bl fn_8020981C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80227C2C
    mr r3, r30
    mr r4, r31
    li r5, 0x0
    bl fn_802097C8
@L_80227C2C:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
