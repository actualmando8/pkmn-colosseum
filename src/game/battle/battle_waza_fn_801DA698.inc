    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r6
    bne @L_801DA6C0
    li r3, 0x0
    b @L_801DA734
@L_801DA6C0:
    bl fn_801DD0C8
    mr. r31, r3
    bne @L_801DA6D4
    li r3, 0x1
    b @L_801DA734
@L_801DA6D4:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x6
    bne @L_801DA70C
    lbz r0, 0x14(r31)
    cmplwi r0, 0x0
    beq @L_801DA704
    lbz r0, 0x15(r31)
    extsb r0, r0
    cmpwi r0, -0x1
    beq @L_801DA704
    li r3, 0x0
    b @L_801DA734
@L_801DA704:
    li r3, 0x1
    b @L_801DA734
@L_801DA70C:
    lwz r3, 0x3c(r31)
    mr r4, r31
    mr r5, r30
    bl fn_801DB3F8
    lwz r0, 0x0(r31)
    rlwinm r4, r3, 1, 31, 31
    srawi r5, r0, 31
    subfc r0, r3, r0
    adde r0, r5, r4
    rlwinm r3, r0, 0, 24, 31
@L_801DA734:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
