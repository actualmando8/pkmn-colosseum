    nofralloc
    stwu r1, -0x10(r1)
    lwz r3, 0x8(r3)
    lwz r0, 0x14(r3)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_800EE77C
    lwz r4, 0x18(r3)
    b @L_800EE780
    @L_800EE77C:
    li r4, 0x0
    @L_800EE780:
    cmplwi r4, 0x0
    bne @L_800EE790
    li r3, 0x0
    b @L_800EE7D8
    @L_800EE790:
    li r3, 0x0
    b @L_800EE7C0
    @L_800EE798:
    addis r0, r3, 0x1
    addi r3, r3, 0x1
    cmplwi r0, 0xffff
    bne @L_800EE7BC
    addic. r0, r1, 0x8
    beq @L_800EE7D4
    li r0, -0x1
    stw r0, 0x8(r1)
    b @L_800EE7D4
    @L_800EE7BC:
    lwz r4, 0x4(r4)
    @L_800EE7C0:
    cmplwi r4, 0x0
    bne @L_800EE798
    addic. r0, r1, 0x8
    beq @L_800EE7D4
    stw r3, 0x8(r1)
    @L_800EE7D4:
    lwz r3, 0x8(r1)
    @L_800EE7D8:
    addi r1, r1, 0x10
    blr
