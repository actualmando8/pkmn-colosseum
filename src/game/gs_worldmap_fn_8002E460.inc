    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r27, r3
    lwz r0, lbl_8047A424(r13)
    lis r3, lbl_803A2518@ha
    addi r31, r3, lbl_803A2518@l
    li r3, 0x0
    clrlwi r5, r0, 16
    li r4, 0x3
    bl fn_8012A5B0
    lwz r0, lbl_8047A420(r13)
    mr r26, r3
    addi r3, r31, 0x170
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r0, r3
    addi r3, r31, 0xd18
    mr r29, r0
    li r4, 0xe4
    li r5, 0x8f
    bl fn_8010A5BC
    addi r3, r31, 0xcd0
    li r4, 0xe4
    li r5, 0x8f
    bl fn_8010A5BC
    mr r4, r26
    addi r3, r31, 0xd18
    bl fn_80109C88
    mr r4, r29
    addi r3, r31, 0xcd0
    bl fn_80109C88
    li r3, 0xdb
    li r4, 0x0
    bl fn_8010264C
    li r3, 0xdb
    bl fn_80104704
    li r4, 0x11a8
    bl fn_801046C8
    li r4, 0x0
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xf9b
    bl fn_801046C8
    li r4, 0x1
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xf9a
    bl fn_801046C8
    li r4, 0x1
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xfa3
    bl fn_801046C8
    li r4, 0x1
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xfa5
    bl fn_801046C8
    li r4, 0x1
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0x11a9
    bl fn_801046C8
    li r4, 0x0
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xf99
    bl fn_801046C8
    li r4, 0x1
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xf98
    bl fn_801046C8
    li r4, 0x1
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xfa4
    bl fn_801046C8
    li r4, 0x1
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xfa6
    bl fn_801046C8
    li r4, 0x1
    bl fn_80109220
    lfs f1, lbl_8047B9D0(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r30, 0x0
    li r29, 0x0
    b @L_8002E870
    @L_8002E604:
    bl fn_80105624
    lhz r26, 0x4(r3)
    addi r4, r1, 0x8
    li r3, 0x1
    bl fn_80073A44
    cmpwi r3, 0x0
    beq @L_8002E658
    li r3, 0x2
    li r4, 0x4448
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0xdb
    bl fn_80102510
    addi r3, r31, 0xd18
    bl fn_8010A420
    addi r3, r31, 0xcd0
    bl fn_8010A420
    li r0, 0x0
    stw r0, lbl_8047A42C(r13)
    b @L_8002EA48
    @L_8002E658:
    bl fn_8017B1AC
    cmpwi r3, 0x5
    beq @L_8002E764
    rlwinm r0, r26, 0, 27, 27
    lhz r3, 0x8(r1)
    cmplwi r0, 0x0
    andc r0, r3, r28
    mr r28, r3
    clrlwi r25, r0, 16
    beq @L_8002E6A4
    clrlwi r0, r30, 24
    cmplwi r0, 0x0
    bne @L_8002E69C
    li r3, 0x24
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    @L_8002E69C:
    li r30, 0x1
    b @L_8002E6EC
    @L_8002E6A4:
    rlwinm r0, r26, 0, 26, 26
    cmplwi r0, 0x0
    beq @L_8002E6EC
    clrlwi r0, r30, 24
    cmplwi r0, 0x0
    beq @L_8002E6D4
    li r3, 0x25
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    li r30, 0x0
    b @L_8002E6EC
    @L_8002E6D4:
    li r3, 0x25
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    li r30, 0x0
    b @L_8002E888
    @L_8002E6EC:
    clrlwi r0, r25, 31
    cmpwi r0, 0x0
    beq @L_8002E71C
    clrlwi r0, r29, 24
    cmplwi r0, 0x0
    bne @L_8002E714
    li r3, 0x24
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    @L_8002E714:
    li r29, 0x1
    b @L_8002E764
    @L_8002E71C:
    rlwinm r0, r25, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_8002E764
    clrlwi r0, r29, 24
    cmplwi r0, 0x0
    beq @L_8002E74C
    li r3, 0x25
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    li r29, 0x0
    b @L_8002E764
    @L_8002E74C:
    li r3, 0x25
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    li r29, 0x0
    b @L_8002E888
    @L_8002E764:
    bl fn_800F0308
    li r3, 0xdb
    bl fn_80104704
    li r4, 0x11a8
    bl fn_801046C8
    mr r4, r30
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xf9b
    bl fn_801046C8
    clrlwi r0, r30, 24
    cntlzw r0, r0
    srwi r26, r0, 5
    clrlwi r4, r26, 24
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xf9a
    bl fn_801046C8
    clrlwi r4, r26, 24
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xfa3
    bl fn_801046C8
    clrlwi r4, r26, 24
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xfa5
    bl fn_801046C8
    clrlwi r4, r26, 24
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0x11a9
    bl fn_801046C8
    mr r4, r29
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xf99
    bl fn_801046C8
    clrlwi r0, r29, 24
    cntlzw r0, r0
    srwi r26, r0, 5
    clrlwi r4, r26, 24
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xf98
    bl fn_801046C8
    clrlwi r4, r26, 24
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xfa4
    bl fn_801046C8
    clrlwi r4, r26, 24
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xfa6
    bl fn_801046C8
    clrlwi r4, r26, 24
    bl fn_80109220
    @L_8002E870:
    clrlwi r0, r30, 24
    cmplwi r0, 0x0
    beq @L_8002E604
    clrlwi r0, r29, 24
    cmplwi r0, 0x0
    beq @L_8002E604
    @L_8002E888:
    li r3, 0xdb
    bl fn_80104704
    li r4, 0x11a8
    bl fn_801046C8
    mr r4, r30
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xf9b
    bl fn_801046C8
    clrlwi r0, r30, 24
    cntlzw r0, r0
    srwi r28, r0, 5
    clrlwi r4, r28, 24
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xf9a
    bl fn_801046C8
    clrlwi r4, r28, 24
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xfa3
    bl fn_801046C8
    clrlwi r4, r28, 24
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xfa5
    bl fn_801046C8
    clrlwi r4, r28, 24
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0x11a9
    bl fn_801046C8
    mr r4, r29
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xf99
    bl fn_801046C8
    clrlwi r0, r29, 24
    cntlzw r0, r0
    srwi r28, r0, 5
    clrlwi r4, r28, 24
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xf98
    bl fn_801046C8
    clrlwi r4, r28, 24
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xfa4
    bl fn_801046C8
    clrlwi r4, r28, 24
    bl fn_80109220
    li r3, 0xdb
    bl fn_80104704
    li r4, 0xfa6
    bl fn_801046C8
    clrlwi r4, r28, 24
    bl fn_80109220
    clrlwi r0, r30, 24
    cmplwi r0, 0x0
    beq @L_8002E9A8
    clrlwi r0, r29, 24
    cmplwi r0, 0x0
    bne @L_8002E9E4
    @L_8002E9A8:
    li r3, 0xdb
    bl fn_80102510
    li r0, -0x1
    addi r3, r31, 0xd18
    stw r0, lbl_8047A428(r13)
    stw r0, lbl_8047A424(r13)
    stw r0, lbl_8047A420(r13)
    bl fn_8010A420
    addi r3, r31, 0xcd0
    bl fn_8010A420
    li r3, 0x1
    li r0, 0x2
    stb r3, lbl_8047A410(r13)
    stw r0, lbl_8047A42C(r13)
    b @L_8002EA48
    @L_8002E9E4:
    addi r3, r31, 0xd18
    bl fn_8010A420
    addi r3, r31, 0xcd0
    bl fn_8010A420
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    li r0, 0x3bfa
    subi r5, r27, 0x4
    subi r4, r3, 0x4
    mtctr r0
    @L_8002EA10:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_8002EA10
    li r0, 0xd
    lfs f1, lbl_8047B9D0(r2)
    stw r0, lbl_8047A42C(r13)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0xdb
    bl fn_80102510
    @L_8002EA48:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
