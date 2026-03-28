    nofralloc
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    dcbz r0, r4
    lwz r12, -21376(r13)
    lwz r11, 0x6a4(r3)
    addi r9, r12, 0x20
    lwz r10, 0x6a0(r3)
    addi r5, r11, 0x4
    cmpwi r11, 0x1c
    rlwnm r8, r10, r5, 27, 31
    bgt @L_801EB738
    lbzx r5, r12, r8
    lbzx r9, r9, r8
    cmpwi r5, 0xff
    beq @L_801EB694
    add r11, r11, r9
    stw r11, 0x6a4(r3)
    b @L_801EB8DC
@L_801EB694:
    addi r6, r12, 0x44
    addi r11, r11, 0x5
    li r0, 0x14
    li r5, 0x5
    addi r6, r6, 0x14
@L_801EB6A8:
    cmpwi r11, 0x21
    slwi r8, r8, 1
    beq @L_801EB6C8
    rlwnm r9, r10, r11, 31, 31
    lwzu r0, 0x4(r6)
    or r8, r8, r9
    addi r11, r11, 0x1
    b @L_801EB70C
@L_801EB6C8:
    lwz r9, 0x69c(r3)
    li r11, 0x1
    lwzu r10, 0x4(r9)
    lwzu r0, 0x4(r6)
    stw r9, 0x69c(r3)
    rlwimi r8, r10, 1, 31, 31
    stw r10, 0x6a0(r3)
    b @L_801EB6F8
@L_801EB6E8:
    slwi r8, r8, 1
    rlwnm r9, r10, r11, 31, 31
    lwzu r0, 0x4(r6)
    or r8, r8, r9
@L_801EB6F8:
    cmpw r8, r0
    addi r11, r11, 0x1
    addi r5, r5, 0x1
    bgt @L_801EB6E8
    b @L_801EB718
@L_801EB70C:
    cmpw r8, r0
    addi r5, r5, 0x1
    bgt @L_801EB6A8
@L_801EB718:
    stw r11, 0x6a4(r3)
    slwi r0, r5, 2
    add r5, r12, r0
    lwz r6, 0x40(r12)
    lwz r0, 0x8c(r5)
    add r0, r0, r6
    lbzx r5, r8, r0
    b @L_801EB8DC
@L_801EB738:
    cmpwi r11, 0x21
    lwz r8, 0x69c(r3)
    beq @L_801EB7F4
    cmpwi r11, 0x20
    rlwnm r5, r10, r5, 27, 31
    beq @L_801EB778
    lbzx r8, r12, r5
    lbzx r9, r9, r5
    cmpwi r8, 0xff
    add r5, r11, r9
    beq @L_801EB858
    cmpwi r5, 0x21
    stw r5, 0x6a4(r3)
    bgt @L_801EB858
    mr r5, r8
    b @L_801EB8DC
@L_801EB778:
    lwzu r10, 0x4(r8)
    stw r8, 0x69c(r3)
    rlwimi r5, r10, 4, 28, 31
    lbzx r8, r12, r5
    lbzx r9, r9, r5
    stw r10, 0x6a0(r3)
    cmpwi r8, 0xff
    stw r9, 0x6a4(r3)
    beq @L_801EB7A4
    mr r5, r8
    b @L_801EB8DC
@L_801EB7A4:
    addi r6, r12, 0x44
    li r11, 0x14
    addi r6, r6, 0x14
    slwi r8, r5, 27
    li r11, 0x5
    rlwimi r8, r10, 31, 1, 31
@L_801EB7BC:
    subfic r10, r11, 0x1f
    lwzu r0, 0x4(r6)
    srw r5, r8, r10
    cmpw r5, r0
    addi r11, r11, 0x1
    bgt @L_801EB7BC
    stw r11, 0x6a4(r3)
@L_801EB7D8:
    slwi r0, r11, 2
    lwz r7, 0x40(r12)
    add r6, r12, r0
    lwz r0, 0x8c(r6)
    add r0, r0, r7
    lbzx r5, r5, r0
    b @L_801EB8DC
@L_801EB7F4:
    lwzu r10, 0x4(r8)
    srwi r5, r10, 27
    stw r8, 0x69c(r3)
    lbzx r11, r12, r5
    lbzx r9, r9, r5
    cmpwi r11, 0xff
    stw r10, 0x6a0(r3)
    addi r9, r9, 0x1
    beq @L_801EB824
    stw r9, 0x6a4(r3)
    mr r5, r11
    b @L_801EB8DC
@L_801EB824:
    li r11, 0x5
    li r6, 0x14
@L_801EB82C:
    subfic r8, r11, 0x1f
    addi r11, r11, 0x1
    addi r6, r6, 0x4
    srw r5, r10, r8
    add r7, r12, r6
    lwz r0, 0x44(r7)
    cmpw r5, r0
    bgt @L_801EB82C
    addi r0, r11, 0x1
    stw r0, 0x6a4(r3)
    b @L_801EB7D8
