    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    li r30, 0x0
    stw r29, 0x14(r1)
    addi r29, r3, 0x0
    lwz r31, -24808(r13)
    b @L_8009FF04
@L_8009FEE4:
    lwz r12, 0x0(r31)
    addi r3, r29, 0x0
    mtlr r12
    blrl
    cntlzw r0, r3
    lwz r31, 0x8(r31)
    rlwinm r0, r0, 27, 5, 31
    or r30, r30, r0
@L_8009FF04:
    cmplwi r31, 0x0
    beq @L_8009FF14
    cmpwi r30, 0x0
    beq @L_8009FEE4
@L_8009FF14:
    bl __OSSyncSram
    cntlzw r0, r3
    rlwinm r0, r0, 27, 5, 31
    or. r30, r30, r0
    beq @L_8009FF30
    li r3, 0x0
    b @L_8009FF34
@L_8009FF30:
    li r3, 0x1
@L_8009FF34:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
