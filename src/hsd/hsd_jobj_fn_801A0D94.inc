    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r4
    stw r28, 0x10(r1)
    mr. r28, r3
    lis r3, lbl_80274AA0@ha
    addi r31, r3, lbl_80274AA0@l
    beq @L_801A0F9C
    cmplwi r29, 0x0
    bne @L_801A0DD0
    b @L_801A0F9C
    @L_801A0DD0:
    lwz r3, 0x80(r28)
    lwz r4, 0x3c(r29)
    bl fn_801AEBE4
    lwz r0, 0x14(r28)
    rlwinm. r0, r0, 0, 19, 19
    beq @L_801A0F7C
    lwz r30, 0x10(r28)
    cmplwi r30, 0x0
    beq @L_801A0F18
    lis r3, 0x1
    lhz r4, 0x4(r30)
    subi r3, r3, 0x1
    clrlwi r0, r3, 16
    subf r0, r4, r0
    cntlzw r0, r0
    srwi. r3, r0, 5
    beq @L_801A0E18
    b @L_801A0E30
    @L_801A0E18:
    lhz r0, 0x4(r30)
    lhz r3, 0x4(r30)
    cntlzw r4, r0
    subi r0, r3, 0x1
    sth r0, 0x4(r30)
    srwi r3, r4, 5
    @L_801A0E30:
    cmpwi r3, 0x0
    beq @L_801A0F18
    lhz r3, 0x6(r30)
    subic. r0, r3, 0x1
    bge @L_801A0E78
    cmplwi r30, 0x0
    beq @L_801A0F18
    lwz r4, 0x0(r30)
    mr r3, r30
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r30)
    mr r3, r30
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
    b @L_801A0F18
    @L_801A0E78:
    lhz r3, 0x6(r30)
    addi r0, r3, 0x1
    sth r0, 0x6(r30)
    lhz r0, 0x6(r30)
    cmplwi r0, 0x0
    bne @L_801A0EA0
    addi r3, r31, 0x54
    addi r5, r31, 0x60
    li r4, 0x9e
    bl fn_80196E10
    @L_801A0EA0:
    lwz r4, 0x0(r30)
    mr r3, r30
    lwz r12, 0x4c(r4)
    mtctr r12
    bctrl
    lhz r0, 0x6(r30)
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A0EC8
    b @L_801A0EE0
    @L_801A0EC8:
    lhz r3, 0x6(r30)
    subi r0, r3, 0x1
    sth r0, 0x6(r30)
    lhz r0, 0x6(r30)
    cntlzw r0, r0
    srwi r0, r0, 5
    @L_801A0EE0:
    cmpwi r0, 0x0
    beq @L_801A0F18
    cmplwi r30, 0x0
    beq @L_801A0F18
    lwz r4, 0x0(r30)
    mr r3, r30
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r30)
    mr r3, r30
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
    @L_801A0F18:
    lwz r4, 0x8(r29)
    li r3, 0x0
    li r5, 0x0
    bl fn_8019C128
    stw r3, 0x10(r28)
    lwz r0, 0x10(r28)
    cmplwi r0, 0x0
    bne @L_801A0F48
    addi r5, r31, 0x210
    la r3, lbl_8047DB20(r2)
    li r4, 0x45f
    bl fn_80196E10
    @L_801A0F48:
    lwz r4, 0x10(r28)
    cmplwi r4, 0x0
    beq @L_801A0F7C
    lhz r3, 0x4(r4)
    addi r0, r3, 0x1
    sth r0, 0x4(r4)
    lhz r0, 0x4(r4)
    cmplwi r0, 0xffff
    bne @L_801A0F7C
    addi r3, r31, 0x54
    addi r5, r31, 0xc4
    li r4, 0x5d
    bl fn_80196E10
    @L_801A0F7C:
    lwz r0, 0x14(r28)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A0F9C
    lwz r3, 0x18(r28)
    lwz r4, 0x10(r29)
    bl fn_801991F8
    @L_801A0F9C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
