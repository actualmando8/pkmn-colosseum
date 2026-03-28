    nofralloc
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    dcbz r0, r4
    lwz r12, -21312(r13)
    lwz r11, 0x6a4(r3)
    addi r9, r12, 0x20
    lwz r10, 0x6a0(r3)
    addi r5, r11, 0x4
    cmpwi r11, 0x1c
    rlwnm r8, r10, r5, 27, 31
    bgt @L_801EC458
    lbzx r5, r12, r8
    lbzx r9, r9, r8
    cmpwi r5, 0xff
    beq @L_801EC3B4
    add r11, r11, r9
    stw r11, 0x6a4(r3)
    b @L_801EC5FC
@L_801EC3B4:
    addi r6, r12, 0x44
    addi r11, r11, 0x5
    li r0, 0x14
    li r5, 0x5
    addi r6, r6, 0x14
@L_801EC3C8:
    cmpwi r11, 0x21
    slwi r8, r8, 1
    beq @L_801EC3E8
    rlwnm r9, r10, r11, 31, 31
    lwzu r0, 0x4(r6)
    or r8, r8, r9
    addi r11, r11, 0x1
    b @L_801EC42C
@L_801EC3E8:
    lwz r9, 0x69c(r3)
    li r11, 0x1
    lwzu r10, 0x4(r9)
    lwzu r0, 0x4(r6)
    stw r9, 0x69c(r3)
    rlwimi r8, r10, 1, 31, 31
    stw r10, 0x6a0(r3)
    b @L_801EC418
@L_801EC408:
    slwi r8, r8, 1
    rlwnm r9, r10, r11, 31, 31
    lwzu r0, 0x4(r6)
    or r8, r8, r9
@L_801EC418:
    cmpw r8, r0
    addi r11, r11, 0x1
    addi r5, r5, 0x1
    bgt @L_801EC408
    b @L_801EC438
@L_801EC42C:
    cmpw r8, r0
    addi r5, r5, 0x1
    bgt @L_801EC3C8
@L_801EC438:
    stw r11, 0x6a4(r3)
    slwi r0, r5, 2
    add r5, r12, r0
    lwz r6, 0x40(r12)
    lwz r0, 0x8c(r5)
    add r0, r0, r6
    lbzx r5, r8, r0
    b @L_801EC5FC
@L_801EC458:
    cmpwi r11, 0x21
    lwz r8, 0x69c(r3)
    beq @L_801EC514
    cmpwi r11, 0x20
    rlwnm r5, r10, r5, 27, 31
    beq @L_801EC498
    lbzx r8, r12, r5
    lbzx r9, r9, r5
    cmpwi r8, 0xff
    add r5, r11, r9
    beq @L_801EC578
    cmpwi r5, 0x21
    stw r5, 0x6a4(r3)
    bgt @L_801EC578
    mr r5, r8
    b @L_801EC5FC
@L_801EC498:
    lwzu r10, 0x4(r8)
    stw r8, 0x69c(r3)
    rlwimi r5, r10, 4, 28, 31
    lbzx r8, r12, r5
    lbzx r9, r9, r5
    stw r10, 0x6a0(r3)
    cmpwi r8, 0xff
    stw r9, 0x6a4(r3)
    beq @L_801EC4C4
    mr r5, r8
    b @L_801EC5FC
@L_801EC4C4:
    addi r6, r12, 0x44
    li r11, 0x14
    addi r6, r6, 0x14
    slwi r8, r5, 27
    li r11, 0x5
    rlwimi r8, r10, 31, 1, 31
@L_801EC4DC:
    subfic r10, r11, 0x1f
    lwzu r0, 0x4(r6)
    srw r5, r8, r10
    cmpw r5, r0
    addi r11, r11, 0x1
    bgt @L_801EC4DC
    stw r11, 0x6a4(r3)
@L_801EC4F8:
    slwi r0, r11, 2
    lwz r7, 0x40(r12)
    add r6, r12, r0
    lwz r0, 0x8c(r6)
    add r0, r0, r7
    lbzx r5, r5, r0
    b @L_801EC5FC
