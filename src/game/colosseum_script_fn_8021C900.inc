    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    bl fn_801F025C
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    mr r31, r3
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BEB4
    mr r0, r3
    li r3, 0x12
    mr r30, r0
    li r4, 0x0
    bl fn_801F025C
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x87
    li r6, 0x0
    bl fn_8012640C
    lwz r4, lbl_8047B610(r13)
    lbz r0, 0x1(r4)
    cmpwi r0, 0x1
    beq @L_8021C99C
    bge @L_8021C988
    cmpwi r0, 0x0
    bge @L_8021C994
    b @L_8021C9C8
@L_8021C988:
    cmpwi r0, 0x3
    bge @L_8021C9C8
    b @L_8021C9C4
@L_8021C994:
    mulli r30, r30, -0x1
    b @L_8021C9C8
@L_8021C99C:
    srawi r0, r30, 1
    addze. r30, r0
    bne @L_8021C9AC
    li r30, 0x1
@L_8021C9AC:
    srawi r0, r3, 1
    addze r0, r0
    cmpw r0, r30
    bge @L_8021C9C8
    mr r30, r0
    b @L_8021C9C8
@L_8021C9C4:
    rlwinm r30, r30, 1, 0, 30
@L_8021C9C8:
    mr r3, r31
    mr r7, r30
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x2
    stw r0, lbl_8047B610(r13)
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
