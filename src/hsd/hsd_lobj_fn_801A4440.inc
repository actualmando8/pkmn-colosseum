    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r4
    stw r30, 0x18(r1)
    mr. r30, r3
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    lwz r0, 0xc(r4)
    stw r0, 0x8(r1)
    lwz r0, 0x8(r1)
    stw r0, 0x10(r3)
    lhz r3, 0x8(r4)
    beq @L_801A4488
    lhz r0, 0x8(r30)
    or r0, r0, r3
    sth r0, 0x8(r30)
    @L_801A4488:
    lhz r0, 0x8(r31)
    clrlwi r0, r0, 30
    cmpwi r0, 0x2
    beq @L_801A4554
    bge @L_801A44AC
    cmpwi r0, 0x0
    beq @L_801A488C
    bge @L_801A44B8
    b @L_801A4868
    @L_801A44AC:
    cmpwi r0, 0x4
    bge @L_801A4868
    b @L_801A466C
    @L_801A44B8:
    lwz r3, 0x10(r31)
    bl fn_801917D0
    cmplwi r30, 0x0
    mr r31, r3
    beq @L_801A488C
    lwz r29, 0x18(r30)
    cmplwi r29, 0x0
    beq @L_801A454C
    lis r3, 0x1
    lhz r4, 0x4(r29)
    subi r3, r3, 0x1
    clrlwi r0, r3, 16
    subf r0, r4, r0
    cntlzw r0, r0
    srwi. r3, r0, 5
    beq @L_801A44FC
    b @L_801A4514
    @L_801A44FC:
    lhz r0, 0x4(r29)
    lhz r3, 0x4(r29)
    cntlzw r4, r0
    subi r0, r3, 0x1
    sth r0, 0x4(r29)
    srwi r3, r4, 5
    @L_801A4514:
    cmpwi r3, 0x0
    beq @L_801A454C
    cmplwi r29, 0x0
    beq @L_801A454C
    lwz r4, 0x0(r29)
    mr r3, r29
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r29)
    mr r3, r29
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
    @L_801A454C:
    stw r31, 0x18(r30)
    b @L_801A488C
    @L_801A4554:
    lwz r3, 0x10(r31)
    bl fn_801917D0
    cmplwi r30, 0x0
    mr r29, r3
    beq @L_801A45EC
    lwz r28, 0x18(r30)
    cmplwi r28, 0x0
    beq @L_801A45E8
    lis r3, 0x1
    lhz r4, 0x4(r28)
    subi r3, r3, 0x1
    clrlwi r0, r3, 16
    subf r0, r4, r0
    cntlzw r0, r0
    srwi. r3, r0, 5
    beq @L_801A4598
    b @L_801A45B0
    @L_801A4598:
    lhz r0, 0x4(r28)
    lhz r3, 0x4(r28)
    cntlzw r4, r0
    subi r0, r3, 0x1
    sth r0, 0x4(r28)
    srwi r3, r4, 5
    @L_801A45B0:
    cmpwi r3, 0x0
    beq @L_801A45E8
    cmplwi r28, 0x0
    beq @L_801A45E8
    lwz r4, 0x0(r28)
    mr r3, r28
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r28)
    mr r3, r28
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
    @L_801A45E8:
    stw r29, 0x18(r30)
    @L_801A45EC:
    lhz r0, 0xa(r31)
    clrlwi. r0, r0, 31
    beq @L_801A463C
    cmplwi r30, 0x0
    beq @L_801A460C
    lhz r0, 0x8(r30)
    ori r0, r0, 0x40
    sth r0, 0x8(r30)
    @L_801A460C:
    lwz r5, 0x18(r31)
    cmplwi r30, 0x0
    lwz r4, 0x18(r31)
    lwz r3, 0x18(r31)
    lfs f2, 0x14(r5)
    lfs f1, 0x10(r4)
    lfs f0, 0xc(r3)
    beq @L_801A488C
    stfs f0, 0x2c(r30)
    stfs f1, 0x30(r30)
    stfs f2, 0x34(r30)
    b @L_801A488C
    @L_801A463C:
    lwz r5, 0x18(r31)
    cmplwi r30, 0x0
    lwz r4, 0x18(r31)
    lwz r3, 0x18(r31)
    lwz r0, 0x8(r5)
    lfs f1, 0x0(r4)
    lfs f0, 0x4(r3)
    beq @L_801A488C
    stfs f0, 0x2c(r30)
    stfs f1, 0x28(r30)
    stw r0, 0x30(r30)
    b @L_801A488C
    @L_801A466C:
    lwz r3, 0x10(r31)
    bl fn_801917D0
    cmplwi r30, 0x0
    mr r29, r3
    beq @L_801A4704
    lwz r28, 0x18(r30)
    cmplwi r28, 0x0
    beq @L_801A4700
    lis r3, 0x1
    lhz r4, 0x4(r28)
    subi r3, r3, 0x1
    clrlwi r0, r3, 16
    subf r0, r4, r0
    cntlzw r0, r0
    srwi. r3, r0, 5
    beq @L_801A46B0
    b @L_801A46C8
    @L_801A46B0:
    lhz r0, 0x4(r28)
    lhz r3, 0x4(r28)
    cntlzw r4, r0
    subi r0, r3, 0x1
    sth r0, 0x4(r28)
    srwi r3, r4, 5
    @L_801A46C8:
    cmpwi r3, 0x0
    beq @L_801A4700
    cmplwi r28, 0x0
    beq @L_801A4700
    lwz r4, 0x0(r28)
    mr r3, r28
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r28)
    mr r3, r28
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
    @L_801A4700:
    stw r29, 0x18(r30)
    @L_801A4704:
    lwz r3, 0x14(r31)
    bl fn_801917D0
    cmplwi r30, 0x0
    mr r29, r3
    beq @L_801A479C
    lwz r28, 0x1c(r30)
    cmplwi r28, 0x0
    beq @L_801A4798
    lis r3, 0x1
    lhz r4, 0x4(r28)
    subi r3, r3, 0x1
    clrlwi r0, r3, 16
    subf r0, r4, r0
    cntlzw r0, r0
    srwi. r3, r0, 5
    beq @L_801A4748
    b @L_801A4760
    @L_801A4748:
    lhz r0, 0x4(r28)
    lhz r3, 0x4(r28)
    cntlzw r4, r0
    subi r0, r3, 0x1
    sth r0, 0x4(r28)
    srwi r3, r4, 5
    @L_801A4760:
    cmpwi r3, 0x0
    beq @L_801A4798
    cmplwi r28, 0x0
    beq @L_801A4798
    lwz r4, 0x0(r28)
    mr r3, r28
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r28)
    mr r3, r28
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
    @L_801A4798:
    stw r29, 0x1c(r30)
    @L_801A479C:
    lhz r0, 0xa(r31)
    clrlwi. r0, r0, 31
    beq @L_801A4818
    cmplwi r30, 0x0
    beq @L_801A47BC
    lhz r0, 0x8(r30)
    ori r0, r0, 0x40
    sth r0, 0x8(r30)
    @L_801A47BC:
    lwz r8, 0x18(r31)
    cmplwi r30, 0x0
    lwz r7, 0x18(r31)
    lwz r6, 0x18(r31)
    lwz r5, 0x18(r31)
    lwz r4, 0x18(r31)
    lwz r3, 0x18(r31)
    lfs f5, 0x14(r8)
    lfs f4, 0x10(r7)
    lfs f3, 0xc(r6)
    lfs f2, 0x8(r5)
    lfs f1, 0x4(r4)
    lfs f0, 0x0(r3)
    beq @L_801A4800
    stfs f0, 0x20(r30)
    stfs f1, 0x24(r30)
    stfs f2, 0x28(r30)
    @L_801A4800:
    cmplwi r30, 0x0
    beq @L_801A488C
    stfs f3, 0x2c(r30)
    stfs f4, 0x30(r30)
    stfs f5, 0x34(r30)
    b @L_801A488C
    @L_801A4818:
    lwz r5, 0x18(r31)
    cmplwi r30, 0x0
    lwz r4, 0x18(r31)
    lwz r3, 0x18(r31)
    lwz r0, 0x10(r5)
    lfs f1, 0x8(r4)
    lfs f0, 0xc(r3)
    beq @L_801A4844
    stfs f0, 0x2c(r30)
    stfs f1, 0x28(r30)
    stw r0, 0x30(r30)
    @L_801A4844:
    lwz r4, 0x18(r31)
    cmplwi r30, 0x0
    lwz r3, 0x18(r31)
    lwz r0, 0x4(r4)
    lfs f0, 0x0(r3)
    beq @L_801A488C
    stfs f0, 0x20(r30)
    stw r0, 0x24(r30)
    b @L_801A488C
    @L_801A4868:
    lis r3, lbl_80274D94@ha
    lhz r4, 0x8(r31)
    addi r3, r3, lbl_80274D94@l
    crxor 6, 6, 6
    bl OSReport
    la r3, lbl_8047DBB8(r2)
    li r4, 0x659
    la r5, lbl_8047DBC8(r2)
    bl fn_80196D78
    @L_801A488C:
    lwz r0, 0x24(r1)
    li r3, 0x0
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
