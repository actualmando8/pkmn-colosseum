    nofralloc
    mflr r0
    lis r4, 0x8048
    stw r0, 0x4(r1)
    slwi r3, r3, 8
    addi r0, r4, -31776
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    add r31, r0, r3
    lwz r0, 0x20(r31)
    cmpwi r0, 0x0
    bne @L_8025F570
    lwz r3, 0x18(r31)
    addi r4, r31, 0x5
    li r5, 0x4
    bl memcpy
    lbz r0, 0x9(r31)
    lwz r3, 0x14(r31)
    andi. r0, r0, 0x3a
    stb r0, 0x0(r3)
@L_8025F570:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
