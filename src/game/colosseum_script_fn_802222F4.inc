    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x14(r1)
    lwz r3, lbl_8047B610(r13)
    lbz r3, 0x1(r3)
    bl fn_801F025C
    lwz r3, lbl_8047B610(r13)
    lwz r4, 0x2(r3)
    cmplwi r4, 0x0
    beq @L_80222328
    lbz r0, 0x0(r4)
    b @L_8022232C
@L_80222328:
    li r0, 0x0
@L_8022232C:
    lwz r4, 0x6(r3)
    cmplwi r4, 0x0
    beq @L_80222340
    lhz r5, 0x0(r4)
    b @L_80222344
@L_80222340:
    li r5, 0x0
@L_80222344:
    lbz r3, 0x1(r3)
    rlwinm r4, r0, 0, 24, 31
    rlwinm r5, r5, 0, 16, 31
    bl fn_80221104
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0xa
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
