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
    beq @L_8021691C
    bge @L_802168C0
    cmpwi r0, 0x5b
    beq @L_802168F4
    bge @L_80216940
    cmpwi r0, 0x13
    beq @L_802168CC
    b @L_80216940
@L_802168C0:
    cmpwi r0, 0x154
    beq @L_802168CC
    b @L_80216940
@L_802168CC:
    mr r3, r31
    li r4, 0x1f
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80216940
    mr r3, r31
    li r4, 0x1f
    bl fn_80202810
    b @L_80216940
@L_802168F4:
    mr r3, r31
    li r4, 0x20
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80216940
    mr r3, r31
    li r4, 0x20
    bl fn_80202810
    b @L_80216940
@L_8021691C:
    mr r3, r31
    li r4, 0x21
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80216940
    mr r3, r31
    li r4, 0x21
    bl fn_80202810
@L_80216940:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
