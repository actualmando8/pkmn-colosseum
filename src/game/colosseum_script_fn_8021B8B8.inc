    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, lbl_80379F58@ha
    stw r0, 0x14(r1)
    addi r3, r3, lbl_80379F58@l
    addis r3, r3, 0x1
    lwz r4, lbl_8047B610(r13)
    lbz r0, 0x601e(r3)
    lbz r5, 0x1(r4)
    lwz r6, 0x2(r4)
    rlwinm r3, r0, 0, 24, 27
    rlwinm r4, r0, 0, 28, 31
    bl fn_8021B910
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021B900
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x6
    stw r0, lbl_8047B610(r13)
@L_8021B900:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