@L_801EC514:
    lwzu r10, 0x4(r8)
    srwi r5, r10, 27
    stw r8, 0x69c(r3)
    lbzx r11, r12, r5
    lbzx r9, r9, r5
    cmpwi r11, 0xff
    stw r10, 0x6a0(r3)
    addi r9, r9, 0x1
    beq @L_801EC544
    stw r9, 0x6a4(r3)
    mr r5, r11
    b @L_801EC5FC
@L_801EC544:
    li r11, 0x5
    li r6, 0x14
@L_801EC54C:
    subfic r8, r11, 0x1f
    addi r11, r11, 0x1
    addi r6, r6, 0x4
    srw r5, r10, r8
    add r7, r12, r6
    lwz r0, 0x44(r7)
    cmpw r5, r0
    bgt @L_801EC54C
    addi r0, r11, 0x1
    stw r0, 0x6a4(r3)
    b @L_801EC4F8
@L_801EC578:
    subfic r0, r11, 0x21
    li r5, -0x1
    slw r7, r5, r0
    andc r5, r10, r7
    addi r7, r12, 0x44
    lwz r8, 0x69c(r3)
    subfic r6, r11, 0x21
    addi r11, r6, 0x1
    slwi r6, r6, 2
    lwzu r10, 0x4(r8)
    add r7, r7, r6
    stw r8, 0x69c(r3)
    slwi r5, r5, 1
    stw r10, 0x6a0(r3)
    rlwimi r5, r10, 1, 31, 31
    lwzu r6, 0x4(r7)
    li r8, 0x2
    b @L_801EC5D4
@L_801EC5C0:
    slwi r5, r5, 1
    addi r11, r11, 0x1
    lwzu r6, 0x4(r7)
    add r5, r5, r9
    addi r8, r8, 0x1
@L_801EC5D4:
    cmpw r5, r6
    rlwnm r9, r10, r8, 31, 31
    bgt @L_801EC5C0
    stw r8, 0x6a4(r3)
    slwi r0, r11, 2
    add r6, r12, r0
    lwz r7, 0x40(r12)
    lwz r0, 0x8c(r6)
    add r0, r0, r7
    lbzx r5, r5, r0
@L_801EC5FC:
    li r0, 0x20
    dcbz r4, r0
    li r7, 0x0
    li r0, 0x40
    dcbz r4, r0
    cmpwi r5, 0x0
    beq @L_801EC698
    lwz r9, 0x6a4(r3)
    subfic r10, r9, 0x21
    lwz r7, 0x6a0(r3)
    subf. r11, r10, r5
    subi r12, r9, 0x1
    bgt @L_801EC648
    add r0, r9, r5
    slw r9, r7, r12
    stw r0, 0x6a4(r3)
    subfic r0, r5, 0x20
    srw r7, r9, r0
    b @L_801EC674
@L_801EC648:
    lwz r9, 0x69c(r3)
    slw r0, r7, r12
    lwzu r7, 0x4(r9)
    addi r11, r11, 0x1
    stw r7, 0x6a0(r3)
    srw r7, r7, r10
    stw r9, 0x69c(r3)
    add r0, r7, r0
    stw r11, 0x6a4(r3)
    subfic r11, r5, 0x20
    srw r7, r0, r11
@L_801EC674:
    extsh r0, r7
    cntlzw r6, r0
    subfic r0, r5, 0x20
    cmpw r6, r0
    ble @L_801EC698
    li r0, -0x1
    slw r0, r0, r5
    add r7, r0, r7
    addi r7, r7, 0x1
@L_801EC698:
    li r0, 0x60
    dcbz r4, r0
    lha r0, 0x690(r3)
    lis r5, -32728
    addi r8, r5, -25880
    add r0, r0, r7
    sth r0, 0x690(r3)
    li r6, 0x1
    sth r0, 0x0(r4)
    b @L_801EC9F8
