    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lbz r4, 0x80(r3)
    cmplwi r4, 0x0
    beq @L_800E6BB8
    subi r0, r4, 0x1
    stb r0, 0x80(r3)
    lbz r0, 0x80(r3)
    cmplwi r0, 0x0
    bne @L_800E6BB8
    lwz r0, 0x0(r3)
    lwz r3, 0x8(r3)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E6B60
    lwz r3, 0x10(r3)
    @L_800E6B60:
    cmplwi r3, 0x0
    bne @L_800E6B70
    li r3, 0x0
    b @L_800E6B74
    @L_800E6B70:
    lwz r3, 0x10(r3)
    @L_800E6B74:
    cmplwi r3, 0x0
    bne @L_800E6B98
    b @L_800E6BB8
    b @L_800E6B98
    @L_800E6B84:
    cmplwi r3, 0x0
    bne @L_800E6B94
    li r3, 0x0
    b @L_800E6B98
    @L_800E6B94:
    lwz r3, 0x8(r3)
    @L_800E6B98:
    cmplwi r3, 0x0
    bne @L_800E6BA8
    li r0, 0x0
    b @L_800E6BAC
    @L_800E6BA8:
    lwz r0, 0x8(r3)
    @L_800E6BAC:
    cmplwi r0, 0x0
    bne @L_800E6B84
    bl fn_801A02B0
    @L_800E6BB8:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
