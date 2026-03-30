    nofralloc
    mflr r0
    lis r3, 0xcc00
    stw r0, 0x4(r1)
    addi r3, r3, 0x5000
    li r0, -0x89
    stwu r1, -0x2e0(r1)
    stw r31, 0x2dc(r1)
    addi r31, r4, 0x0
    lhz r5, 0xa(r3)
    and r0, r5, r0
    ori r0, r0, 0x20
    sth r0, 0xa(r3)
    addi r3, r1, 0x10
    bl OSClearContext
    addi r3, r1, 0x10
    bl OSSetCurrentContext
    lwz r12, lbl_8047A908(r13)
    cmplwi r12, 0x0
    beq @L_800ACBA8
    mtlr r12
    blrl
    @L_800ACBA8:
    addi r3, r1, 0x10
    bl OSClearContext
    mr r3, r31
    bl OSSetCurrentContext
    lwz r0, 0x2e4(r1)
    lwz r31, 0x2dc(r1)
    addi r1, r1, 0x2e0
    mtlr r0
    blr
