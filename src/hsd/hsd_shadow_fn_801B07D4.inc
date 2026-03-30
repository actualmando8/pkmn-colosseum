    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr. r30, r3
    beq @L_801B0868
    cmplwi r31, 0x0
    bne @L_801B0800
    b @L_801B0868
@L_801B0800:
    lwz r3, 0x0(r30)
    b @L_801B0818
@L_801B0808:
    lwz r0, 0x4(r3)
    cmplw r0, r31
    beq @L_801B0868
    lwz r3, 0x0(r3)
@L_801B0818:
    cmplwi r3, 0x0
    bne @L_801B0808
    lwz r3, 0x0(r30)
    mr r4, r31
    bl fn_801A3EB4
    cmplwi r31, 0x0
    stw r3, 0x0(r30)
    beq @L_801B0868
    lhz r3, 0x4(r31)
    addi r0, r3, 0x1
    sth r0, 0x4(r31)
    lhz r0, 0x4(r31)
    cmplwi r0, 0xffff
    bne @L_801B0868
    lis r3, 0x8027
    lis r5, 0x8027
    addi r3, r3, 21268
    li r4, 0x5d
    addi r5, r5, 21280
    bl fn_80196E10
@L_801B0868:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
