    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    cmplwi r30, 0x0
    bne @L_80109BB8
    li r3, 0x0
    b @L_80109BE4
@L_80109BB8:
    rlwinm r31, r4, 0, 24, 31
@L_80109BBC:
    lbz r0, 0x0(r30)
    cmplwi r0, 0x1
    beq @L_80109BD0
    li r3, 0x0
    b @L_80109BE4
@L_80109BD0:
    cmplwi r31, 0x0
    beq @L_80109BE0
    bl fn_800F0308
    b @L_80109BBC
@L_80109BE0:
    li r3, 0x1
@L_80109BE4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
