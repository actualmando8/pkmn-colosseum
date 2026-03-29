    nofralloc
    stwu r1, -0x20(r1)
    lwz r5, 0x60(r3)
    lis r6, lbl_80266BB0@ha
    addi r8, r6, lbl_80266BB0@l
    lbz r3, 0x95(r3)
    lwz r0, 0x8(r5)
    lwz r7, 0x0(r8)
    extsb r3, r3
    subfic r0, r0, 0x5
    lwz r6, 0x4(r8)
    add r9, r3, r0
    lwz r5, 0x8(r8)
    lwz r3, 0xc(r8)
    cmpwi r9, 0x0
    lwz r0, 0x10(r8)
    stw r7, 0x8(r1)
    stw r6, 0xc(r1)
    stw r5, 0x10(r1)
    stw r3, 0x14(r1)
    stw r0, 0x18(r1)
    blt @L_80014BFC
    cmpwi r9, 0x5
    blt @L_80014C04
    @L_80014BFC:
    li r3, 0x0
    b @L_80014C30
    @L_80014C04:
    slwi r0, r9, 2
    addi r3, r1, 0x8
    lha r5, 0x6(r4)
    lwzx r0, r3, r0
    cmpw r5, r0
    bne @L_80014C24
    li r0, 0xff
    b @L_80014C28
    @L_80014C24:
    li r0, 0x0
    @L_80014C28:
    stb r0, 0x67(r4)
    li r3, 0x0
    @L_80014C30:
    addi r1, r1, 0x20
    blr