@L_801EC6C0:
    lwz r30, -21216(r13)
    lwz r31, 0x6a4(r3)
    addi r11, r30, 0x20
    lwz r12, 0x6a0(r3)
    addi r5, r31, 0x4
    cmpwi r31, 0x1c
    rlwnm r10, r12, r5, 27, 31
    bgt @L_801EC7A0
    lbzx r5, r30, r10
    lbzx r11, r11, r10
    cmpwi r5, 0xff
    beq @L_801EC6FC
    add r31, r31, r11
    stw r31, 0x6a4(r3)
    b @L_801EC944
@L_801EC6FC:
    addi r7, r30, 0x44
    addi r31, r31, 0x5
    li r0, 0x14
    li r5, 0x5
    addi r7, r7, 0x14
@L_801EC710:
    cmpwi r31, 0x21
    slwi r10, r10, 1
    beq @L_801EC730
    rlwnm r11, r12, r31, 31, 31
    lwzu r0, 0x4(r7)
    or r10, r10, r11
    addi r31, r31, 0x1
    b @L_801EC774
@L_801EC730:
    lwz r11, 0x69c(r3)
    li r31, 0x1
    lwzu r12, 0x4(r11)
    lwzu r0, 0x4(r7)
    stw r11, 0x69c(r3)
    rlwimi r10, r12, 1, 31, 31
    stw r12, 0x6a0(r3)
    b @L_801EC760
@L_801EC750:
    slwi r10, r10, 1
    rlwnm r11, r12, r31, 31, 31
    lwzu r0, 0x4(r7)
    or r10, r10, r11
@L_801EC760:
    cmpw r10, r0
    addi r31, r31, 0x1
    addi r5, r5, 0x1
    bgt @L_801EC750
    b @L_801EC780
@L_801EC774:
    cmpw r10, r0
    addi r5, r5, 0x1
    bgt @L_801EC710
@L_801EC780:
    stw r31, 0x6a4(r3)
    slwi r0, r5, 2
    add r5, r30, r0
    lwz r7, 0x40(r30)
    lwz r0, 0x8c(r5)
    add r0, r0, r7
    lbzx r5, r10, r0
    b @L_801EC944
@L_801EC7A0:
    cmpwi r31, 0x21
    lwz r10, 0x69c(r3)
    beq @L_801EC85C
    cmpwi r31, 0x20
    rlwnm r5, r12, r5, 27, 31
    beq @L_801EC7E0
    lbzx r10, r30, r5
    lbzx r11, r11, r5
    cmpwi r10, 0xff
    add r5, r31, r11
    beq @L_801EC8C0
    cmpwi r5, 0x21
    stw r5, 0x6a4(r3)
    bgt @L_801EC8C0
    mr r5, r10
    b @L_801EC944
@L_801EC7E0:
    lwzu r12, 0x4(r10)
    stw r10, 0x69c(r3)
    rlwimi r5, r12, 4, 28, 31
    lbzx r10, r30, r5
    lbzx r11, r11, r5
    stw r12, 0x6a0(r3)
    cmpwi r10, 0xff
    stw r11, 0x6a4(r3)
    beq @L_801EC80C
    mr r5, r10
    b @L_801EC944
@L_801EC80C:
    addi r7, r30, 0x44
    li r31, 0x14
    addi r7, r7, 0x14
    slwi r10, r5, 27
    li r31, 0x5
    rlwimi r10, r12, 31, 1, 31
@L_801EC824:
    subfic r12, r31, 0x1f
    lwzu r0, 0x4(r7)
    srw r5, r10, r12
    cmpw r5, r0
    addi r31, r31, 0x1
    bgt @L_801EC824
    stw r31, 0x6a4(r3)
@L_801EC840:
    slwi r0, r31, 2
    lwz r9, 0x40(r30)
    add r7, r30, r0
    lwz r0, 0x8c(r7)
    add r0, r0, r9
    lbzx r5, r5, r0
    b @L_801EC944
