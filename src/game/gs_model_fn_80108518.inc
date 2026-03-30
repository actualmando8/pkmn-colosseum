    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_80108550
    li r0, 0x0
    stw r0, 0x0(r30)
    sth r0, 0x4(r30)
    b @L_80108568
@L_80108550:
    li r4, 0x0
    li r5, 0x3c
    bl memset
    rlwinm r3, r31, 0, 16, 31
    bl fn_8005D830
    stw r3, 0x0(r30)
@L_80108568:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
