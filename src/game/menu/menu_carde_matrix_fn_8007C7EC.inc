    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    li r3, 0xa6
    bl fn_80104704
    bl fn_801040A0
    lwz r31, 0x0(r3)
    cmplwi r31, 0x0
    beq @L_8007CA9C
    lbz r0, 0xb6(r31)
    cmplwi r0, 0x0
    beq @L_8007C838
    lis r3, lbl_80268D78@ha
    lis r5, lbl_80268DA0@ha
    addi r3, r3, lbl_80268D78@l
    li r4, 0x594
    addi r5, r5, lbl_80268DA0@l
    bl fn_80196E10
@L_8007C838:
    lwz r3, 0xb0(r31)
    lwz r30, 0xa4(r31)
    cmplwi r3, 0x0
    beq @L_8007C888
    bl fn_800E202C
    mr r27, r3
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_8007C870
    lis r3, lbl_80268D78@ha
    li r4, 0x1ab
    addi r3, r3, lbl_80268D78@l
    la r5, lbl_8047C140(r2)
    bl fn_80196E10
@L_8007C870:
    mr r3, r27
    bl fn_800E24B0
    mr r3, r27
    bl fn_800E209C
    li r0, 0x0
    stw r0, 0xb0(r31)
@L_8007C888:
    li r3, 0x0
    bl fn_80083BF8
    mr r26, r3
    stw r3, 0xac(r31)
    cmpwi r26, 0x0
    beq @L_8007C93C
    rlwinm r27, r26, 2, 0, 29
    li r4, 0x20
    addi r0, r27, 0x1f
    rlwinm r3, r0, 0, 0, 26
    bl fn_800E2C04
    mr r28, r3
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_8007C8D8
    lis r3, lbl_80268D78@ha
    li r4, 0x1a2
    addi r3, r3, lbl_80268D78@l
    la r5, lbl_8047C140(r2)
    bl fn_80196E10
@L_8007C8D8:
    mr r3, r28
    bl fn_800E27B0
    mr r29, r3
    mr r5, r27
    li r4, 0x0
    bl memset
    li r28, 0x0
    stw r29, 0xb0(r31)
    mr r27, r28
    b @L_8007C91C
@L_8007C900:
    mr r4, r28
    li r3, 0x0
    bl fn_80083AF4
    lwz r4, 0xb0(r31)
    addi r28, r28, 0x1
    stwx r3, r4, r27
    addi r27, r27, 0x4
@L_8007C91C:
    cmpw r28, r26
    blt @L_8007C900
    lis r4, fn_8007FD64@ha
    lwz r3, 0xb0(r31)
    addi r6, r4, fn_8007FD64@l
    li r5, 0x4
    mr r4, r26
    bl fn_800CA620
@L_8007C93C:
    li r0, -0x1
    li r5, 0x0
    stw r0, 0xa4(r31)
    mr r6, r5
    lwz r0, 0xac(r31)
    mtctr r0
    cmpwi r0, 0x0
    ble @L_8007C988
@L_8007C95C:
    lwz r3, 0xb0(r31)
    lha r4, 0xaa(r31)
    lwzx r3, r3, r6
    lbz r0, 0x1a(r3)
    cmpw r4, r0
    bne @L_8007C97C
    stw r5, 0xa4(r31)
    b @L_8007C988
@L_8007C97C:
    addi r6, r6, 0x4
    addi r5, r5, 0x1
    bdnz @L_8007C95C
@L_8007C988:
    lwz r0, 0xa4(r31)
    cmpwi r0, 0x0
    bge @L_8007C9A8
    lwz r3, 0xac(r31)
    cmpw r3, r30
    bgt @L_8007C9A8
    subi r0, r3, 0x1
    stw r0, 0xa4(r31)
@L_8007C9A8:
    lwz r0, 0xa4(r31)
    stw r0, 0xa0(r31)
    lwz r0, 0xac(r31)
    cmpwi r0, 0x0
    ble @L_8007C9C8
    lwz r0, 0xa4(r31)
    cmpwi r0, 0x0
    bge @L_8007C9D0
@L_8007C9C8:
    li r28, 0x0
    b @L_8007C9DC
@L_8007C9D0:
    lwz r3, 0xb0(r31)
    rlwinm r0, r0, 2, 0, 29
    lwzx r28, r3, r0
@L_8007C9DC:
    cmplwi r28, 0x0
    beq @L_8007CA18
    lbz r29, 0xb5(r31)
    b @L_8007CA08
@L_8007C9EC:
    mr r3, r28
    mr r4, r29
    bl fn_80082A88
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8007CA14
    subi r29, r29, 0x1
@L_8007CA08:
    extsb r0, r29
    cmpwi r0, 0x0
    bgt @L_8007C9EC
@L_8007CA14:
    stb r29, 0xb5(r31)
@L_8007CA18:
    lwz r3, 0xa4(r31)
    lwz r0, 0xa0(r31)
    cmpw r3, r0
    bne @L_8007CA40
    lbz r3, 0xb5(r31)
    lbz r0, 0xb4(r31)
    extsb r3, r3
    extsb r0, r0
    cmpw r3, r0
    beq @L_8007CA9C
@L_8007CA40:
    li r3, 0x1
    li r0, 0x0
    stb r3, 0xb6(r31)
    li r3, 0xa6
    stw r0, 0xb8(r31)
    bl fn_80104704
    bl fn_801040A0
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    beq @L_8007CA9C
    li r0, 0x0
    stb r0, 0xc8(r3)
    b @L_8007CA90
@L_8007CA74:
    bl fn_800F0308
    li r3, 0xa6
    bl fn_80104704
    bl fn_801040A0
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    beq @L_8007CA9C
@L_8007CA90:
    lbz r0, 0xb6(r3)
    cmplwi r0, 0x0
    bne @L_8007CA74
@L_8007CA9C:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

