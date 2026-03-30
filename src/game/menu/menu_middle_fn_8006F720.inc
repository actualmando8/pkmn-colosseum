    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r29, r3
    lis r5, lbl_80267EA8@ha
    li r4, 0x0
    addi r30, r5, lbl_80267EA8@l
    bl fn_801040D0
    mr r31, r3
    addi r28, r30, 0x3dc
    li r26, 0x0
    @L_8006F750:
    lhz r4, 0x0(r28)
    mr r3, r29
    bl fn_801046C8
    lwz r0, 0x4(r28)
    addi r28, r28, 0x8
    addi r26, r26, 0x1
    stw r0, 0x4c(r3)
    cmplwi r26, 0x1a
    blt @L_8006F750
    lwz r3, 0x4(r29)
    bl fn_801022B8
    subi r0, r3, 0x9ca
    cmplwi r0, 0x20
    bgt @L_8006F7E0
    lis r3, jumptable_802EE06C@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_802EE06C@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r25, 0x0
    b @L_8006F7E4
    li r25, 0x1
    b @L_8006F7E4
    li r25, 0x2
    b @L_8006F7E4
    li r25, 0x3
    b @L_8006F7E4
    li r25, 0x4
    b @L_8006F7E4
    li r25, 0x5
    b @L_8006F7E4
    li r25, 0x6
    b @L_8006F7E4
    li r25, 0x7
    b @L_8006F7E4
    @L_8006F7E0:
    li r25, 0x8
    @L_8006F7E4:
    bl fn_80077BD0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8006F824
    addi r28, r30, 0x3cc
    li r26, 0x0
    @L_8006F7FC:
    lhz r4, 0x0(r28)
    mr r3, r29
    bl fn_801046C8
    li r4, 0x0
    bl fn_80109220
    addi r28, r28, 0x2
    addi r26, r26, 0x1
    cmplwi r26, 0x8
    blt @L_8006F7FC
    b @L_8006F884
    @L_8006F824:
    addi r27, r30, 0x3cc
    li r26, 0x0
    @L_8006F82C:
    lhz r4, 0x0(r27)
    mr r3, r29
    bl fn_801046C8
    subf r0, r25, r26
    mr r28, r3
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    li r3, 0x191
    bl fn_8005D830
    lwz r0, 0xc(r28)
    cmplw r0, r3
    beq @L_8006F874
    lhz r4, 0x0(r27)
    mr r3, r29
    li r5, 0x191
    bl fn_801081F8
    @L_8006F874:
    addi r27, r27, 0x2
    addi r26, r26, 0x1
    cmplwi r26, 0x8
    blt @L_8006F82C
    @L_8006F884:
    mr r3, r29
    li r4, 0xd80
    bl fn_801046C8
    lbz r4, 0xc(r31)
    bl fn_80109220
    mr r3, r29
    li r4, 0x9eb
    bl fn_801046C8
    lbz r0, 0xc(r31)
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    mr r3, r29
    li r4, 0x9ec
    bl fn_801046C8
    lwz r0, 0x8(r31)
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    mr r3, r29
    li r4, 0x9ed
    bl fn_801046C8
    lwz r0, 0x8(r31)
    subfic r0, r0, 0x1
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    mr r3, r29
    li r4, 0x9ee
    bl fn_801046C8
    lwz r0, 0x8(r31)
    subfic r0, r0, 0x2
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    mr r3, r29
    li r4, 0x9ef
    bl fn_801046C8
    lbz r4, 0xd(r31)
    bl fn_80109220
    mr r3, r29
    li r4, 0x9f0
    bl fn_801046C8
    lbz r0, 0xd(r31)
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    mr r3, r29
    li r4, 0x9f1
    bl fn_801046C8
    lbz r4, 0xe(r31)
    bl fn_80109220
    mr r3, r29
    li r4, 0x9f2
    bl fn_801046C8
    lbz r0, 0xe(r31)
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    mr r3, r29
    li r4, 0x9f3
    bl fn_801046C8
    lbz r4, 0xf(r31)
    bl fn_80109220
    mr r3, r29
    li r4, 0x9f4
    bl fn_801046C8
    lbz r0, 0xf(r31)
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    mr r3, r29
    li r4, 0x9f5
    bl fn_801046C8
    lbz r4, 0x10(r31)
    bl fn_80109220
    mr r3, r29
    li r4, 0x8a2
    bl fn_801046C8
    lbz r0, 0x10(r31)
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    mr r3, r29
    li r4, 0x9d4
    bl fn_801046C8
    lha r0, 0x2(r31)
    lha r6, 0x4(r31)
    mulli r0, r0, 0x6
    srawi r5, r6, 31
    srwi r4, r0, 31
    subfc r0, r0, r6
    adde r0, r5, r4
    clrlwi r4, r0, 24
    bl fn_80109220
    mr r3, r29
    li r4, 0xfb0
    bl fn_801046C8
    li r28, 0x0
    mr r26, r3
    bl fn_80077BD0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8006FA64
    lwz r3, 0x4(r29)
    bl fn_801022B8
    cmpwi r3, 0x9cd
    bne @L_8006FA64
    lwz r0, 0x8(r31)
    cmpwi r0, 0x2
    bne @L_8006FA64
    li r28, 0x1
    @L_8006FA64:
    clrlwi r4, r28, 24
    mr r3, r26
    bl fn_80109220
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    bne @L_8006FB18
    addi r27, r30, 0x4ac
    li r28, 0x0
    @L_8006FA90:
    lhz r4, 0x0(r27)
    mr r3, r29
    bl fn_801046C8
    li r4, 0x0
    bl fn_80109220
    addi r27, r27, 0x2
    addi r28, r28, 0x1
    cmplwi r28, 0x12
    blt @L_8006FA90
    mr r3, r29
    li r4, 0xd9b
    bl fn_801046C8
    li r0, 0x4238
    li r4, 0xd9c
    stw r0, 0x4c(r3)
    mr r3, r29
    bl fn_801046C8
    li r0, 0x0
    li r4, 0x9d3
    stw r0, 0x4c(r3)
    mr r3, r29
    bl fn_801046C8
    li r0, 0x4238
    li r4, 0xd9a
    stw r0, 0x4c(r3)
    mr r3, r29
    bl fn_801046C8
    li r0, 0x0
    li r4, 0x9d4
    stw r0, 0x4c(r3)
    mr r3, r29
    bl fn_801046C8
    li r0, 0x0
    stw r0, 0x4c(r3)
    @L_8006FB18:
    lbz r0, 0x1(r29)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_8006FB3C
    bge @L_8006FB34
    cmpwi r0, 0x0
    b @L_8006FBD8
    @L_8006FB34:
    cmpwi r0, 0x6
    b @L_8006FBD8
    @L_8006FB3C:
    lwz r3, 0x4(r29)
    bl fn_801022B8
    cmpwi r3, 0x9d2
    bne @L_8006FB54
    li r0, 0x1
    stb r0, 0x98(r29)
    @L_8006FB54:
    bl fn_80105624
    lhz r0, 0x4(r3)
    rlwinm r0, r0, 0, 27, 27
    cmpwi r0, 0x0
    beq @L_8006FBD8
    bl fn_80077BD0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8006FBAC
    lwz r3, 0x4(r29)
    li r4, 0x9d2
    bl fn_80102138
    extsb r3, r3
    li r0, 0x0
    stb r3, 0xd(r1)
    addi r4, r1, 0x8
    stb r0, 0xc(r1)
    lhz r0, 0xc(r1)
    sth r0, 0x8(r1)
    lwz r3, 0x4(r29)
    bl fn_801044D0
    b @L_8006FBD8
    @L_8006FBAC:
    lbz r0, 0x95(r29)
    li r3, 0x0
    extsb r0, r0
    cmpwi r0, 0x3
    bne @L_8006FBD0
    lwz r0, 0x8(r31)
    cmpwi r0, 0x2
    bne @L_8006FBD0
    li r3, 0x1
    @L_8006FBD0:
    clrlwi r0, r3, 24
    stb r0, 0x98(r29)
    @L_8006FBD8:
    mr r3, r29
    addi r4, r30, 0x4d0
    li r5, 0x2
    bl fn_80070D84
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
