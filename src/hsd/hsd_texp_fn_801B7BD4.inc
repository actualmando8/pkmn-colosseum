    nofralloc
    cmplwi r3, 0x0
    bne @L_801B7BE4
    li r0, 0x0
    b @L_801B7C10
@L_801B7BE4:
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    bne @L_801B7BF8
    li r0, 0x2
    b @L_801B7C10
@L_801B7BF8:
    addis r0, r3, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B7C0C
    li r0, 0x3
    b @L_801B7C10
@L_801B7C0C:
    lwz r0, 0x0(r3)
@L_801B7C10:
    cmpwi r0, 0x4
    beq @L_801B7C50
    bgelr
    cmpwi r0, 0x1
    bnelr
    rlwinm r0, r4, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801B7C40
    lwz r4, 0x8(r3)
    addi r0, r4, 0x1
    stw r0, 0x8(r3)
    blr
@L_801B7C40:
    lwz r4, 0x14(r3)
    addi r0, r4, 0x1
    stw r0, 0x14(r3)
    blr
@L_801B7C50:
    lbz r4, 0x10(r3)
    addi r0, r4, 0x1
    stb r0, 0x10(r3)
    blr