@L_801EB858:
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
    b @L_801EB8B4
@L_801EB8A0:
    slwi r5, r5, 1
    addi r11, r11, 0x1
    lwzu r6, 0x4(r7)
    add r5, r5, r9
    addi r8, r8, 0x1
@L_801EB8B4:
    cmpw r5, r6
    rlwnm r9, r10, r8, 31, 31
    bgt @L_801EB8A0
    stw r8, 0x6a4(r3)
    slwi r0, r11, 2
    add r6, r12, r0
    lwz r7, 0x40(r12)
    lwz r0, 0x8c(r6)
    add r0, r0, r7
    lbzx r5, r5, r0
@L_801EB8DC:
    li r0, 0x20
    dcbz r4, r0
    li r7, 0x0
    li r0, 0x40
    dcbz r4, r0
    cmpwi r5, 0x0
    beq @L_801EB978
    lwz r7, 0x6a4(r3)
    subfic r8, r7, 0x21
    lwz r6, 0x6a0(r3)
    subfc. r9, r8, r5
    subi r10, r7, 0x1
    bgt @L_801EB928
    add r0, r7, r5
    slw r7, r6, r10
    stw r0, 0x6a4(r3)
    subfic r0, r5, 0x20
    srw r7, r7, r0
    b @L_801EB954
@L_801EB928:
    lwz r7, 0x69c(r3)
    slw r0, r6, r10
    lwzu r6, 0x4(r7)
    addi r9, r9, 0x1
    stw r6, 0x6a0(r3)
    srw r6, r6, r8
    stw r7, 0x69c(r3)
    add r0, r6, r0
    stw r9, 0x6a4(r3)
    subfic r9, r5, 0x20
    srw r7, r0, r9
@L_801EB954:
    extsh r0, r7
    cntlzw r6, r0
    subfic r0, r5, 0x20
    cmpw r6, r0
    ble @L_801EB978
    li r0, -0x1
    slw r0, r0, r5
    add r7, r0, r7
    addi r7, r7, 0x1
@L_801EB978:
    li r0, 0x60
    dcbz r4, r0
    lha r0, 0x684(r3)
    add r0, r0, r7
    sth r0, 0x684(r3)
    sth r0, 0x0(r4)
    lwz r8, -21280(r13)
    lwz r6, 0x6a4(r3)
    addi r7, r8, 0x20
    lwz r0, 0x6a0(r3)
    li r5, 0x1
    lis r9, -32728
    addi r10, r9, -25880
    b @L_801EBC9C
@L_801EB9B0:
    addi r31, r6, 0x4
    cmpwi r6, 0x1c
    rlwnm r12, r0, r31, 27, 31
    bgt @L_801EBA74
    lbzx r30, r8, r12
    lbzx r31, r7, r12
    cmpwi r30, 0xff
    beq @L_801EB9D8
    add r6, r6, r31
    b @L_801EBBFC
@L_801EB9D8:
    addi r6, r6, 0x5
    addi r9, r8, 0x44
    li r11, 0x14
    li r31, 0x5
    addi r9, r9, 0x14
@L_801EB9EC:
    cmpwi r6, 0x21
    slwi r12, r12, 1
    beq @L_801EBA0C
    rlwnm r30, r0, r6, 31, 31
    lwzu r11, 0x4(r9)
    or r12, r12, r30
    addi r6, r6, 0x1
    b @L_801EBA4C
@L_801EBA0C:
    lwz r30, 0x69c(r3)
    li r6, 0x1
    lwzu r0, 0x4(r30)
    lwzu r11, 0x4(r9)
    stw r30, 0x69c(r3)
    rlwimi r12, r0, 1, 31, 31
    b @L_801EBA38
@L_801EBA28:
    slwi r12, r12, 1
    rlwnm r30, r0, r6, 31, 31
    lwzu r11, 0x4(r9)
    or r12, r12, r30
@L_801EBA38:
    cmpw r12, r11
    addi r6, r6, 0x1
    addi r31, r31, 0x1
    bgt @L_801EBA28
    b @L_801EBA58
@L_801EBA4C:
    cmpw r12, r11
    addi r31, r31, 0x1
    bgt @L_801EB9EC
@L_801EBA58:
    slwi r9, r31, 2
    lwz r11, 0x40(r8)
    add r9, r8, r9
    lwz r9, 0x8c(r9)
    add r9, r9, r11
    lbzx r30, r12, r9
    b @L_801EBBFC
@L_801EBA74:
    cmpwi r6, 0x21
    lwz r12, 0x69c(r3)
    beq @L_801EBAB0
    cmpwi r6, 0x20
    rlwnm r31, r0, r31, 27, 31
    beq @L_801EBB14
    lbzx r30, r8, r31
    lbzx r29, r7, r31
    cmpwi r30, 0xff
    add r31, r6, r29
    beq @L_801EBB80
    cmpwi r31, 0x21
    bgt @L_801EBB80
    mr r6, r31
    b @L_801EBBFC
