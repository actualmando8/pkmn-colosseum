    nofralloc
    mflr r0
    lis r4, -32704
    stw r0, 0x4(r1)
    addi r0, r4, 48
    lis r4, 0x6666
    subf r0, r0, r3
    stwu r1, -0x20(r1)
    addi r3, r4, 0x6667
    mulhw r0, r3, r0
    stw r31, 0x1c(r1)
    srawi r0, r0, 4
    rlwinm r3, r0, 1, 31, 31
    add r0, r0, r3
    lis r3, -32704
    rlwinm r4, r0, 5, 0, 26
    addi r0, r3, -80
    add r31, r0, r4
    lwz r3, 0x0(r31)
    cmpwi r3, -0x1
    beq @L_800D06E0
    lwz r4, 0x4(r31)
    lwz r5, 0x8(r31)
    lwz r6, 0xc(r31)
    lwz r7, 0x10(r31)
    lwz r8, 0x14(r31)
    bl fn_800D00B0
    cmpwi r3, 0x0
    beq @L_800D06E0
    li r0, -0x1
    stw r0, 0x0(r31)
@L_800D06E0:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
