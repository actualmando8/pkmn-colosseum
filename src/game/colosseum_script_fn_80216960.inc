    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_801F025C
    mr r31, r3
    bl fn_80205184
    rlwinm r0, r3, 0, 16, 31
    cmpwi r0, 0x123
    beq @L_80216A10
    bge @L_802169AC
    cmpwi r0, 0x5b
    beq @L_802169E4
    bge @L_80216A38
    cmpwi r0, 0x13
    beq @L_802169B8
    b @L_80216A38
@L_802169AC:
    cmpwi r0, 0x154
    beq @L_802169B8
    b @L_80216A38
@L_802169B8:
    mr r3, r31
    li r4, 0x1f
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80216A38
    mr r3, r31
    li r4, 0x1f
    li r5, 0x0
    bl fn_8020248C
    b @L_80216A38
@L_802169E4:
    mr r3, r31
    li r4, 0x20
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80216A38
    mr r3, r31
    li r4, 0x20
    li r5, 0x0
    bl fn_8020248C
    b @L_80216A38
@L_80216A10:
    mr r3, r31
    li r4, 0x21
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80216A38
    mr r3, r31
    li r4, 0x21
    li r5, 0x0
    bl fn_8020248C
@L_80216A38:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