@L_801EBAB0:
    lwzu r0, 0x4(r12)
    srwi r31, r0, 27
    stw r12, 0x69c(r3)
    lbzx r30, r8, r31
    lbzx r12, r7, r31
    cmpwi r30, 0xff
    addi r6, r12, 0x1
    beq @L_801EBAD4
    b @L_801EBBFC
@L_801EBAD4:
    li r30, 0x5
    li r6, 0x14
@L_801EBADC:
    subfic r12, r30, 0x1f
    addi r30, r30, 0x1
    addi r6, r6, 0x4
    srw r31, r0, r12
    add r12, r8, r6
    lwz r9, 0x44(r12)
    cmpw r31, r9
    bgt @L_801EBADC
    lwz r11, 0x40(r8)
    addi r6, r30, 0x1
    lwz r9, 0x8c(r12)
    add r9, r9, r11
    lbzx r30, r31, r9
    b @L_801EBBFC
@L_801EBB14:
    lwzu r0, 0x4(r12)
    stw r12, 0x69c(r3)
    rlwimi r31, r0, 4, 28, 31
    lbzx r30, r8, r31
    lbzx r6, r7, r31
    cmpwi r30, 0xff
    beq @L_801EBB34
    b @L_801EBBFC
@L_801EBB34:
    addi r9, r8, 0x44
    li r6, 0x14
    addi r9, r9, 0x14
    slwi r12, r31, 27
    li r6, 0x5
    rlwimi r12, r0, 31, 1, 31
@L_801EBB4C:
    subfic r30, r6, 0x1f
    lwzu r11, 0x4(r9)
    srw r31, r12, r30
    cmpw r31, r11
    addi r6, r6, 0x1
    bgt @L_801EBB4C
    slwi r9, r6, 2
    lwz r11, 0x40(r8)
    add r9, r8, r9
    lwz r9, 0x8c(r9)
    add r9, r9, r11
    lbzx r30, r31, r9
    b @L_801EBBFC
@L_801EBB80:
    subfic r9, r6, 0x21
    li r11, -0x1
    slw r9, r11, r9
    andc r31, r0, r9
    addi r9, r8, 0x44
    lwz r12, 0x69c(r3)
    subfic r11, r6, 0x21
    addi r30, r11, 0x1
    slwi r11, r11, 2
    lwzu r0, 0x4(r12)
    add r9, r9, r11
    stw r12, 0x69c(r3)
    slwi r31, r31, 1
    rlwimi r31, r0, 1, 31, 31
    lwzu r11, 0x4(r9)
    li r6, 0x2
    b @L_801EBBD8
@L_801EBBC4:
    slwi r31, r31, 1
    addi r30, r30, 0x1
    lwzu r11, 0x4(r9)
    add r31, r31, r29
    addi r6, r6, 0x1
@L_801EBBD8:
    cmpw r31, r11
    rlwnm r29, r0, r6, 31, 31
    bgt @L_801EBBC4
    slwi r9, r30, 2
    lwz r11, 0x40(r8)
    add r9, r8, r9
    lwz r9, 0x8c(r9)
    add r9, r9, r11
    lbzx r30, r31, r9
@L_801EBBFC:
    andi. r29, r30, 0xf
    srawi r30, r30, 4
    beq @L_801EBC8C
    add r5, r5, r30
    subfic r31, r6, 0x21
    subfc. r12, r31, r29
    subi r9, r6, 0x1
    bgt @L_801EBC30
    add r6, r6, r29
    slw r11, r0, r9
    subfic r9, r29, 0x20
    srw r30, r11, r9
    b @L_801EBC54
@L_801EBC30:
    lwz r11, 0x69c(r3)
    slw r9, r0, r9
    lwzu r0, 0x4(r11)
    addi r6, r12, 0x1
    stw r11, 0x69c(r3)
    srw r11, r0, r31
    add r9, r11, r9
    subfic r12, r29, 0x20
    srw r30, r9, r12
@L_801EBC54:
    cntlzw r11, r30
    subfic r9, r29, 0x20
    cmpw r11, r9
    ble @L_801EBC74
    li r9, -0x1
    slw r9, r9, r29
    add r30, r9, r30
    addi r30, r30, 0x1
@L_801EBC74:
    add r9, r10, r5
    lbz r9, 0x0(r9)
    extsh r11, r30
    slwi r9, r9, 1
    sthx r11, r4, r9
    b @L_801EBC98
@L_801EBC8C:
    cmpwi r30, 0xf
    bne @L_801EBCA4
    addi r5, r5, 0xf
@L_801EBC98:
    addi r5, r5, 0x1
@L_801EBC9C:
    cmpwi r5, 0x40
    blt @L_801EB9B0
@L_801EBCA4:
    stw r6, 0x6a4(r3)
    stw r0, 0x6a0(r3)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    addi r1, r1, 0x20
    blr
