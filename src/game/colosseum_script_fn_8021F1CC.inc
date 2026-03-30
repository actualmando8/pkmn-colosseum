    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r3, lbl_8047B610(r13)
    lbz r3, 0x1(r3)
    bl fn_801F025C
    lwz r4, lbl_8047B610(r13)
    mr r31, r3
    lbz r0, 0x2(r4)
    cmpwi r0, 0x2
    beq @L_8021F208
    bge @L_8021F22C
    b @L_8021F22C
@L_8021F208:
    bl fn_802037DC
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r3, r31
    bl fn_80203758
    mr r4, r3
    li r3, 0xe
    bl fn_80132A38
@L_8021F22C:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x3
    stw r0, lbl_8047B610(r13)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