@L_801EC85C:
    lwzu r12, 0x4(r10)
    srwi r5, r12, 27
    stw r10, 0x69c(r3)
    lbzx r31, r30, r5
    lbzx r11, r11, r5
    cmpwi r31, 0xff
    stw r12, 0x6a0(r3)
    addi r11, r11, 0x1
    beq @L_801EC88C
    stw r11, 0x6a4(r3)
    mr r5, r31
    b @L_801EC944
@L_801EC88C:
    li r31, 0x5
    li r7, 0x14
@L_801EC894:
    subfic r10, r31, 0x1f
    addi r31, r31, 0x1
    addi r7, r7, 0x4
    srw r5, r12, r10
    add r9, r30, r7
    lwz r0, 0x44(r9)
    cmpw r5, r0
    bgt @L_801EC894
    addi r0, r31, 0x1
    stw r0, 0x6a4(r3)
    b @L_801EC840
@L_801EC8C0:
    subfic r0, r31, 0x21
    li r5, -0x1
    slw r9, r5, r0
    andc r5, r12, r9
    addi r9, r30, 0x44
    lwz r10, 0x69c(r3)
    subfic r7, r31, 0x21
    addi r31, r7, 0x1
    slwi r7, r7, 2
    lwzu r12, 0x4(r10)
    add r9, r9, r7
    stw r10, 0x69c(r3)
    slwi r5, r5, 1
    stw r12, 0x6a0(r3)
    rlwimi r5, r12, 1, 31, 31
    lwzu r7, 0x4(r9)
    li r10, 0x2
    b @L_801EC91C
@L_801EC908:
    slwi r5, r5, 1
    addi r31, r31, 0x1
    lwzu r7, 0x4(r9)
    add r5, r5, r11
    addi r10, r10, 0x1
@L_801EC91C:
    cmpw r5, r7
    rlwnm r11, r12, r10, 31, 31
    bgt @L_801EC908
    stw r10, 0x6a4(r3)
    slwi r0, r31, 2
    add r7, r30, r0
    lwz r9, 0x40(r30)
    lwz r0, 0x8c(r7)
    add r0, r0, r9
    lbzx r5, r5, r0
@L_801EC944:
    clrlwi. r30, r5, 28
    srawi r7, r5, 4
    beq @L_801EC9E8
    add r6, r6, r7
    lwz r9, 0x6a4(r3)
    subfic r10, r9, 0x21
    lwz r7, 0x6a0(r3)
    subf. r11, r10, r30
    subi r12, r9, 0x1
    bgt @L_801EC984
    add r0, r9, r30
    slw r9, r7, r12
    stw r0, 0x6a4(r3)
    subfic r0, r30, 0x20
    srw r7, r9, r0
    b @L_801EC9B0
@L_801EC984:
    lwz r9, 0x69c(r3)
    slw r0, r7, r12
    lwzu r7, 0x4(r9)
    addi r11, r11, 0x1
    stw r7, 0x6a0(r3)
    srw r7, r7, r10
    stw r9, 0x69c(r3)
    add r0, r7, r0
    stw r11, 0x6a4(r3)
    subfic r11, r30, 0x20
    srw r7, r0, r11
@L_801EC9B0:
    cntlzw r5, r7
    subfic r0, r30, 0x20
    cmpw r5, r0
    ble @L_801EC9D0
    li r0, -0x1
    slw r0, r0, r30
    add r7, r0, r7
    addi r7, r7, 0x1
@L_801EC9D0:
    add r5, r8, r6
    lbz r0, 0x0(r5)
    extsh r5, r7
    slwi r0, r0, 1
    sthx r5, r4, r0
    b @L_801EC9F4
@L_801EC9E8:
    cmpwi r7, 0xf
    bne @L_801ECA00
    addi r6, r6, 0xf
@L_801EC9F4:
    addi r6, r6, 0x1
@L_801EC9F8:
    cmpwi r6, 0x40
    blt @L_801EC6C0
@L_801ECA00:
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    blr
