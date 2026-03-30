    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0xa6
    bl fn_80104704
    bl fn_801040A0
    lwz r4, 0x0(r3)
    cmplwi r4, 0x0
    bne @L_8007C660
    li r4, 0x0
    b @L_8007C69C
@L_8007C660:
    li r0, 0x0
    stb r0, 0xc8(r4)
    b @L_8007C690
@L_8007C66C:
    bl fn_800F0308
    li r3, 0xa6
    bl fn_80104704
    bl fn_801040A0
    lwz r4, 0x0(r3)
    cmplwi r4, 0x0
    bne @L_8007C690
    li r4, 0x0
    b @L_8007C69C
@L_8007C690:
    lbz r0, 0xb6(r4)
    cmplwi r0, 0x0
    bne @L_8007C66C
@L_8007C69C:
    cmplwi r4, 0x0
    beq @L_8007C754
    lwz r0, 0xac(r4)
    cmpwi r0, 0x0
    bgt @L_8007C6B4
    b @L_8007C754
@L_8007C6B4:
    lwz r3, 0xa4(r4)
    li r0, 0x0
    stw r3, 0xa0(r4)
    lbz r3, 0xb5(r4)
    stb r3, 0xb4(r4)
    stw r0, 0xa4(r4)
    stb r0, 0xb5(r4)
    lwz r3, 0xa4(r4)
    lwz r0, 0xa0(r4)
    cmpw r3, r0
    bne @L_8007C6F8
    lbz r3, 0xb5(r4)
    lbz r0, 0xb4(r4)
    extsb r3, r3
    extsb r0, r0
    cmpw r3, r0
    beq @L_8007C754
@L_8007C6F8:
    li r3, 0x1
    li r0, 0x0
    stb r3, 0xb6(r4)
    li r3, 0xa6
    stw r0, 0xb8(r4)
    bl fn_80104704
    bl fn_801040A0
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    beq @L_8007C754
    li r0, 0x0
    stb r0, 0xc8(r3)
    b @L_8007C748
@L_8007C72C:
    bl fn_800F0308
    li r3, 0xa6
    bl fn_80104704
    bl fn_801040A0
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    beq @L_8007C754
@L_8007C748:
    lbz r0, 0xb6(r3)
    cmplwi r0, 0x0
    bne @L_8007C72C
@L_8007C754:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
