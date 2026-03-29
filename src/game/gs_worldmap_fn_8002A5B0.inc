    nofralloc
    lbz r0, 0x95(r3)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_8002A5C8
    cmpwi r0, 0x2
    blt @L_8002A5D0
    @L_8002A5C8:
    li r3, 0x0
    blr
    @L_8002A5D0:
    mulli r0, r0, 0xc
    lis r3, lbl_80266E58@ha
    lha r5, 0x6(r4)
    addi r3, r3, lbl_80266E58@l
    add r3, r3, r0
    lwz r0, 0x4(r3)
    cmpw r0, r5
    beq @L_8002A5FC
    lwz r0, 0x8(r3)
    cmpw r0, r5
    bne @L_8002A608
    @L_8002A5FC:
    li r0, 0xff
    stb r0, 0x67(r4)
    b @L_8002A610
    @L_8002A608:
    li r0, 0x0
    stb r0, 0x67(r4)
    @L_8002A610:
    li r3, 0x0
    blr
